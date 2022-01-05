/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 31/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que proporciona la estructura de un paquete de      */
/* datos del protocolo de comunicación.                      */
/*************************************************************/

#ifndef HMIPROTOCOLPACKAGE_H
#define HMIPROTOCOLPACKAGE_H

#include <QObject>

class HMIProtocolPackage : public QObject
{
        Q_OBJECT

    public:
        explicit HMIProtocolPackage(QObject *parent = nullptr);

        bool packageReadAvailable();
        void packageReadReset();
        uint8_t packageReadNextByte();

        // Paquete
        QByteArray packageReadData;

        uint16_t packageReadIndex = 0;
        uint8_t packageReadState = 0;

        uint16_t packageReadPayloadLength = 0;
        uint8_t packageReadCmd = 0x00;
        QByteArray packageReadPayload;
};

#endif // HMIPROTOCOLPACKAGE_H
