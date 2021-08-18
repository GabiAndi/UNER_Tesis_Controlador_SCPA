#include "hmiclient.h"

HMIClient::HMIClient(QTcpSocket *tcpSocket, QObject *parent) : QObject(parent)
{
    this->tcpSocket = tcpSocket;

    // Comunicacion
    connect(this->tcpSocket, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
    connect(this->tcpSocket, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);
}

HMIClient::~HMIClient()
{
    delete tcpSocket;
}

void HMIClient::hmiClientDisconnected()
{
    emit hmiClientClosed(this);
}

void HMIClient::hmiClientReadData()
{
    qDebug() << tcpSocket->readAll();
}
