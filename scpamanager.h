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

    private:
        TUIThread *tuiThread = nullptr;
        HMIThread *hmiThread = nullptr;
};

#endif // SCPAMANAGER_H
