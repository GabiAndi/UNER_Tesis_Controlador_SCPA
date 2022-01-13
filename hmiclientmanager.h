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
#include <QThread>

#include "hmiprotocolmanager.h"
#include "hmiusersmanager.h"

class HMIClientManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIClientManager(QTcpSocket *tcpSocket, QObject *parent = nullptr);
        ~HMIClientManager();

        QHostAddress getAddress();

    signals:
        void readData(const QByteArray data);

        void clientDisconnected(HMIClientManager *client);

        void userConnected(HMIClientManager *client);

    private:
        // Conexion
        QTcpSocket *tcpSocket = nullptr;

        // Protocolo
        QThread *protocolThread = nullptr;
        HMIProtocolManager *protocolManager = nullptr;

    private slots:
        // Comandos
        void userLogin(const QString user, const QString password);
};

#endif // HMICLIENTMANAGER_H
