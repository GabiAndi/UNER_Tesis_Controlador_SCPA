#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent) : QObject(parent)
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->create("TUIManager");

    logFile->println("Controlador de consola iniciado");

    // Evento de consola
    consoleListener = new ConsoleListener(this);

    connect(consoleListener, &ConsoleListener::newLine, this, &TUIManager::consoleReadyLine);
}

TUIManager::~TUIManager()
{
    // Cierre del archivo de log
    logFile->println("Controlador de consola finalizado");

    logFile->close();

    // Se borran los recursos utilizados
    delete logFile;
}

void TUIManager::consoleReadyLine(const QString &line)
{
    // Comando de cierre
    if (line == "exit")
    {
        delete consoleListener;

        emit closeApplication();
    }
}
