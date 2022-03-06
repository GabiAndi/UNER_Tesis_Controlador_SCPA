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
#include "controlmanager.h"
#include "metricsmanager.h"

class SCPAManager : public QObject
{
        Q_OBJECT

    public:
        explicit SCPAManager(QObject *parent = nullptr);
        ~SCPAManager();

    public slots:
        void init();

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

        // Controlador de entradas y salidas
        QThread *controlManagerThread = nullptr;
        ControlManager *controlManager = nullptr;

        // Comunicación con el servidor de métricas
        QThread *metricsManagerThread = nullptr;
        MetricsManager *metricsManager = nullptr;
};

#endif // SCPAMANAGER_H
