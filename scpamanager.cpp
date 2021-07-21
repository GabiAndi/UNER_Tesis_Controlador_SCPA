#include "scpamanager.h"

SCPAManager::SCPAManager(QObject *parent) : QObject(parent)
{
    // Instancia los servicios
    hmiThread = new HMIThread(this);    // Servidor de HMI
}

SCPAManager::~SCPAManager()
{

}

void SCPAManager::start()
{
    // Inicio de los servicios
    hmiThread->start(); // Servidor de HMI
}
