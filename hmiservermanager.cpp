#include "hmiservermanager.h"

HMIServerManager::HMIServerManager(QObject *parent) : QObject(parent)
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->create("HMIServerManager");

    logFile->println("Controlador de servidor iniciado");
}

HMIServerManager::~HMIServerManager()
{
    // Se eliminan los clientes que estan conectados
    for (int i = 0 ; i < hmiClients->length() ; i++)
    {
        delete hmiClients->at(i);
    }

    delete hmiClients;

    // Cierre del archivo de log
    logFile->println("Controlador de servidor finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void HMIServerManager::init()
{
    // Se inicia el servidor
    hmiServer = new QTcpServer(this);

    connect(hmiServer, &QTcpServer::newConnection, this, &HMIServerManager::newConnection);
    connect(hmiServer, &QTcpServer::acceptError, this, &HMIServerManager::newConnectionError);

    if (hmiServer->listen(QHostAddress::AnyIPv4, hmiServerPort))
    {
        logFile->println("Controlador de servidor cargado");
    }

    else
    {
        logFile->println("Controlador de servidor no se pudo cargar");
    }

    // Inicialización de la lista de clientes
    hmiClients = new QList<HMIClient *>();
}

void HMIServerManager::newConnection()
{
    HMIClient *newClient = new HMIClient(hmiServer->nextPendingConnection(), this);

    hmiClients->append(newClient);

    connect(newClient, &HMIClient::hmiClientClosed, this, &HMIServerManager::hmiClientDisconnected);
}

void HMIServerManager::newConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error de conexion: %d", socketError));
}

void HMIServerManager::hmiClientDisconnected(HMIClient *hmiClient)
{
    // Se cierra la conexión del cliente eliminandolo de la lista de clientes activos
    hmiClients->removeOne(hmiClient);

    disconnect(hmiClient, &HMIClient::hmiClientClosed, this, &HMIServerManager::hmiClientDisconnected);

    hmiClient->deleteLater();
}
