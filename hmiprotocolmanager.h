/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 19/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Gestiona el protocolo de comunicacion desarrollado para   */
/* el sistema.                                               */
/*************************************************************/

#ifndef HMIPROTOCOLMANAGER_H
#define HMIPROTOCOLMANAGER_H

#include <QObject>

#include <QByteArray>
#include <QTimer>

#include <QDebug>

class HMIProtocolManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIProtocolManager(QObject *parent = nullptr);
        ~HMIProtocolManager();

    public slots:
        void init();

        void readProtocol(const QByteArray data);

    private:
        // Timer del paquete de datos
        QTimer *timeOutTimer = nullptr;

        void newCommand(uint8_t &cmd, QByteArray &payload);

        // Paquete de datos
        typedef struct
        {
            uint16_t dataMaxLength;
            uint32_t timeOutMs;

            QByteArray packageReadData;

            uint16_t packageReadIndex;
            uint8_t packageReadState;

            uint16_t packageReadPayloadLength;
            uint8_t packageReadCmd;
            QByteArray packageReadPayload;
        }hmi_protocol_package_t;

        hmi_protocol_package_t *hmi_protocol_package = nullptr;

    private slots:
        void readReset();
};

#endif // HMIPROTOCOLMANAGER_H
