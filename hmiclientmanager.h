/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 11/01/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Gestion de una nueva conexión de usuario.                 */
/*************************************************************/

#ifndef HMICLIENTMANAGER_H
#define HMICLIENTMANAGER_H

#include <QObject>

#include <QTcpSocket>
#include <QTimer>

#include "hmiprotocolmanager.h"
#include "hmiusersmanager.h"

#define CLIENT_LOGIN_TIMEOUT        1000

class HMIClientManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIClientManager(QTcpSocket *tcpSocket, QObject *parent = nullptr);
        ~HMIClientManager();

        QHostAddress getAddress();

    signals:
        void clientDisconnected(HMIClientManager *client);
        void clientTimeOut(HMIClientManager *client);

    public slots:
        void clientDisconnect();

    private:
        // Conexion
        QTcpSocket *tcpSocket = nullptr;

        // Protocolo
        HMIProtocolManager *protocolManager = nullptr;

        // Timer de desconexion
        QTimer *timerTimeOut = nullptr;
};

#endif // HMICLIENTMANAGER_H
