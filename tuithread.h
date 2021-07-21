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
#include <QDebug>
#include <QTextStream>

class TUIThread : public QThread
{
        Q_OBJECT

    public:
        explicit TUIThread(QObject *parent = nullptr);
        ~TUIThread();

    protected:
        void run() override;
};

#endif // TUITHREAD_H
