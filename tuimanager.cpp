#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent)
    : QObject{parent}
{

}

TUIManager::~TUIManager()
{
    // Detenemos el controlador de consola
    consoleThread->quit();
    consoleThread->wait();

    delete consoleListener;
    delete consoleThread;

    // Liberacion de memoria
    delete consoleOutput;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void TUIManager::init()
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Consola
    consoleThread = new QThread(this);
    consoleListener = new ConsoleListener();

    consoleListener->moveToThread(consoleThread);

    connect(consoleThread, &QThread::started, consoleListener, &ConsoleListener::init);

    connect(consoleListener, &ConsoleListener::newLine, this, &TUIManager::consoleReadyLine);

    consoleThread->start();

    // Mostrar mensajes por consola
    consoleOutput = new QTextStream(stdout, QIODevice::OpenModeFlag::WriteOnly);

    consoleWelcome();
    consoleWait();

    // Mensaje de inicio
    logFile->println("Cargado");
}

void TUIManager::consoleReadyLine(const QString line)
{
    // Comando convertido a minusculas
    const QString cmd = line.toLower();

    // Comando de cierre
    if (cmd == "exit")
    {
        *consoleOutput << "Cerrando programa controlador" << Qt::endl;

        emit closedApplication();
    }

    // Comando para limpiar la pantalla
    else if (cmd == "clear")
    {
        consoleClear();
        consoleWelcome();
        consoleWait();
    }

    // Comando no conocido
    else
    {
        *consoleOutput << "Comando no valido para: " << line << Qt::endl;
        consoleWait();
    }
}

void TUIManager::consoleWelcome()
{
    // Dialogo de bienvenida
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Controlador del sistema de aireacion." << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Tesis de grado para recibir el" << Qt::endl;
    *consoleOutput << "titulo de Ingeniero en Mecatronica." << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Gianluca Lovatto y Gabriel Aguirre" << Qt::endl;
    *consoleOutput << "Facultad de Ciencias de la" << Qt::endl;
    *consoleOutput << "Alimentacion de la Universidad" << Qt::endl;
    *consoleOutput << "Nacional de Entre Rios." << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Qt: " << QT_VERSION_STR << Qt::endl;
    *consoleOutput << "GLibc: " << gnu_get_libc_version() << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
}

void TUIManager::consoleClear()
{
    // Se limpia la pantalla
    QProcess::execute("clear", QStringList());
}

void TUIManager::consoleWait()
{
    // Mensaje de espera
    *consoleOutput << "Ingrese comando: ";
    consoleOutput->flush();
}
