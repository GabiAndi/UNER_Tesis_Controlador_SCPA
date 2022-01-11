#include "hmiservermanager.h"

HMIServerManager::HMIServerManager(QObject *parent)
    : QObject{parent}
{

}

HMIServerManager::~HMIServerManager()
{
    // Se finaliza el hilo del analisis de paquete
    if ((clientProtocolThread != nullptr) && (clientProtocolManager != nullptr))
    {
        clientProtocolThread->quit();
        clientProtocolThread->wait();
    }

    delete clientProtocolManager;
    delete clientProtocolThread;

    // Se libera la memoria
    delete clientTcpSocket;
    delete hmiServer;

    delete hmiUsersManager;

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

    if (hmiServer->listen(QHostAddress::AnyIPv4, HMI_SERVER_PORT))
    {
        logFile->println("Cargado");
    }

    else
    {
        logFile->println("No se pudo cargar");
    }

    // Cargando usuarios
    logFile->println("Cargando usuarios");

    hmiUsersManager = new HMIUsersManager(this);

    logFile->println(QString::asprintf("%d usuarios cargados", hmiUsersManager->getNumberUsers()));
}

void HMIServerManager::getHmiServerStatus()
{
    hmi_server_status_t status;

    status.serverIP = hmiServer->serverAddress().toString();
    status.port = QString::asprintf("%d", hmiServer->serverPort());

    if (clientTcpSocket != nullptr)
        status.clientIP = clientTcpSocket->localAddress().toString();

    else
        status.clientIP = "No conectado";

    emit hmiServerStatus(status);
}

void HMIServerManager::newConnection()
{
    // Conexión
    clientTcpSocket = hmiServer->nextPendingConnection();

    connect(clientTcpSocket, &QTcpSocket::readyRead, this, &HMIServerManager::readData);
    connect(clientTcpSocket, &QTcpSocket::disconnected, this, &HMIServerManager::clientDisconnected);

    // Protocolo
    clientProtocolThread = new QThread(this);
    clientProtocolManager = new HMIProtocolManager();

    clientProtocolManager->moveToThread(clientProtocolThread);

    connect(clientProtocolThread, &QThread::started, clientProtocolManager, &HMIProtocolManager::init);

    connect(this, &HMIServerManager::readProtocol, clientProtocolManager, &HMIProtocolManager::readProtocol);

    clientProtocolThread->start();

    logFile->println("Nuevo cliente conectado desde " + clientTcpSocket->localAddress().toString());
}

void HMIServerManager::newConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error de conexion: %d", socketError));
}

void HMIServerManager::clientDisconnected()
{
    // Se cierra la conexión del cliente
    logFile->println("Cliente desconectado desde " + clientTcpSocket->localAddress().toString());

    // Se finaliza el hilo del analisis de paquete
    clientProtocolThread->quit();
    clientProtocolThread->wait();

    // Se elimina la memoria
    clientProtocolManager->deleteLater();
    clientProtocolThread->deleteLater();

    clientTcpSocket->deleteLater();

    clientProtocolManager = nullptr;
    clientProtocolThread = nullptr;

    clientTcpSocket = nullptr;
}

void HMIServerManager::readData()
{
    // Interpretacion de los comandos enviados por los clientes
    emit readProtocol(clientTcpSocket->readAll());
}
