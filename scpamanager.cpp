#include "scpamanager.h"

SCPAManager::SCPAManager()
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
