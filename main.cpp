#include <QCoreApplication>
#include <QThread>
#include <QSemaphore>
#include <QDebug>
#include <QStringList>

const int N = 10; // Recurso compartido
int Ent = 0;

QStringList recursos; // Lista de los procesos en el recurso
QSemaphore semMaxProcesses(N); // Semaphore para limitar el número de procesos simultáneos

// Proceso A
class ProcesoA : public QThread
{
public:
    ProcesoA(int id)
        : id(id) {}

protected:
    void run() override
    {
        qDebug() << "Proceso A" << id << ": Solicitarlo";
        semMaxProcesses.acquire(); // Esperar hasta que haya permisos disponibles
        qDebug() << "Proceso A" << id << ": Entrar";
        Ent++;
        qDebug() << "Valor de Ent:" << Ent;

        // Simular el uso del recurso compartido
        qDebug() << "Proceso A" << id << ": Usarlo";
        QThread::msleep(2000); // Simulación de uso
        qDebug() << "Proceso A" << id << ": Liberarlo";
        Ent--;

        // Insertar la letra del proceso en el recurso
        insertarEnRecurso(QString("A%1").arg(id));

        semMaxProcesses.release(); // Liberar el permiso
        qDebug() << "Valor de Ent:" << Ent;

        // Imprimir la información del recurso
        imprimirInformacionRecurso();
    }

private:
    int id;

    void insertarEnRecurso(const QString &letra)
    {
        if (recursos.size() >= N) {
            // Si la lista ya tiene N elementos, reemplazar el más antiguo
            recursos.takeFirst();
        }
        recursos.append(letra);
    }

    void imprimirInformacionRecurso()
    {
        qDebug() << " Recurso: " << recursos.join(", ");
    }
};

// Proceso B
class ProcesoB : public QThread
{
public:
    ProcesoB(int id)
        : id(id) {}

protected:
    void run() override
    {
        qDebug() << "Proceso B" << id << ": Solicitarlo";
        semMaxProcesses.acquire(); // Esperar hasta que haya permisos disponibles
        qDebug() << "Proceso B" << id << ": Entrar";
        Ent++;
        qDebug() << "Valor de Ent:" << Ent;

        // Simular el uso del recurso compartido
        qDebug() << "Proceso B" << id << ": Usarlo";
        QThread::msleep(2000); // Simulación de uso
        qDebug() << "Proceso B" << id << ": Liberarlo";
        Ent--;

        // Insertar la letra del proceso en el recurso
        insertarEnRecurso(QString("B%1").arg(id));

        semMaxProcesses.release(); // Liberar el permiso
        qDebug() << "Valor de Ent:" << Ent;

        // Imprimir la información del recurso
        imprimirInformacionRecurso();
    }

private:
    int id;

    void insertarEnRecurso(const QString &letra)
    {
        if (recursos.size() >= N) {
            // Si la lista ya tiene N elementos, reemplazar el más antiguo
            recursos.takeFirst();
        }
        recursos.append(letra);
    }

    void imprimirInformacionRecurso()
    {
        qDebug() << "Recurso: " << recursos.join(", ");
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Crear instancias de 10 procesos A y 10 procesos B
    QList<ProcesoA *> procesosA;
    QList<ProcesoB *> procesosB;

    for (int i = 0; i < 100; ++i) {
        procesosA.append(new ProcesoA(i + 1));
        procesosB.append(new ProcesoB(i + 1));
    }

    // Iniciar los procesos
    for (int i = 0; i < 100; ++i) {
        procesosA[i]->start();
        procesosB[i]->start();
    }

    // Esperar a que los procesos terminen
    for (int i = 0; i < 100; ++i) {
        procesosA[i]->wait();
        procesosB[i]->wait();
    }

    // Liberar la memoria de las instancias
    qDeleteAll(procesosA);
    qDeleteAll(procesosB);

    return a.exec();
}
