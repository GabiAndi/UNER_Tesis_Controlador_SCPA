/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 11/01/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Gestion de una nueva conexión de usuario.                 */
/*************************************************************/

#ifndef HMICLIENT_H
#define HMICLIENT_H

#include <QObject>

#include <QTcpSocket>

#include "hmiprotocol.h"
#include "hmiprotocoldata.h"

using namespace hmiprotocoldata;

class HMIClient : public QObject
{
        Q_OBJECT

    public:
        explicit HMIClient(QTcpSocket *tcpSocket, QObject *parent = nullptr);
        ~HMIClient();

        QTcpSocket *getTcpSocket() const;

    signals:
        void clientDisconnected(HMIClient *client);
        void clientLogin(HMIClient *client, const QString userName, const QString password);

    public slots:
        void tcpSocketDisconnect();

        void sendAlive();

        void sendLoginError();
        void sendDisconnectTimeOut();

    protected:
        // Conexion
        QTcpSocket *tcpSocket = nullptr;

        // Protocolo
        HMIProtocol *hmiProtocol = nullptr;

    protected slots:
        // Slot de desconexion
        virtual void tcpSocketDisconnected();

        // Slot que analiza los comandos
        virtual void newPackage(const uint8_t cmd, const QByteArray payload);
};

#endif // HMICLIENT_H
