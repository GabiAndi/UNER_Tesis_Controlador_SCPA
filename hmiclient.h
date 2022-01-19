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
#include <QTimer>

#include "hmiprotocol.h"
#include "hmiprotocoldata.h"

#define MAX_LOGIN_TIMEOUT           10000

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
        // TimeOut
        void stopTimeOut();

        // Eventos socket
        void tcpSocketDisconnect();

        // Comandos
        void sendAlive();

        void sendLoginError();
        void sendDisconnectTimeOut();

    protected:
        // Conexion
        QTcpSocket *tcpSocket = nullptr;

        // Protocolo
        HMIProtocol *hmiProtocol = nullptr;

        // Timer de timeout
        QTimer *timerTimeOut = nullptr;

    protected slots:
        // Slot de desconexion
        virtual void tcpSocketDisconnected();

        // Slot que analiza los comandos
        virtual void newPackage(const uint8_t cmd, const QByteArray payload);
};

#endif // HMICLIENT_H
