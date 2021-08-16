#include "hmithread.h"

HMIThread::HMIThread(QObject *parent) : QThread(parent)
{

}

HMIThread::~HMIThread()
{

}

void HMIThread::finishProcess()
{
    emit closeAplication();
}

void HMIThread::run()
{
    // Instancia del gestor del HMI
    hmiManager = new HMIManager();

    connect(this, &HMIThread::closeAplication, hmiManager, &HMIManager::closeAplication);

    // Se inicia el gestor
    hmiManager->start();

    // Bucle
    exec();

    // Se borra la instancia
    disconnect(this, &HMIThread::closeAplication, hmiManager, &HMIManager::closeAplication);

    delete hmiManager;

    hmiManager = nullptr;
}
