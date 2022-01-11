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

    if (hmiServer->listen(QHostAddress::AnyIPv4, HMI_SERVER_PORT))
    {
        logFile->println("Cargado");
    }

    else
    {
        logFile->println("No se pudo cargar");
    }
}

void HMIServerManager::getHmiServerStatus()
{
    /*hmi_server_status_t status;

    status.serverIP = hmiServer->serverAddress().toString();
    status.port = QString::asprintf("%d", hmiServer->serverPort());

    if (clientTcpSocket != nullptr)
        status.clientIP = clientTcpSocket->localAddress().toString();

    else
        status.clientIP = "No conectado";

    emit hmiServerStatus(status);*/
}

void HMIServerManager::clientConnection()
{
    // Nuevo cliente
    QTcpSocket *newTcpSocket = hmiServer->nextPendingConnection();

    HMIClientManager *newClient = new HMIClientManager(newTcpSocket, this);

    connect(newClient, &HMIClientManager::clientDisconnected, this, &HMIServerManager::clientDisconnection);

    connect(newClient, &HMIClientManager::userConnected, this, &HMIServerManager::userConnection);

    logFile->println("Nuevo cliente conectado desde " + newTcpSocket->localAddress().toString());
}

void HMIServerManager::clientConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error de conexion: %d", socketError));
}

void HMIServerManager::clientDisconnection(HMIClientManager *client)
{
    logFile->println("No se autentico el cliente " + client->getAddress().toString());

    client->deleteLater();
}

void HMIServerManager::userConnection(HMIClientManager *client)
{
    disconnect(client, &HMIClientManager::clientDisconnected, this, &HMIServerManager::clientDisconnection);

    connect(client, &HMIClientManager::clientDisconnected, this, &HMIServerManager::userDisconnection);
}

void HMIServerManager::userDisconnection(HMIClientManager *client)
{
    client->deleteLater();
}
