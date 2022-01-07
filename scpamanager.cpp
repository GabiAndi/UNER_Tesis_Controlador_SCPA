#include "scpamanager.h"

SCPAManager::SCPAManager(QObject *parent) : QObject(parent)
{

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

    // Estados
    // Estado de la aplicación
    delete applicationState;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void SCPAManager::init()
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Estados
    // Estado de la applicación
    applicationState = new ApplicationState(this);

    connect(applicationState, &ApplicationState::closedApplication, this, &SCPAManager::deleteLater);

    // Hilos
    // TUI
    tuiThread = new QThread(this);
    tuiManager = new TUIManager(applicationState);

    tuiManager->moveToThread(tuiThread);

    tuiManager->setObjectName("TUIManager");

    connect(tuiThread, &QThread::started, tuiManager, &TUIManager::init);

    tuiThread->start();

    // HMI Server
    hmiServerThread = new QThread(this);
    hmiServerManager = new HMIServerManager();

    hmiServerManager->moveToThread(hmiServerThread);

    hmiServerManager->setObjectName("HMIServerManager");

    connect(hmiServerThread, &QThread::started, hmiServerManager, &HMIServerManager::init);

    hmiServerThread->start();

    // Inicio
    logFile->println("Cargado");
}

void SCPAManager::applicationStateChanged()
{

}
