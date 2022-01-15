#include "hmiclient.h"

HMIClient::HMIClient(QObject *parent)
    : QObject{parent}
{

}

HMIClient::~HMIClient()
{
    // Protocolo
    delete hmiProtocol;

    delete tcpSocket;
}

void HMIClient::tcpSocketDisconnect()
{
    tcpSocket->disconnectFromHost();
}

void HMIClient::tcpSocketDisconnected()
{
    emit clientDisconnected(this);
}

QTcpSocket *HMIClient::getTcpSocket() const
{
    return tcpSocket;
}

void HMIClient::setTcpSocket(QTcpSocket *newTcpSocket)
{
    tcpSocket = newTcpSocket;
}

void HMIClient::init()
{
    // Conexion
    tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    connect(tcpSocket, &QTcpSocket::disconnected, this, &HMIClient::tcpSocketDisconnected);

    // Protocolo
    hmiProtocol = new HMIProtocol(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, [this]()
    {
        hmiProtocol->read(tcpSocket->readAll());
    });

    connect(hmiProtocol, &HMIProtocol::readyWrite, this, [this](const QByteArray package)
    {
        tcpSocket->write(package);
    });

    connect(hmiProtocol, &HMIProtocol::readyRead, this, &HMIClient::newPackage);
}

void HMIClient::newPackage(const uint8_t cmd, const QByteArray payload)
{
    // Se analiza el comando recibido
    switch (cmd)
    {
        /*
         * ALIVE
         *
         * Si recibimos un alive lo devolvemos automaticamente
         * sin informar al client manager.
         */
        case Command::ALIVE:
        {
            if ((uint8_t)(payload.at(0)) != (uint8_t)(0xFF))
                break;

            hmiProtocol->write(Command::ALIVE, QByteArray().append((uint8_t)(0xFF)));

            break;
        }

        /*
         * LOGIN
         *
         * Al recibir un LOGIN extraemos el usuario y contraseña y lo enviamos
         * a las capas superiores.
         */
        case Command::LOGIN:
        {
            uint8_t userLength = payload.at(0);

            QString user(payload.mid(1, userLength));

            uint8_t passwordLength = payload.at(1 + userLength);

            QString password(payload.mid(1 + userLength + 1, passwordLength));

            emit clientLogin(this, user, password);

            break;
        }
    }
}
