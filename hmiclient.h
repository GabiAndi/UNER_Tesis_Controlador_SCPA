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

class HMIClient : public QObject
{
        Q_OBJECT

    public:
        explicit HMIClient(QObject *parent = nullptr);
        ~HMIClient();

        QTcpSocket *getTcpSocket() const;
        void setTcpSocket(QTcpSocket *newTcpSocket);

        void init();

    signals:
        void clientDisconnected(HMIClient *client);
        void clientLogin(HMIClient *client, const QString user, const QString password);

    public slots:
        void tcpSocketDisconnect();

    protected:
        // Conexion
        QTcpSocket *tcpSocket = nullptr;

        // Protocolo
        enum Command : uint8_t
        {
            ALIVE = 0xA0,
            LOGIN = 0xA1
        };

        HMIProtocol *hmiProtocol = nullptr;

    protected slots:
        // Slot de desconexion
        virtual void tcpSocketDisconnected();

        // Slot que analiza los comandos
        virtual void newPackage(const uint8_t cmd, const QByteArray payload);
};

#endif // HMICLIENT_H
