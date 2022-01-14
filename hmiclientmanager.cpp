#include "hmiclientmanager.h"

HMIClientManager::HMIClientManager(QTcpSocket *tcpSocket, QObject *parent)
    : QObject{parent}, tcpSocket{tcpSocket}
{
    // Conexion
    tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    connect(tcpSocket, &QTcpSocket::disconnected, this, [this]()
    {
        emit clientDisconnected(this);
    });

    // Protocolo
    protocolManager = new HMIProtocolManager(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, [this]()
    {
        protocolManager->readData(this->tcpSocket->readAll());
    });

    connect(protocolManager, &HMIProtocolManager::readyWrite, this, [this](const QByteArray package)
    {
        this->tcpSocket->write(package);
    });

    // Timer de conexion
    timerTimeOut = new QTimer(this);

    timerTimeOut->setSingleShot(true);

    connect(timerTimeOut, &QTimer::timeout, this, [this]()
    {
        emit clientTimeOut(this);

        clientDisconnect();
    });

    timerTimeOut->start(CLIENT_LOGIN_TIMEOUT);
}

HMIClientManager::~HMIClientManager()
{
    // Protocolo
    delete protocolManager;

    delete tcpSocket;

    delete timerTimeOut;
}

void HMIClientManager::clientDisconnect()
{
    tcpSocket->disconnectFromHost();
}

QHostAddress HMIClientManager::getAddress()
{
    return tcpSocket->localAddress();
}
