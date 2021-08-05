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
        void closing();

    private:
        // TUI
        TUIThread *tuiThread = nullptr;

        // HMI
        HMIThread *hmiThread = nullptr;

        // Comprobación de los hilos
        void closingProgress();

    private slots:
        void tuiThreadFinished();
        void hmiThreadFinished();
};

#endif // SCPAMANAGER_H
