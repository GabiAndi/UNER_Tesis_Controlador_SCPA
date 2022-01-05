/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 18/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Gestiona los eventos del cliente HMI.                     */
/*************************************************************/

#ifndef HMICLIENT_H
#define HMICLIENT_H

#include <QObject>

#include <QTcpSocket>
#include <QThread>
#include <QByteArray>

#include "hmiprotocolmanager.h"

class HMIClient : public QObject
{
        Q_OBJECT

    public:
        explicit HMIClient(QTcpSocket *tcpSocket, QObject *parent = nullptr);
        ~HMIClient();

        void disconnect();

    private:
        QTcpSocket *tcpSocket = nullptr;

        HMIProtocolManager *hmiProtocolManager = nullptr;
        QThread *hmiProtocolThread = nullptr;

    public slots:
        void hmiClientDisconnected();
        void hmiClientReadData();

    signals:
        void hmiClientClosed(HMIClient *hmiClient);

        void hmiClientReadProtocol(const QByteArray data);
};

#endif // HMICLIENT_H
