#include "hmiclient.h"

HMIClient::HMIClient(QTcpSocket *tcpSocket, QObject *parent)
    : QObject{parent}, tcpSocket{tcpSocket}
{
    // Conexion
    tcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    connect(tcpSocket, &QTcpSocket::disconnected, this, &HMIClient::tcpSocketDisconnected);

    // Protocolo
    hmiProtocol = new HMIProtocol(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, [this]()
    {
        hmiProtocol->read(this->tcpSocket->readAll());
    });

    connect(hmiProtocol, &HMIProtocol::readyWrite, this, [this](const QByteArray package)
    {
        this->tcpSocket->write(package);
    });

    connect(hmiProtocol, &HMIProtocol::readyRead, this, &HMIClient::newPackage);
}

HMIClient::~HMIClient()
{
    // Protocolo
    delete hmiProtocol;
}

void HMIClient::tcpSocketDisconnect()
{
    tcpSocket->disconnectFromHost();
}

void HMIClient::sendLoginError()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_ERROR));
}

void HMIClient::tcpSocketDisconnected()
{
    emit clientDisconnected(this);
}

QTcpSocket *HMIClient::getTcpSocket() const
{
    return tcpSocket;
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
            if ((uint8_t)(payload.at(0)) != Payload::PAYLOAD_OK)
                break;

            hmiProtocol->write(Command::ALIVE, QByteArray().append(Payload::PAYLOAD_OK));

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

            QString userName(payload.mid(1, userLength));

            uint8_t passwordLength = payload.at(1 + userLength);

            QString password(payload.mid(1 + userLength + 1, passwordLength));

            emit clientLogin(this, userName, password);

            break;
        }
    }
}
