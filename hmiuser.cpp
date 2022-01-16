#include "hmiuser.h"

HMIUser::HMIUser(QString userName, QString password, QTcpSocket *tcpSocket, QObject *parent)
    : HMIClient{tcpSocket, parent}, userName{userName}, password{password}
{

}

HMIUser::~HMIUser()
{

}

QString HMIUser::getUserName()
{
    return userName;
}

QString HMIUser::getPassword()
{
    return password;
}

void HMIUser::sendLoginOk()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_OK));
}

void HMIUser::sendLoginBusy()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_BUSY));
}

void HMIUser::sendLoginPass()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_PASS));
}

void HMIUser::tcpSocketDisconnected()
{
    emit userDisconnected(this);
}

void HMIUser::newPackage(const uint8_t cmd, const QByteArray payload)
{
    // Se analiza el comando recibido
    switch (cmd)
    {
        /*
         * ALIVE
         *
         * Si recibimos un alive lo devolvemos automaticamente
         * sin informar al client manager.
         */
        case Command::ALIVE:
        {
            if ((uint8_t)(payload.at(0)) != Payload::PAYLOAD_OK)
                break;

            hmiProtocol->write(Command::ALIVE, QByteArray().append(Payload::PAYLOAD_OK));

            break;
        }

        /*
         * LOGIN
         *
         * Al recibir un LOGIN extraemos el usuario y contraseña y lo enviamos
         * a las capas superiores.
         */
        case Command::FORCE_LOGIN:
        {
            bool confirm = payload.at(0);

            uint8_t userLength = payload.at(1);

            QString userName(payload.mid(2, userLength));

            uint8_t passwordLength = payload.at(2 + userLength);

            QString password(payload.mid(2 + userLength + 1, passwordLength));

            emit userForcedConnected(this, userName, password, confirm);

            break;
        }
    }
}
