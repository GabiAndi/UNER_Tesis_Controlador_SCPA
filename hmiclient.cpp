#include "hmiclient.h"

HMIClient::HMIClient(QObject *parent, QTcpSocket *hmiClient, int id) : QObject(parent)
{
    // Socket e ID
    this->hmiClient = hmiClient;
    this->id = id;

    // Comunicacion
    connect(this->hmiClient, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
    connect(this->hmiClient, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);

    // Protocolo de comunicion
    scpaProtocol = new SCPAProtocol(this);
}

HMIClient::~HMIClient()
{

}

void HMIClient::disconnectClient()
{
    hmiClient->disconnectFromHost();
}

int HMIClient::getId()
{
    return id;
}

void HMIClient::closeThisClient()
{
    emit hmiClientClosed(this);

    deleteLater();
}

void HMIClient::hmiClientDisconnected()
{
    if (scpaProtocol->isRunning())
    {
        connect(scpaProtocol, &SCPAProtocol::finished, this, &HMIClient::scpaProtocolFinished);

        scpaProtocol->abortReadProtocol();
    }

    else
    {
        closeThisClient();
    }
}

void HMIClient::hmiClientReadData()
{
    scpaProtocol->readProtocol(hmiClient->readAll());
}

void HMIClient::scpaProtocolFinished()
{
    disconnect(scpaProtocol, &SCPAProtocol::finished, this, &HMIClient::scpaProtocolFinished);

    closeThisClient();
}
