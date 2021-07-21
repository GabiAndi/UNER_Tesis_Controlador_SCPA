#include "hmimanager.h"

HMIManager::HMIManager(QObject *parent) : QObject(parent)
{
    // Inicializacion del servidor
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

    // Inicialización del cliente
    hmiClients = new QList<HMIClient *>();
}

HMIManager::~HMIManager()
{
    delete hmiServer;

    for (int i = 0; i < hmiClients->length() ; i++)
    {
        hmiClients->at(i)->hmiClientDisconect();
    }

    delete hmiClients;
}

void HMIManager::newConnection()
{
    qInfo() << "Nueva conexión entrante";

    bool hmiAceptNewConnection = true;
    int newId = 0;

    for (newId = 0 ; newId < hmiClientsMax ; newId++)
    {
        hmiAceptNewConnection = true;

        for (int i = 0 ; i < hmiClients->length() ; i++)
        {
            if (hmiClients->at(i)->getId() == newId)
            {
                hmiAceptNewConnection = false;

                break;
            }
        }

        if (hmiAceptNewConnection)
        {
            break;
        }
    }

    if (hmiAceptNewConnection)
    {
        qInfo() << "Conexión aceptada";

        HMIClient *newClient = new HMIClient(this, hmiServer->nextPendingConnection(), newId);

        hmiClients->append(newClient);

        connect(newClient, &HMIClient::hmiClientClosed, this, &HMIManager::hmiClientDisconnected);
    }

    else
    {
        qInfo() << "Conexión rechasada";
    }
}

void HMIManager::newConnectionError(const QAbstractSocket::SocketError socketError)
{
    qCritical() << "Error en la conexión entrante: " << socketError;
}

void HMIManager::hmiClientDisconnected(HMIClient *hmiClient)
{
    hmiClients->removeOne(hmiClient);

    disconnect(hmiClient, &HMIClient::hmiClientClosed, this, &HMIManager::hmiClientDisconnected);
}
