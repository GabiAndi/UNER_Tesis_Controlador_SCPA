#include "hmiprotocolmanager.h"

HMIProtocolManager::HMIProtocolManager(QObject *parent)
    : QObject{parent}
{

}

HMIProtocolManager::~HMIProtocolManager()
{
    // Paquete
    delete hmiProtocol;
}

void HMIProtocolManager::init()
{
    // Paquete de datos
    hmiProtocol = new HMIProtocol(this);

    connect(hmiProtocol, &HMIProtocol::readyRead, this, &HMIProtocolManager::newPackage);
    connect(hmiProtocol, &HMIProtocol::readyWrite, this, &HMIProtocolManager::readyWrite);
}

void HMIProtocolManager::readData(const QByteArray package)
{
    hmiProtocol->read(package);
}

void HMIProtocolManager::newPackage(const uint8_t cmd, const QByteArray payload)
{
    // Se analiza el comando recibido
    switch (cmd)
    {
        // Alive
        case Command::ALIVE:
            if (payload.length() != 1)
                break;

            if ((uint8_t)(payload.at(0)) != (uint8_t)(0xFF))
                break;

            break;

        // Login
        case Command::LOGIN:
            uint8_t userLength = (uint8_t)(payload.at(0));

            QByteArray user;

            for (uint8_t i = 1 ; i < 1 + userLength ; i++)
            {
                user.append(payload.at(i));
            }

            uint8_t passwordLength = (uint8_t)(payload.at(1 + userLength));

            QByteArray password;

            for (uint8_t i = 1 + userLength + 1 ; i < 1 + userLength + 1 + passwordLength ; i++)
            {
                password.append(payload.at(i));
            }

            emit userLogin(QString(user), QString(password));

            break;
    }
}
