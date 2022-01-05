#include "hmiservermanager.h"

HMIServerManager::HMIServerManager(QObject *parent)
    : QObject{parent}
{

}

HMIServerManager::~HMIServerManager()
{
    // Se elimina el cliente que esta conectado

    delete hmiClient;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void HMIServerManager::init()
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Se inicia el servidor
    hmiServer = new QTcpServer(this);

    connect(hmiServer, &QTcpServer::newConnection, this, &HMIServerManager::newConnection);
    connect(hmiServer, &QTcpServer::acceptError, this, &HMIServerManager::newConnectionError);

    if (hmiServer->listen(QHostAddress::AnyIPv4, hmiServerPort))
    {
        logFile->println("Cargado");
    }

    else
    {
        logFile->println("No se pudo cargar");
    }
}

void HMIServerManager::newConnection()
{
    //HMIClient *newClient = new HMIClient(hmiServer->nextPendingConnection(), this);

    //connect(newClient, &HMIClient::hmiClientClosed, this, &HMIServerManager::hmiClientDisconnected);
}

void HMIServerManager::newConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error de conexion: %d", socketError));
}

void HMIServerManager::hmiClientDisconnected(HMIClient *hmiClient)
{
    // Se cierra la conexión del cliente
    /*hmiClients->removeOne(hmiClient);

    disconnect(hmiClient, &HMIClient::hmiClientClosed, this, &HMIServerManager::hmiClientDisconnected);

    hmiClient->deleteLater();*/
}
