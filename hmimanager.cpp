#include "hmimanager.h"

HMIManager::HMIManager(QObject *parent) : QObject(parent)
{
    // Log
    logFile = new LogFile(this, "HMIManager.txt");

    logFile->println("Iniciando");

    // Inicializacion del servidor
    hmiServer = new QTcpServer(this);

    // Inicialización de la lista de clientes
    hmiClients = new QList<HMIClient *>();
}

HMIManager::~HMIManager()
{
    logFile->println("Saliendo");

    delete hmiClients;
}

void HMIManager::start()
{
    // Conectamos las señales del servidor tcp
    connect(hmiServer, &QTcpServer::newConnection, this, &HMIManager::newConnection);
    connect(hmiServer, &QTcpServer::acceptError, this, &HMIManager::newConnectionError);

    if (hmiServer->listen(QHostAddress::AnyIPv4, 33600))
    {
        logFile->println("Servidor HMI iniciado");
    }

    else
    {
        logFile->println("Error al iniciar el servidor HMI");
    }
}

int HMIManager::getID()
{
    bool hmiAceptNewConnection = true;

    for (int newId = 0 ; newId < hmiClientsMax ; newId++)
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
            return newId;
        }
    }

    return -1;
}

void HMIManager::newConnection()
{
    logFile->println("Nueva conexión entrante");

    int newId = getID();

    if (newId != -1)
    {
        logFile->println("Conexión aceptada");

        HMIClient *newClient = new HMIClient(this, hmiServer->nextPendingConnection(), newId);

        hmiClients->append(newClient);

        connect(newClient, &HMIClient::hmiClientClosed, this, &HMIManager::hmiClientDisconnected);
    }

    else
    {
        logFile->println("Conexión rechasada");
    }
}

void HMIManager::newConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println("Error en la conexión entrante: " + QString(socketError));
}

void HMIManager::hmiClientDisconnected(HMIClient *hmiClient)
{
    hmiClients->removeOne(hmiClient);

    disconnect(hmiClient, &HMIClient::hmiClientClosed, this, &HMIManager::hmiClientDisconnected);
}
