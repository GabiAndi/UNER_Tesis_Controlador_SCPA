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

void HMIServerManager::socketDisconnection(QTcpSocket *tcpSocket)
{

}

void HMIServerManager::clientConnection()
{
    // Nuevo cliente
    QTcpSocket *tcpSocket = hmiServer->nextPendingConnection();

    HMIClient *client = new HMIClient(tcpSocket, this);

    connect(client, &HMIClient::clientDisconnected, this, &HMIServerManager::clientDisconnection);
    connect(client, &HMIClient::clientLogin, this, &HMIServerManager::clientLogin);

    logFile->println("Cliente conectado desde " + client->getTcpSocket()->localAddress().toString());
}

void HMIServerManager::clientConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error de conexion: %d", socketError));
}

void HMIServerManager::clientDisconnection(HMIClient *client)
{
    logFile->println("Cliente desconectado desde " + client->getTcpSocket()->localAddress().toString());

    client->getTcpSocket()->deleteLater();
    client->deleteLater();
}

void HMIServerManager::clientLogin(HMIClient *client, const QString userName, const QString password)
{
    // Debemos verificar si el cliente existe
    if (HMIUsersManager::loginUser(userName, password))
    {
        if (activeUser != nullptr)
        {
            // Hay un usuario que ya se encuentra activo
            HMIUser *newUser = new HMIUser(userName, password, client->getTcpSocket(), this);

            connect(newUser, &HMIUser::userDisconnected, this, &HMIServerManager::userDisconnection);
            connect(newUser, &HMIUser::userForcedConnected, this, &HMIServerManager::userForceConnection);

            client->deleteLater();

            logFile->println("Usuario ya conectado desde " + activeUser->getTcpSocket()->localAddress().toString());
            logFile->println("Usuario ya autenticado como: " + activeUser->getUserName());

            logFile->println("Usuario conectado desde " + newUser->getTcpSocket()->localAddress().toString());
            logFile->println("Usuario autenticado como: " + newUser->getUserName());

            logFile->println("Enviando peticion de conexion a la fuerza");

            newUser->sendLoginBusy();
        }

        else
        {
            // Marcamos el usuario como activo
            activeUser = new HMIUser(userName, password, client->getTcpSocket(), this);

            connect(activeUser, &HMIUser::userDisconnected, this, &HMIServerManager::userDisconnection);

            client->deleteLater();

            logFile->println("Usuario conectado desde " + activeUser->getTcpSocket()->localAddress().toString());
            logFile->println("Usuario autenticado como: " + activeUser->getUserName());

            logFile->println("Usuario conectado correctamente");

            activeUser->sendLoginOk();
        }
    }

    else
    {
        logFile->println("Cliente quizo autenticarse desde: " + client->getTcpSocket()->localAddress().toString());
        logFile->println("Cliente quizo autenticarse como: " + userName);

        logFile->println("Error en credenciales de usuario");

        client->sendLoginError();

        client->tcpSocketDisconnect();
    }
}

void HMIServerManager::userForceConnection(HMIUser *user, const QString userName,
                                           const QString password, bool confirm)
{
    logFile->println("Recibida la peticion de conexion a la fuerza");

    // Debemos verificar si el cliente existe
    if (HMIUsersManager::loginUser(userName, password))
    {
        if (confirm)
        {
            // Marcamos el usuario como activo
            logFile->println("Usuario conectado desde " + user->getTcpSocket()->localAddress().toString());
            logFile->println("Usuario autenticado como: " + user->getUserName());
            logFile->println("Usuario conectado correctamente");

            logFile->println("Usuario cerrado desde: " +
                             activeUser->getTcpSocket()->localAddress().toString());
            logFile->println("Usuario cerrado como: " +
                             activeUser->getUserName());

            activeUser->tcpSocketDisconnect();
            activeUser = user;

            disconnect(activeUser, &HMIUser::userForcedConnected, this, &HMIServerManager::userForceConnection);

            logFile->println("Nuevo usuario conectado");

            activeUser->sendLoginOk();
        }

        else
        {
            // Cerramos la sesion del usuario
            logFile->println("Usuario cerrado desde: " +
                             user->getTcpSocket()->localAddress().toString());
            logFile->println("Usuario cerrado como: " +
                             user->getUserName());

            logFile->println("El nuevo usuario no quizo conectarse");

            user->sendLoginPass();

            user->tcpSocketDisconnect();
        }
    }

    else
    {
        logFile->println("Usuario quizo autenticarse desde: " + user->getTcpSocket()->localAddress().toString());
        logFile->println("Usuario quizo autenticarse como: " + userName);

        logFile->println("Error en credenciales de usuario");

        user->sendLoginError();

        user->tcpSocketDisconnect();
    }
}

void HMIServerManager::userDisconnection(HMIUser *user)
{
    logFile->println("Usuario desconectado desde " + user->getTcpSocket()->localAddress().toString());
    logFile->println("Usuario desconectado como " + user->getUserName());

    logFile->println("Usuario desconectado correctamente");

    if (user == activeUser)
    {
        activeUser->getTcpSocket()->deleteLater();
        activeUser->deleteLater();

        activeUser = nullptr;
    }

    else
    {
        user->getTcpSocket()->deleteLater();
        user->deleteLater();
    }
}
