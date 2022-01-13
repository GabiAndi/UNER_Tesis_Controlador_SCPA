#include "hmiclientmanager.h"

HMIClientManager::HMIClientManager(QTcpSocket *tcpSocket, QObject *parent)
    : QObject{parent}, tcpSocket{tcpSocket}
{
    // Eventos
    connect(tcpSocket, &QTcpSocket::readyRead, this, [this]()
    {
        QByteArray data = this->tcpSocket->readAll();

        emit readData(data);
    });
    connect(tcpSocket, &QTcpSocket::disconnected, this, [this]()
    {
        emit clientDisconnected(this);
    });

    // Protocolo
    protocolThread = new QThread(this);
    protocolManager = new HMIProtocolManager();

    protocolManager->moveToThread(protocolThread);

    connect(protocolThread, &QThread::started, protocolManager, &HMIProtocolManager::init);

    connect(protocolManager, &HMIProtocolManager::readyWrite, this, [this](const QByteArray package)
    {
        this->tcpSocket->write(package);
    });

    connect(this, &HMIClientManager::readData, protocolManager, &HMIProtocolManager::readData);

    connect(protocolManager, &HMIProtocolManager::userLogin, this, &HMIClientManager::userLogin);

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

void HMIClientManager::userLogin(const QString user, const QString password)
{
    // Se pudo loguear este cliente
    if (HMIUsersManager::loginUser(user, password))
    {
        //emit userConnected(this);
    }

    // No se pudo loguear este cliente
    else
    {

    }
}
