#ifndef SCPAPROTOCOL_H
#define SCPAPROTOCOL_H

#include <QThread>
#include <QByteArray>
#include <QTimer>
#include <QDebug>

#define PACKAGE_MAX_LENGTH                  1       // Tamaño maximo en kilobytes del buffer de lectura
#define PACKAGE_MAX_TIMEOUT_MS              100     // Tiempo maximo de espera para la recepción completa de un paquete

class SCPAProtocol : public QThread
{
        Q_OBJECT

    public:
        explicit SCPAProtocol(QObject *parent = nullptr);
        ~SCPAProtocol();

        void readProtocol(const QByteArray dataToRead);
        static uint8_t checksum(QByteArray *data);

    protected:
        void run() override;

    private:
        QByteArray pendingDataToRead;

        enum INFO
        {
            C = (1 << 6),
            F = (1 << 7)
        };

        uint8_t packageState = 0;
        uint16_t packageReadIndex = 0;
        uint8_t packageInfo = 0;
        uint16_t packagePayloadLength = 0;
        QTimer *packageTimeOut = nullptr;

        typedef struct scpaPackage
        {
            uint8_t info;
            uint8_t cmd;
            QByteArray payload;
        }scpaPackage_t;

        scpaPackage_t pendingPackageToRead;

        void packageReset();
        void packageRead(const scpaPackage_t paquete);

    private slots:
        void packageTimeOutTrigger();
};

#endif // SCPAPROTOCOL_H