#include "hmiclient.h"

HMIClient::HMIClient(QTcpSocket *tcpSocket, QObject *parent)
    : QObject{parent}, tcpSocket{tcpSocket}
{
    // Conexion
    tcpSocket->setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);

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

void HMIClient::sendAlive()
{
    hmiProtocol->write(Command::KEEP_ALIVE, QByteArray().append(KeepAliveMode::REPLY));
}

void HMIClient::sendLoginError()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_ERROR));
}

void HMIClient::sendDisconnectTimeOut()
{
    hmiProtocol->write(Command::DISCONNECT_CODE, QByteArray().append(DisconnectCode::LOGIN_TIMEOUT));
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
    switch ((Command)(cmd))
    {
        /*
         * ALIVE
         *
         * Si recibimos un alive lo devolvemos automaticamente
         * sin informar al client manager.
         */
        case Command::KEEP_ALIVE:
        {
            if ((uint8_t)(payload.at(0)) == KeepAliveMode::REPLY)
            {
                hmiProtocol->write(Command::KEEP_ALIVE, QByteArray().append(KeepAliveMode::REQUEST));
            }

            else if ((uint8_t)(payload.at(0)) == KeepAliveMode::REQUEST)
            {

            }

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

        default:
            break;
    }
}
