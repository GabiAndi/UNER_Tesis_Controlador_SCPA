#include "hmimanager.h"

HMIManager::HMIManager(QObject *parent) : QObject(parent)
{
    // Inicializacion
    hmiServer = new QTcpServer();

    connect(hmiServer, &QTcpServer::newConnection, this, &HMIManager::newConnection);
    connect(hmiServer, &QTcpServer::acceptError, this, &HMIManager::newConnectionError);

    if (hmiServer->listen(QHostAddress::AnyIPv4, 33600))
    {
        qInfo() << "Servidor HMI iniciado";
    }

    else
    {
        qCritical() << "Servidor HMI error";
    }
}

HMIManager::~HMIManager()
{
    delete hmiClient;
    delete hmiServer;
}

void HMIManager::newConnection()
{
    qInfo() << "Nueva conexión entrante";

    hmiClient = hmiServer->nextPendingConnection();

    connect(hmiClient, &QTcpSocket::readyRead, this, &HMIManager::clientReadData);
    connect(hmiClient, &QTcpSocket::disconnected, this, &HMIManager::clientDisconnected);
}

void HMIManager::newConnectionError(const QAbstractSocket::SocketError socketError)
{
    qCritical() << "Error en la conexión entrante: " << socketError;
}

void HMIManager::clientDisconnected()
{
     qInfo() << "Cliente desconectado";

     disconnect(hmiClient, &QTcpSocket::readyRead, this, &HMIManager::clientReadData);
     disconnect(hmiClient, &QTcpSocket::disconnected, this, &HMIManager::clientDisconnected);
}

void HMIManager::clientReadData()
{
    QByteArray data = hmiClient->readAll();

    qInfo() << QString(data);
}
