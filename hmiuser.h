/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 15/01/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que describe a un usuario identificado.             */
/*************************************************************/

#ifndef HMIUSER_H
#define HMIUSER_H

#include <QObject>

#include "hmiclient.h"
#include "hmiprotocoldata.h"

using namespace hmiprotocoldata;

class HMIUser : public HMIClient
{
        Q_OBJECT

    public:
        explicit HMIUser(QString userName, QString password, QTcpSocket *tcpSocket,
                         QObject *parent = nullptr);
        ~HMIUser();

        QString getUserName();
        QString getPassword();

    signals:
        void userForceLogin(HMIUser *user, bool connect);

        void userDisconnected(HMIUser *user);

    public slots:
        void sendLoginForceRequired();
        void sendLoginCorrect();

        void sendDisconnectOtherUserLogin();

    private:
        // Datos de sesion
        QString userName;
        QString password;

    private slots:
        // Conexion
        void tcpSocketDisconnected() override;

        // Protocolo
        void newPackage(const uint8_t cmd, const QByteArray payload) override;
};

#endif // HMIUSER_H
