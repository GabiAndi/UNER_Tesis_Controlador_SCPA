#include "hmiservermanager.h"

HMIServerManager::HMIServerManager(QObject *parent)
    : QObject{parent}
{

}

HMIServerManager::~HMIServerManager()
{
    // Se libera la memoria
    delete hmiServer;

    delete activeUser;

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
    QTcpSocket *tcpSocket = hmiServer->nextPendingConnection();

    HMIClient *client = new HMIClient(this);

    client->setTcpSocket(tcpSocket);

    connect(client, &HMIClient::clientDisconnected, this, &HMIServerManager::clientDisconnection);
    connect(client, &HMIClient::clientLogin, this, &HMIServerManager::clientLogin);

    client->init();

    logFile->println("Cliente conectado desde " + client->getTcpSocket()->localAddress().toString());
}

void HMIServerManager::clientConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error de conexion: %d", socketError));
}

void HMIServerManager::clientDisconnection(HMIClient *client)
{
    logFile->println("Cliente desconectado desde " + client->getTcpSocket()->localAddress().toString());

    client->deleteLater();
}

void HMIServerManager::clientLogin(HMIClient *client, const QString user, const QString password)
{
    // Debemos verificar si el cliente existe
    if (HMIUsersManager::loginUser(user, password))
    {
        // Marcamos el usuario como activo
        activeUser = new HMIUser(user, password, this);

        activeUser->setTcpSocket(client->getTcpSocket());

        connect(activeUser, &HMIUser::userDisconnected, this, &HMIServerManager::userDisconnection);

        activeUser->init();

        logFile->println("Usuario conectado desde " + activeUser->getTcpSocket()->localAddress().toString());
        logFile->println("Usuario autenticado como: " + activeUser->getUser());

        /*
         * Le quitamos el acceso al socket del cliente y borramos.
         * Esto es necesario porque el cliente al eliminarse, elimina
         * tambien el socket asociado
         */
        client->setTcpSocket(nullptr);

        client->deleteLater();
    }

    else
    {
        logFile->println("Cliente quizo autenticarse como: " + user);

        client->tcpSocketDisconnect();
    }
}

void HMIServerManager::userDisconnection(HMIUser *user)
{
    logFile->println("Usuario desconectado desde " + user->getTcpSocket()->localAddress().toString());
    logFile->println("Usuario desconectado como " + user->getUser());

    if (user == activeUser)
    {
        activeUser->deleteLater();

        activeUser = nullptr;
    }

    else
    {
        user->deleteLater();
    }
}
