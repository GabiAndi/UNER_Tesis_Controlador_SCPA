#include "scpamanager.h"

SCPAManager::SCPAManager(QObject *parent) : QObject(parent)
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->create("SCPAManager");

    logFile->println("Controlador de aplicacion iniciado");
}

SCPAManager::~SCPAManager()
{
    // Cierre del archivo de log
    logFile->println("Controlador de aplicacion finalizado");

    logFile->close();

    // Cierre de hilos
    tuiThread->quit();
    tuiThread->wait(threadExitTimeOut);

    // Se borran los recursos utilizados
    delete logFile;

    delete tuiManager;
    delete tuiThread;
}

void SCPAManager::start()
{
    // Inicio
    logFile->println("Controlador de aplicacion cargado");

    // Hilos
    // TUI
    tuiManager = new TUIManager();
    tuiThread = new QThread(this);

    tuiManager->moveToThread(tuiThread);

    connect(tuiManager, &TUIManager::closeApplication, this, &SCPAManager::deleteLater);

    tuiThread->start();
}
