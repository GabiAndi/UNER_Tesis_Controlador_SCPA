#include "scpamanager.h"

SCPAManager::SCPAManager(QObject *parent) : QObject(parent)
{
    // Instancia los servicios
    tuiThread = new TUIThread(this);    // Interfaz de usuario
    hmiThread = new HMIThread(this);    // Servidor de HMI
}

SCPAManager::~SCPAManager()
{

}

void SCPAManager::start()
{
    // Inicio de los servicios
    tuiThread->start(); // Interfaz de usuario
    hmiThread->start(); // Servidor de HMI
}
