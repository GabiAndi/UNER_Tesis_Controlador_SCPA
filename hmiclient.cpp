#include "hmiclient.h"

HMIClient::HMIClient(QObject *parent, QTcpSocket *hmiClient, int id) : QObject(parent)
{
    if (hmiClient != nullptr)
    {
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
    QByteArray data = hmiClient->readAll();

    qInfo() << "Cliente " << id << ": " << QString(data);
}
