#include "hmiclient.h"

HMIClient::HMIClient(QObject *parent, QTcpSocket *hmiClient, int id) : QObject(parent)
{
    // Inicialización del cliente
    if (hmiClient != nullptr)
    {
        // Socket e ID
        this->hmiClient = hmiClient;
        this->id = id;

        if (id < 0)
        {
            qCritical() << "Cliente asignado con id invalido";
        }

        // Comunicacion
        connect(this->hmiClient, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
        connect(this->hmiClient, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);

        qInfo() << "Cliente creado";
        qInfo() << "ID: " << id;

        // Protocolo
        scpaProtocol = new SCPAProtocol();

        // Hilos
        connect(scpaProtocol, &SCPAProtocol::started, this, &HMIClient::scpaProtocolThreadStart);
        connect(scpaProtocol, &SCPAProtocol::finished, this, &HMIClient::scpaProtocolThreadStop);
    }
}

HMIClient::~HMIClient()
{
    // Comunicacion
    disconnect(this->hmiClient, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
    disconnect(this->hmiClient, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);

    // Hilos
    disconnect(scpaProtocol, &SCPAProtocol::started, this, &HMIClient::scpaProtocolThreadStart);
    disconnect(scpaProtocol, &SCPAProtocol::finished, this, &HMIClient::scpaProtocolThreadStop);

    // Protocolo
    delete scpaProtocol;

    qInfo() << "Cliente eliminado";
}

void HMIClient::hmiClientDisconect()
{
    hmiClient->disconnectFromHost();
}

int HMIClient::getId()
{
    return id;
}

void HMIClient::closeThisClient()
{
    deleteLater();

    emit hmiClientClosed(this);

    qInfo() << "Conexión finalizada";
    qInfo() << "ID: " << id;
}

void HMIClient::hmiClientDisconnected()
{
    if (scpaProtocol->isRunning())
    {
        closing = true;
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
    if (closing)
    {
        closeThisClient();
    }
}
