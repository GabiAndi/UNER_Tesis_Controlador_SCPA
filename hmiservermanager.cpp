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

void HMIServerManager::sendSensorValue(Sensor sensor, float value)
{
    if (activeUser != nullptr)
    {
        activeUser->sendSensorValue(sensor, value);
    }
}

void HMIServerManager::sendSystemState(SystemState state, float value)
{
    if (activeUser != nullptr)
    {
        activeUser->sendSystemState(state, value);
    }
}

void HMIServerManager::setActiveUser(HMIUser *user)
{
    activeUser = user;

    activeUser->stopTimeOut();

    // Sensores
    connect(activeUser, &HMIUser::getSensorValue, this, &HMIServerManager::getSensorValue);
    connect(activeUser, &HMIUser::setSensorValue, this, &HMIServerManager::setSensorValue);

    // Estado del sistema
    connect(activeUser, &HMIUser::getSystemState, this, &HMIServerManager::getSystemState);
    connect(activeUser, &HMIUser::setSystemState, this, &HMIServerManager::setSystemState);

    activeUser->sendLoginCorrect();
}

void HMIServerManager::clientConnection()
{
    // Nuevo cliente
    QTcpSocket *tcpSocket = hmiServer->nextPendingConnection();

    HMIClient *client = new HMIClient(tcpSocket, this);

    connect(client, &HMIClient::clientDisconnected, this, &HMIServerManager::clientDisconnection);
    connect(client, &HMIClient::clientLogin, this, &HMIServerManager::clientLogin);

    logFile->println("Nuevo cliente conectado desde " + client->getTcpSocket()->localAddress().toString());
}

void HMIServerManager::clientConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println("Error de conexion: " + hmiServer->errorString());
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
        // Creamos el usuario
        HMIUser *newUser = new HMIUser(userName, password, client->getTcpSocket(), this);

        connect(newUser, &HMIUser::userDisconnected, this, &HMIServerManager::userDisconnection);

        client->stopTimeOut();
        client->deleteLater();

        // Marcamos el usuario como activo
        if (activeUser == nullptr)
        {
            setActiveUser(newUser);

            logFile->println("El usuario " +
                             activeUser->getUserName() +
                             " se conecto correctamente desde " +
                             activeUser->getTcpSocket()->localAddress().toString());
        }

        // Hay un usuario que ya se encuentra activo
        else
        {
            connect(newUser, &HMIUser::userForceLogin, this, &HMIServerManager::userForceLogin);

            newUser->sendLoginForceRequired();

            logFile->println("El usuario " +
                             newUser->getUserName() +
                             " desde " +
                             newUser->getTcpSocket()->localAddress().toString() +
                             " quiere conectarse, pero " +
                             activeUser->getUserName() +
                             " ya esta activo desde " +
                             activeUser->getTcpSocket()->localAddress().toString());
        }
    }

    else
    {
        client->sendLoginError();

        logFile->println("El cliente quizo autenticarse como " +
                         userName +
                         " desde " +
                         client->getTcpSocket()->localAddress().toString() +
                         ", pero el nombre de usuario o la contrasena no son correctos");
    }
}

void HMIServerManager::userForceLogin(HMIUser *user, bool connecting)
{
    if (connecting)
    {
        // Desconectamos el usuario activo actual
        activeUser->sendDisconnectOtherUserLogin();
        activeUser->tcpSocketDisconnect();

        // Establecemos el nuevo usuario como el activo
        setActiveUser(user);

        disconnect(activeUser, &HMIUser::userForceLogin, this, &HMIServerManager::userForceLogin);

        // Marcamos el usuario como activo
        logFile->println("El usuario " +
                         user->getUserName() +
                         " se conecto desde " +
                         user->getTcpSocket()->localAddress().toString() +
                         ", cerrando la conexión de " +
                         activeUser->getUserName() +
                         " que estaba conectado desde " +
                         activeUser->getTcpSocket()->localAddress().toString());
    }

    else
    {
        // Marcamos el usuario como activo
        logFile->println("El usuario " +
                         user->getUserName() +
                         " desde " +
                         user->getTcpSocket()->localAddress().toString() +
                         " no quizo conectarse y cerrar la sesion de " +
                         activeUser->getUserName() +
                         " que esta conectado desde " +
                         activeUser->getTcpSocket()->localAddress().toString());
    }
}

void HMIServerManager::userDisconnection(HMIUser *user)
{
    logFile->println("Se desconecto el usuario " +
                     user->getUserName() +
                     " desde " +
                     user->getTcpSocket()->localAddress().toString());

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
