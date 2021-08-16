#include "scpamanager.h"

SCPAManager::SCPAManager(QObject *parent) : QObject(parent)
{
    // Log
    logFile = new LogFile(this, "SCPAManager.txt");

    logFile->println("Iniciando aplicacion");

    // TUI
    tuiThread = new TUIThread(this);

    connect(tuiThread, &TUIThread::finished, this, &SCPAManager::tuiThreadFinished);
    connect(tuiThread, &TUIThread::started, this, &SCPAManager::tuiThreadStarted);

    connect(tuiThread, &TUIThread::closeAplication, this, &SCPAManager::closeAplication);

    // HMI
    hmiThread = new HMIThread(this);

    connect(hmiThread, &HMIThread::finished, this, &SCPAManager::hmiThreadFinished);
    connect(hmiThread, &HMIThread::started, this, &SCPAManager::hmiThreadStarted);
}

SCPAManager::~SCPAManager()
{
    logFile->println("Finalizando aplicacion");

    // Se cierra la aplicacion
    exit(0);
}

void SCPAManager::start()
{
    logFile->println("Iniciando procesos");

    // Threads
    tuiThread->start();
    hmiThread->start();
}

void SCPAManager::closeAplication()
{
    logFile->println("Finalizando procesos");

    // Se cierran los procesos
    tuiThread->finishProcess();
    hmiThread->finishProcess();
}

void SCPAManager::tuiThreadFinished()
{
    logFile->println("TUIThread finalizado");

    // Se verifica si se finalizo todo
    finishProgress();
}

void SCPAManager::tuiThreadStarted()
{
    logFile->println("TUIThread iniciado");

    // Se verifica si se inicio todo
    initProgress();
}

void SCPAManager::hmiThreadFinished()
{
    logFile->println("HMIThread finalizado");

    // Se verifica si se finalizo todo
    finishProgress();
}

void SCPAManager::hmiThreadStarted()
{
    logFile->println("HMIThread iniciado");

    // Se verifica si se inicio todo
    initProgress();
}

void SCPAManager::finishProgress()
{
    /*
     * Si todos los hilos del programa se cerraron,
     * se manda a destruir el controlador de aplicación
     * que terminará por finalizar la ejecución del programa
     * principal.
     */
    if (tuiThread->isFinished() && hmiThread->isFinished())
    {
        logFile->println("Todos los procesos se finalizaron");

        deleteLater();
    }
}

void SCPAManager::initProgress()
{
    /*
     * Si todos los hilos del programa se iniciaron.
     */
    if (tuiThread->isRunning() && hmiThread->isRunning())
    {
        logFile->println("Todos los procesos se iniciaron");
    }
}
