#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent) : QObject(parent)
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->create("TUIManager");

    logFile->println("Controlador de consola iniciado");
}

TUIManager::~TUIManager()
{
    // Liberacion de memoria
    delete consoleOutput;

    // Cierre del archivo de log
    logFile->println("Controlador de consola finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void TUIManager::init()
{
    // Evento de consola
    consoleListener = new ConsoleListener(this);

    connect(consoleListener, &ConsoleListener::newLine, this, &TUIManager::consoleReadyLine);

    // Mostrar mensajes por consola
    consoleOutput = new QTextStream(stdout, QIODevice::OpenModeFlag::WriteOnly);

    consoleWelcome();
    consoleWait();

    // Mensaje de inicio
    logFile->println("Controlador de consola cargado");
}

void TUIManager::consoleReadyLine(const QString &line)
{
    // Comando de cierre
    if (line == "exit")
    {
        *consoleOutput << "Cerrando programa controlador" << Qt::endl;

        delete consoleListener;

        emit closeApplication();

        return;
    }

    // Comando para limpiar la pantalla
    else if (line == "clear")
    {
        consoleClear();
        consoleWelcome();
    }

    // Comando no conocido
    else
    {
        *consoleOutput << "Comando no valido para: " << line << Qt::endl;
    }

    consoleWait();
}

void TUIManager::consoleWelcome()
{
    // Dialogo de bienvenida
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Controlador del sistema de aireacion" << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Tesis de grado para recibir el" << Qt::endl;
    *consoleOutput << "titulo de Ingeniero en Mecatronica" << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Gianluca Lovatto y Gabriel Aguirre" << Qt::endl;
    *consoleOutput << "Facultad de Ciencias de la" << Qt::endl;
    *consoleOutput << "Alimentacion de la Universidad" << Qt::endl;
    *consoleOutput << "Nacional de Entre Rios." << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Qt: " << QT_VERSION_STR << Qt::endl;
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
