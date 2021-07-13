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
