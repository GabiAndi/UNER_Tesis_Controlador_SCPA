/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 21/07/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase encargada de gestionar el hilo correspondiente al   */
/* TUI.                                                      */
/*************************************************************/

#ifndef TUITHREAD_H
#define TUITHREAD_H

#include <QThread>

#include "tuimanager.h"

class TUIThread : public QThread
{
        Q_OBJECT

    public:
        explicit TUIThread(QObject *parent = nullptr);
        ~TUIThread();

        void finishProcess();

    signals:
        void closeAplication();

    protected:
        void run() override;

    private:
        TUIManager *tuiManager = nullptr;
};

#endif // TUITHREAD_H
