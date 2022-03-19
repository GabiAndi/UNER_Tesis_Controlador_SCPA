#include "scpa.h"

SCPA::SCPA(QObject *parent)
    : QObject{parent}
{
    // Socket TCP
    tcpSocket = new QTcpSocket(this);

    tcpSocket->setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);

    connect(tcpSocket, &QTcpSocket::connected, this, [this]()
    {
        timer->start(2000);

        emit scpaConnected();
    });
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, [this](QTcpSocket::SocketError error)
    {
        timer->stop();

        emit scpaErrorConnected(error);
    });
    connect(tcpSocket, &QTcpSocket::disconnected, this, [this]()
    {
        timer->stop();

        emit scpaDisconnected();
    });

    connect(tcpSocket, &QTcpSocket::readyRead, this, &SCPA::readyRead);

    // Timer de captura
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]()
    {
        tcpSocket->write(QByteArray("od"));
    });
}

SCPA::~SCPA()
{
    delete tcpSocket;

    delete timer;
}

bool SCPA::init()
{
    // Si la carpeta no existe se crea y se devuelve fail
    if (!QDir(configSubdir).exists())
    {
        QDir().mkdir(configSubdir);

        return false;
    }

    // Se abre el archivo para leer la configuracion
    QFile configFile(configSubdir + "/" + configFileName);

    // Si el archivo no existe se devuelve fail
    if (!configFile.exists())
    {
        return false;
    }

    // Si todo esta ok se procede a leer el archivo
    configFile.open(QIODevice::OpenModeFlag::ReadOnly);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(configFile.readAll());

    configFile.close();

    QJsonObject jsonConfig = jsonDocument.object();

    // Datos
    ip = jsonConfig["ip"].toString();
    port = jsonConfig["port"].toInt();

    // Inicio de la comunicación
    tcpSocket->connectToHost(QHostAddress(ip), port);

    return true;
}

void SCPA::setFreq(float freq)
{
    float rpm = freq * 56;

    setRPM(rpm);
}

void SCPA::setRPM(float rpm)
{
    QString data = QString::asprintf("rpm=%f", rpm);

    tcpSocket->write(data.toLatin1());
}

void SCPA::readyRead()
{
    sensorOD = tcpSocket->readAll().toFloat();

    emit readOD(sensorOD);
}
