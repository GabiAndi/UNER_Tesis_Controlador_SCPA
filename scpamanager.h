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
#include "hmiservermanager.h"

class SCPAManager : public QObject
{
        Q_OBJECT

    public:
        explicit SCPAManager(QObject *parent = nullptr);
        ~SCPAManager();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Hilos
        // TUI
        QThread *tuiThread = nullptr;
        TUIManager *tuiManager = nullptr;

        // HMI Server
        QThread *hmiServerThread = nullptr;
        HMIServerManager *hmiServerManager = nullptr;
};

#endif // SCPAMANAGER_H
