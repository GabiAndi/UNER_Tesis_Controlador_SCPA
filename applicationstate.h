/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 05/01/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que maneja el estado de la aplicación.              */
/* Esta clase almacena estados y notifica algún cambio       */
/* en ellos a las clases que estén escuchando.               */
/*************************************************************/

#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include <QObject>

class ApplicationState : public QObject
{
        Q_OBJECT

    public:
        explicit ApplicationState(QObject *parent = nullptr);
        ~ApplicationState();

    public slots:
        void closeApplication();

    signals:
        void stateChanged();

        void closedApplication();

    private:

};

#endif // APPLICATIONSTATE_H
