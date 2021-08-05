/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 20/07/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase encargada de gestionar el hilo correspondiente al   */
/* servidor HMI.                                             */
/*************************************************************/

#ifndef HMITHREAD_H
#define HMITHREAD_H

#include <QThread>

#include "hmimanager.h"

class HMIThread : public QThread
{
        Q_OBJECT

    public:
        explicit HMIThread(QObject *parent = nullptr);
        ~HMIThread();

    protected:
        void run() override;
};

#endif // HMITHREAD_H
