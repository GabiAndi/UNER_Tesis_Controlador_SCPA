#include "hmiuser.h"

HMIUser::HMIUser(QString user, QString password, QObject *parent)
    : HMIClient{parent}, user{user}, password{password}
{

}

HMIUser::~HMIUser()
{

}

QString HMIUser::getUser()
{
    return user;
}

QString HMIUser::getPassword()
{
    return password;
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
            if ((uint8_t)(payload.at(0)) != (uint8_t)(0xFF))
                break;

            hmiProtocol->write(Command::ALIVE, QByteArray().append((uint8_t)(0xFF)));

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
            break;
        }
    }
}
