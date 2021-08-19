#include "hmiclient.h"

HMIClient::HMIClient(QTcpSocket *tcpSocket, QObject *parent) : QObject(parent)
{
    this->tcpSocket = tcpSocket;

    // Comunicacion
    connect(this->tcpSocket, &QTcpSocket::disconnected, this, &HMIClient::hmiClientDisconnected);
    connect(this->tcpSocket, &QTcpSocket::readyRead, this, &HMIClient::hmiClientReadData);

    // Protocolo
    hmiProtocolManager = new HMIProtocolManager();
    hmiProtocolThread = new QThread(this);

    hmiProtocolManager->moveToThread(hmiProtocolThread);

    connect(hmiProtocolThread, &QThread::started, hmiProtocolManager, &HMIProtocolManager::init);
    connect(this, &HMIClient::hmiClientReadProtocol, hmiProtocolManager, &HMIProtocolManager::readProtocol);

    hmiProtocolThread->start();
}

HMIClient::~HMIClient()
{
    // Se finaliza el hilo del analisis de paquete
    hmiProtocolThread->quit();
    hmiProtocolThread->wait();

    delete hmiProtocolManager;
    delete hmiProtocolThread;

    // Se borra el socket
    delete tcpSocket;
}

void HMIClient::disconnect()
{
    tcpSocket->disconnectFromHost();
}

void HMIClient::hmiClientDisconnected()
{
    emit hmiClientClosed(this);
}

void HMIClient::hmiClientReadData()
{
    // Interpretacion de los comandos enviados por los clientes
    emit hmiClientReadProtocol(tcpSocket->readAll());
}
