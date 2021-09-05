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

#include "hmiprotocolpackage.h"

class HMIProtocolManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIProtocolManager(QObject *parent = nullptr);
        ~HMIProtocolManager();

    private:
        HMIProtocolPackage *hmiProtocolPackage = nullptr;

        QTimer *readProtocolTimer = nullptr;

        uint16_t readProtocolDataMaxLength = 1;  // Tamaño en Kb maximo del buffer de lectura
        uint32_t readProtocolTimeOutMs = 100;  // Tiempo maximo en ms hasta que se descarta el paquete

        void hmiNewCommand(uint8_t &cmd, QByteArray &payload);

    public slots:
        void init();

        void readProtocol(const QByteArray data);

    private slots:
        void readProtocolReset();
};

#endif // HMIPROTOCOLMANAGER_H
