#include "hmiservermanager.h"

HMIServerManager::HMIServerManager(QObject *parent)
    : QObject{parent}
{

}

HMIServerManager::~HMIServerManager()
{
    // Se libera la memoria
    delete hmiServer;

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

    connect(hmiServer, &QTcpServer::newConnection, this, &HMIServerManager::clientConnection);
    connect(hmiServer, &QTcpServer::acceptError, this, &HMIServerManager::clientConnectionError);

    // Detecta si se pudo abrir el servidor
    // Para un futuro es bueno implementar un control de esto
    if (hmiServer->listen(QHostAddress::SpecialAddress::AnyIPv4, HMI_SERVER_PORT))
    {
        logFile->println("Cargado");
    }

    else
    {
        logFile->println("No se pudo cargar");
    }
}

void HMIServerManager::clientConnection()
{
    // Nuevo cliente
    QTcpSocket *newTcpSocket = hmiServer->nextPendingConnection();

    HMIClientManager *newClient = new HMIClientManager(newTcpSocket, this);

    connect(newClient, &HMIClientManager::clientTimeOut, this, &HMIServerManager::clientTimeOut);
    connect(newClient, &HMIClientManager::clientDisconnected, this, &HMIServerManager::clientDisconnection);

    logFile->println("Cliente conectado desde " + newTcpSocket->localAddress().toString());
}

void HMIServerManager::clientConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error de conexion: %d", socketError));
}

void HMIServerManager::clientTimeOut(HMIClientManager *client)
{
    logFile->println("Cliente no auntenticado desde " + client->getAddress().toString());
}

void HMIServerManager::clientDisconnection(HMIClientManager *client)
{
    logFile->println("Cliente desconectado desde " + client->getAddress().toString());

    client->deleteLater();
}
