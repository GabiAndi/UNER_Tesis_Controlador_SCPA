/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 17/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Esta clase es la controladora principal de la aplicacion. */
/*************************************************************/

#ifndef SCPAMANAGER_H
#define SCPAMANAGER_H

#include <QObject>
#include <QThread>

#include "logfile.h"
#include "tuimanager.h"

class SCPAManager : public QObject
{
        Q_OBJECT

    public:
        explicit SCPAManager(QObject *parent = nullptr);
        ~SCPAManager();

        void start();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Hilos
        /*
         * Tiempo de espera maximo en ms de espera antes de cerrar
         * un hilo por la fuerza.
         */
        const int threadExitTimeOut = 2000;

        // TUI
        TUIManager *tuiManager = nullptr;
        QThread *tuiThread = nullptr;
};

#endif // SCPAMANAGER_H
