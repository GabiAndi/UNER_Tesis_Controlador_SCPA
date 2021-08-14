/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 14/07/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase de cliente HMI que se conecta al servidor para      */
/* enviar y recibir comandos.                                */
/*************************************************************/

#ifndef HMICLIENT_H
#define HMICLIENT_H

#include <QObject>
#include <QTcpSocket>

#include "scpaprotocol.h"

class HMIClient : public QObject
{
        Q_OBJECT

    public:
        explicit HMIClient(QObject *parent = nullptr, QTcpSocket *hmiClient = nullptr, int id = -1);
        ~HMIClient();

        void disconect();

        int getId();

    private:
        QTcpSocket *hmiClient = nullptr;

        int id = -1;    // ID asignado al cliente

        SCPAProtocol *scpaProtocol = nullptr;

        bool closing = false;
        void closeThisClient();

    private slots:
        void hmiClientDisconnected();
        void hmiClientReadData();

        void scpaProtocolThreadStart();
        void scpaProtocolThreadStop();

    signals:
        void hmiClientClosed(HMIClient *thisClient);
};

#endif // HMICLIENT_H
