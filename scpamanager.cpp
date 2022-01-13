#include "scpamanager.h"

SCPAManager::SCPAManager(QObject *parent) : QObject(parent)
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Hilos
    // TUI
    tuiThread = new QThread(this);
    tuiManager = new TUIManager();

    tuiManager->moveToThread(tuiThread);

    tuiManager->setObjectName("TUIManager");

    connect(tuiThread, &QThread::started, tuiManager, &TUIManager::init);

    connect(tuiManager, &TUIManager::closedApplication, this, &SCPAManager::deleteLater);

    tuiThread->start();

    // HMI Server
    hmiServerThread = new QThread(this);
    hmiServerManager = new HMIServerManager();

    hmiServerManager->moveToThread(hmiServerThread);

    hmiServerManager->setObjectName("HMIServerManager");

    connect(hmiServerThread, &QThread::started, hmiServerManager, &HMIServerManager::init);

    hmiServerThread->start();

    // Interconexiones
    //connect(tuiManager, &TUIManager::getHmiServerStatus, hmiServerManager, &HMIServerManager::getHmiServerStatus);
    //connect(hmiServerManager, &HMIServerManager::hmiServerStatus, tuiManager, &TUIManager::hmiServerStatus);

    // Inicio
    logFile->println("Cargado");
}

SCPAManager::~SCPAManager()
{
    // Cierre de hilos
    // TUI
    tuiThread->quit();
    tuiThread->wait();

    delete tuiManager;
    delete tuiThread;

    // HMI Server
    hmiServerThread->quit();
    hmiServerThread->wait();

    delete hmiServerManager;
    delete hmiServerThread;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}
