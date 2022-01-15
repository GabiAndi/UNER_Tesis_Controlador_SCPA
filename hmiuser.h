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

class HMIUser : public HMIClient
{
        Q_OBJECT

    public:
        explicit HMIUser(QString user, QString password, QObject *parent = nullptr);
        ~HMIUser();

        QString getUser();
        QString getPassword();

    signals:
        void userDisconnected(HMIUser *user);

    protected:
        // Protocolo
        enum Command : uint8_t
        {
            ALIVE = 0xA0,
            FORCE_LOGIN = 0xA2
        };

        // Datos de sesion
        QString user;
        QString password;

    protected slots:
        // Conexion
        void tcpSocketDisconnected() override;

        // Protocolo
        void newPackage(const uint8_t cmd, const QByteArray payload) override;
};

#endif // HMIUSER_H
