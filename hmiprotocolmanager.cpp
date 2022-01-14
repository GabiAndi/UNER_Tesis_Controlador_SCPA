#include "hmiprotocolmanager.h"

HMIProtocolManager::HMIProtocolManager(QObject *parent)
    : QObject{parent}
{
    // Protocolo
    hmiProtocol = new HMIProtocol(this);

    connect(hmiProtocol, &HMIProtocol::readyRead, this, &HMIProtocolManager::newPackage);
    connect(hmiProtocol, &HMIProtocol::readyWrite, this, &HMIProtocolManager::readyWrite);
}

HMIProtocolManager::~HMIProtocolManager()
{
    // Protocolo
    delete hmiProtocol;
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

            hmiProtocol->write(Command::ALIVE, QByteArray().append((uint8_t)(0xFF)));

            break;

        // Comando no valido
        default:
            break;
    }
}
