#include "scpamanager.h"

SCPAManager::SCPAManager(QObject *parent) : QObject(parent)
{

}

SCPAManager::~SCPAManager()
{
    // Se cierra la aplicacion
    exit(0);
}

void SCPAManager::start()
{
    // TUI
    tuiThread = new TUIThread(this);

    connect(tuiThread, &TUIThread::closing, this, &SCPAManager::closing);
    connect(tuiThread, &TUIThread::finished, this, &SCPAManager::tuiThreadFinished);

    tuiThread->start();

    // HMI
    hmiThread = new HMIThread(this);

    connect(hmiThread, &HMIThread::finished, this, &SCPAManager::hmiThreadFinished);

    hmiThread->start();
}

void SCPAManager::closing()
{
    // Se cierran los procesos
    tuiThread->exit(0);
    hmiThread->exit(0);
}

void SCPAManager::tuiThreadFinished()
{
    // Se verifica si se finalizo todo
    closingProgress();
}

void SCPAManager::hmiThreadFinished()
{
    // Se verifica si se finalizo todo
    closingProgress();
}

void SCPAManager::closingProgress()
{
    /*
     * Si todos los hilos del programa se cerraron,
     * se manda a cerrar el tui para finalizar el programa
     */
    if (tuiThread->isFinished() && hmiThread->isFinished())
    {
        // Cuando todos los hilos se cierren se borra el objeto
        deleteLater();
    }
}
