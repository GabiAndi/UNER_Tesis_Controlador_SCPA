#include "scpamanager.h"

SCPAManager::SCPAManager()
{
    // Instancia los servicios
    hmiThread = new HMIThread();    // Servidor de HMI
}

SCPAManager::~SCPAManager()
{
    // Se elimina
    delete hmiThread;
}

void SCPAManager::start()
{
    // Inicio de los servicios
    hmiThread->start(); // Servidor de HMI
}
