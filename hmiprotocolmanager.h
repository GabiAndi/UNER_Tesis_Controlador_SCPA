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

#include <QTimer>

#include "datatypes.h"

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
        hmi_protocol_package_t *hmi_protocol_package = nullptr;

    private slots:
        void readReset();
};

#endif // HMIPROTOCOLMANAGER_H
