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

    // Controlador de entradas y salidas
    controlManagerThread->quit();
    controlManagerThread->wait();

    delete controlManager;
    delete controlManagerThread;

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

    // Controlador de entradas y salidas
    controlManagerThread = new QThread(this);
    controlManager = new ControlManager();

    controlManager->moveToThread(controlManagerThread);

    controlManager->setObjectName("ControlManager");

    connect(controlManagerThread, &QThread::started, controlManager, &ControlManager::init);

    connect(hmiServerManager, &HMIServerManager::getParameterValue, controlManager, &ControlManager::getParameterValue);

    connect(controlManager, &ControlManager::sendParameterValue, hmiServerManager, &HMIServerManager::sendParameterValue);

    connect(hmiServerManager, &HMIServerManager::setSimulationLvFoso, controlManager, &ControlManager::setLvFoso);
    connect(hmiServerManager, &HMIServerManager::setSimulationLvLodo, controlManager, &ControlManager::setLvLodo);
    connect(hmiServerManager, &HMIServerManager::setSimulationTemp, controlManager, &ControlManager::setTemp);
    connect(hmiServerManager, &HMIServerManager::setSimulationOD, controlManager, &ControlManager::setOD);
    connect(hmiServerManager, &HMIServerManager::setSimulationPhAnox, controlManager, &ControlManager::setPhAnox);
    connect(hmiServerManager, &HMIServerManager::setSimulationPhAireacion, controlManager, &ControlManager::setPhAireacion);

    controlManagerThread->start();

    // Inicio
    logFile->println("Cargado");
}
