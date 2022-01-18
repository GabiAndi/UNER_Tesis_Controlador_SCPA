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

void HMIUser::sendLoginForceRequired()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_FORCE_REQUIRED));
}

void HMIUser::sendLoginCorrect()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_CORRECT));
}

void HMIUser::sendDisconnectOtherUserLogin()
{
    hmiProtocol->write(Command::DISCONNECT_CODE, QByteArray().append(DisconnectCode::OTHER_USER_LOGIN));
}

void HMIUser::tcpSocketDisconnected()
{
    emit userDisconnected(this);
}

void HMIUser::newPackage(const uint8_t cmd, const QByteArray payload)
{
    // Se analiza el comando recibido
    switch ((Command)(cmd))
    {
        /*
         * ALIVE
         *
         * Si recibimos un alive lo devolvemos automaticamente
         * sin informar al client manager.
         */
        case Command::KEEP_ALIVE:
        {
            if ((uint8_t)(payload.at(0)) == KeepAliveMode::REPLY)
            {
                hmiProtocol->write(Command::KEEP_ALIVE, QByteArray().append(KeepAliveMode::REQUEST));
            }

            else if ((uint8_t)(payload.at(0)) == KeepAliveMode::REQUEST)
            {

            }

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
            bool connect = ((ForceLogin)(payload.at(0)) == ForceLogin::FORCE_CONNECT);

            emit userForceLogin(this, connect);

            break;
        }

        default:
            break;
    }
}
