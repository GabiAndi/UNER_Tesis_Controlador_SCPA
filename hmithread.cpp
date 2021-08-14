#include "hmithread.h"

HMIThread::HMIThread(QObject *parent) : QThread(parent)
{

}

HMIThread::~HMIThread()
{

}

void HMIThread::run()
{
    // Instancia del gestor del HMI
    HMIManager *hmiManager = new HMIManager();

    // Se inicia el gestor
    hmiManager->start();

    // Bucle
    exec();

    // Se borra la instancia
    delete hmiManager;
}
