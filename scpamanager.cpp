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
    // Cierre de hilos
    // TUI
    tuiThread->quit();
    tuiThread->wait();

    // HMI Server
    hmiServerThread->quit();
    hmiServerThread->wait();

    // TUI
    delete tuiManager;
    delete tuiThread;

    // HMI Server
    delete hmiServerManager;
    delete hmiServerThread;

    // Cierre del archivo de log
    logFile->println("Controlador de aplicacion finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void SCPAManager::init()
{
    // Hilos
    // TUI
    tuiThread = new QThread(this);
    tuiManager = new TUIManager();

    tuiManager->moveToThread(tuiThread);

    connect(tuiThread, &QThread::started, tuiManager, &TUIManager::init);
    connect(tuiManager, &TUIManager::closeApplication, this, &SCPAManager::deleteLater);

    tuiThread->start();

    // HMI Server
    hmiServerThread = new QThread(this);
    hmiServerManager = new HMIServerManager();

    hmiServerManager->moveToThread(hmiServerThread);

    connect(hmiServerThread, &QThread::started, hmiServerManager, &HMIServerManager::init);

    hmiServerThread->start();

    // Inicio
    logFile->println("Controlador de aplicacion cargado");
}
