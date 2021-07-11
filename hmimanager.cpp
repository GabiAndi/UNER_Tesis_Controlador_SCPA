#include "hmimanager.h"

HMIManager::HMIManager(QObject *parent) : QThread(parent)
{

}

HMIManager::~HMIManager()
{
    // Se elimina el cliente
    delete hmiClient;
}

void HMIManager::run()
{
    // Inicializacion
    hmiServer = new QTcpServer();

    connect(hmiServer, &QTcpServer::newConnection, this, &HMIManager::newConnection);

    hmiServer->listen(QHostAddress::AnyIPv4, 33600);

    while (true)
    {
        if (hmiServer->hasPendingConnections())
        {
            hmiClient = hmiServer->nextPendingConnection();

            hmiClient->open(QIODevice::ReadWrite);
        }
    }

    // Se eliminar el servidor TCP
    delete hmiServer;
}

void HMIManager::newConnection()
{
    qDebug() << "Conexión nueva en espera.";
}
