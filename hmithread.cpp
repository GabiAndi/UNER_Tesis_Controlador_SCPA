#include "hmithread.h"

HMIThread::HMIThread(QObject *parent) : QThread(parent)
{

}

HMIThread::~HMIThread()
{

}

void HMIThread::finishProcess()
{
    //hmiManager->closeAplication();
    exit(0);
}

void HMIThread::run()
{
    // Instancia del gestor del HMI
    hmiManager = new HMIManager();

    // Se inicia el gestor
    hmiManager->start();

    // Bucle
    exec();

    // Se borra la instancia
    delete hmiManager;

    hmiManager = nullptr;
}
