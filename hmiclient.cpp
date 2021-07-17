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

        connect(this->hmiClient, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
        connect(this->hmiClient, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);

        qInfo() << "Cliente creado";
        qInfo() << "ID: " << id;

        // Protocolo de comunicación
        scpaProtocol = new SCPAProtocol(this);
    }
}

HMIClient::~HMIClient()
{
    disconnect(this->hmiClient, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
    disconnect(this->hmiClient, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);

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

void HMIClient::hmiClientDisconnected()
{
    deleteLater();

    emit closed(this);

    qInfo() << "Conexión finalizada";
    qInfo() << "ID: " << id;
}

void HMIClient::hmiClientReadData()
{
    // Lectura de los datos pendientes
    scpaProtocol->readProtocol(hmiClient->readAll());
}
