#include "hmiprotocolmanager.h"

HMIProtocolManager::HMIProtocolManager(QObject *parent) : QObject(parent)
{

}

HMIProtocolManager::~HMIProtocolManager()
{

}

void HMIProtocolManager::init()
{

}

void HMIProtocolManager::readProtocol(const QByteArray data)
{
    // Analisis del paquete ingresado
    for (int readIndex = 0 ; readIndex < data.length() ; readIndex++)
    {

    }
}
