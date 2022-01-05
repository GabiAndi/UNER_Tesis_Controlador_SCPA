/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 17/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que permite leer datos de la consola.               */
/*************************************************************/

#ifndef CONSOLELISTENER_H
#define CONSOLELISTENER_H

#include <QObject>

#include <QSocketNotifier>

#include <iostream>

class ConsoleListener : public QObject
{
        Q_OBJECT

    public:
        explicit ConsoleListener(QObject *parent = nullptr);
        ~ConsoleListener();

    public slots:
        void init();

    signals:
        void newLine(const QString line);

    private:
        QSocketNotifier *socketNotifier = nullptr;

    private slots:
        void getLine();
};

#endif // CONSOLELISTENER_H
