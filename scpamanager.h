/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 07/07/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase encargada de gestionar todos los recursos del       */
/* controlador, desde hilos hasta eventos generados en el    */
/* programa.                                                 */
/*************************************************************/

#ifndef SCPAMANAGER_H
#define SCPAMANAGER_H

#include <QObject>
#include <QFile>

#include "logfile.h"
#include "tuithread.h"
#include "hmithread.h"

class SCPAManager : public QObject
{
        Q_OBJECT

    public:
        explicit SCPAManager(QObject *parent = nullptr);
        ~SCPAManager();

        void start();

    public slots:
        void closeAplication();

    private:
        // Log
        LogFile *logFile = nullptr;

        // Procesos
        TUIThread *tuiThread = nullptr;
        HMIThread *hmiThread = nullptr;

        // Comprobación de los hilos para el cierre
        void finishProgress();

        // Comprobación de los hilos para el inicio
        void initProgress();

    private slots:
        void tuiThreadFinished();
        void tuiThreadStarted();

        void hmiThreadFinished();
        void hmiThreadStarted();
};

#endif // SCPAMANAGER_H
