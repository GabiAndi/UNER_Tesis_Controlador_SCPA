#ifndef SCPA_H
#define SCPA_H

#include <QObject>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>

class SCPA : public QObject
{
        Q_OBJECT

    public:
        explicit SCPA(QObject *parent = nullptr);
        ~SCPA();

        bool init();

        void setFreq(float freq);
        void setRPM(float rpm);

    signals:
        void scpaConnected();
        void scpaErrorConnected(QTcpSocket::SocketError error);
        void scpaDisconnected();

        void readOD(float od);

    private:
        // Configuracion del sistema
        inline static const QString configSubdir = "config";
        inline static const QString configFileName = "scpa.conf";

        // Socket de comunicacion
        QTcpSocket *tcpSocket = nullptr;

        // Valores de configuración
        QString ip;
        int port;

        // Timer de captura de datos
        QTimer *timer = nullptr;

        float sensorOD = 0;

    private slots:
        void readyRead();
};

#endif // SCPA_H
