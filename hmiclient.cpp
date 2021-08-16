#include "hmiclient.h"

HMIClient::HMIClient(QObject *parent, QTcpSocket *hmiClient, int id) : QObject(parent)
{
    // Socket e ID
    this->hmiClient = hmiClient;
    this->id = id;

    // Comunicacion
    connect(this->hmiClient, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
    connect(this->hmiClient, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);

    // Protocolo
    scpaProtocol = new SCPAProtocol(this);

    // Hilos
    connect(scpaProtocol, &SCPAProtocol::started, this, &HMIClient::scpaProtocolThreadStart);
    connect(scpaProtocol, &SCPAProtocol::finished, this, &HMIClient::scpaProtocolThreadStop);
}

HMIClient::~HMIClient()
{

}

void HMIClient::disconect()
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
    // Si se esta analizando datos, se manda a deterner
    if (scpaProtocol->isRunning())
    {
        scpaProtocol->setExitPending();
    }

    else
    {
        closeThisClient();
    }
}

void HMIClient::hmiClientReadData()
{
    // Lectura de los datos pendientes
    scpaProtocol->readProtocol(hmiClient->readAll());
}

void HMIClient::scpaProtocolThreadStart()
{

}

void HMIClient::scpaProtocolThreadStop()
{
    if (scpaProtocol->getExitPending())
    {
        closeThisClient();
    }
}
