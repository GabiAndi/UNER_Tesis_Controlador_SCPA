#include "scpamanager.h"

SCPAManager::SCPAManager()
{
    // Instancia los servicios
    hmiManager = new HMIManager(this);
}

SCPAManager::~SCPAManager()
{
    // Elimina la memoria dinamica utilizada
    delete hmiManager;
}

void SCPAManager::start()
{
    // Inicia los servicios
    // Servicio de HMI manager
    connect(hmiManager, &HMIManager::started, this, &SCPAManager::hmiManagerThreadStarted);
    connect(hmiManager, &HMIManager::finished, this, &SCPAManager::hmiManagerThreadFinished);

    hmiManager->start();
}

void SCPAManager::hmiManagerThreadStarted()
{
    qDebug() << "HMI manager started";
}

void SCPAManager::hmiManagerThreadFinished()
{
    qDebug() << "HMI manager finished";
}
