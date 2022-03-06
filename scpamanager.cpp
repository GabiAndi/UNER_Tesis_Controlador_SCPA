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

    // Comunicacion con el servidor de metricas
    metricsManagerThread->quit();
    metricsManagerThread->wait();

    delete metricsManager;
    delete metricsManagerThread;

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

    // Comunicacion con el servidor de metricas
    metricsManagerThread = new QThread(this);
    metricsManager = new MetricsManager();

    metricsManager->moveToThread(metricsManagerThread);

    metricsManager->setObjectName("MetricsManager");

    connect(metricsManagerThread, &QThread::started, metricsManager, &MetricsManager::init);

    metricsManagerThread->start();

    // Controlador de entradas y salidas
    controlManagerThread = new QThread(this);
    controlManager = new ControlManager();

    controlManager->moveToThread(controlManagerThread);

    controlManager->setObjectName("ControlManager");

    connect(controlManagerThread, &QThread::started, controlManager, &ControlManager::init);

    connect(controlManager, &ControlManager::sendSensorValue, hmiServerManager, &HMIServerManager::sendSensorValue);
    connect(controlManager, &ControlManager::sendSystemState, hmiServerManager, &HMIServerManager::sendSystemState);

    connect(hmiServerManager, &HMIServerManager::getSensorValue, controlManager, &ControlManager::getSensorValue);
    connect(hmiServerManager, &HMIServerManager::setSensorValue, controlManager, &ControlManager::setSensorValue);

    connect(hmiServerManager, &HMIServerManager::getSystemState, controlManager, &ControlManager::getSystemState);
    connect(hmiServerManager, &HMIServerManager::setSystemState, controlManager, &ControlManager::setSystemState);

    connect(metricsManager, &MetricsManager::getMetricSensorValue, controlManager, &ControlManager::getMetricSensorValue);
    connect(metricsManager, &MetricsManager::getMetricSystemState, controlManager, &ControlManager::getMetricSystemState);

    connect(controlManager, &ControlManager::sendMetricSensorValue, metricsManager, &MetricsManager::sendMetricSensorValue);
    connect(controlManager, &ControlManager::sendMetricSystemState, metricsManager, &MetricsManager::sendMetricSystemState);

    controlManagerThread->start();

    // Inicio
    logFile->println("Cargado");
}
