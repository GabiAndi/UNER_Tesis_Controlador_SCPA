#include "hmiclientmanager.h"

HMIClientManager::HMIClientManager(QTcpSocket *tcpSocket, QObject *parent)
    : QObject{parent}, tcpSocket{tcpSocket}
{
    // Eventos
    connect(tcpSocket, &QTcpSocket::readyRead, this, &HMIClientManager::readData);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &HMIClientManager::tcpSocketDisconnected);

    // Protocolo
    protocolThread = new QThread(this);
    protocolManager = new HMIProtocolManager();

    protocolManager->moveToThread(protocolThread);

    connect(protocolThread, &QThread::started, protocolManager, &HMIProtocolManager::init);

    connect(this, &HMIClientManager::readProtocol, protocolManager, &HMIProtocolManager::readProtocol);

    protocolThread->start();
}

HMIClientManager::~HMIClientManager()
{
    // Protocolo
    protocolThread->quit();
    protocolThread->wait();

    delete protocolManager;
    delete protocolThread;

    delete tcpSocket;
}

QHostAddress HMIClientManager::getAddress()
{
    return tcpSocket->localAddress();
}

void HMIClientManager::readData()
{
    emit readProtocol(tcpSocket->readAll());
}

void HMIClientManager::tcpSocketDisconnected()
{
    emit clientDisconnected(this);
}

void HMIClientManager::userLogin(const QString user, const QString password)
{
    // Se pudo loguear este cliente
    if (HMIUsersManager::loginUser(user, password))
    {
        return;
    }

    // No se pudo loguear este cliente
    else
    {
        return;
    }
}
