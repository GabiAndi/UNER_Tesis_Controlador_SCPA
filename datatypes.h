/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 06/01/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Definiciones de tipos especiales de datos.                */
/*************************************************************/

#ifndef DATATYPES_H
#define DATATYPES_H

#include <QObject>

#include <QSize>

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

typedef struct
{
    QString serverIP;
    QString clientIP;
    QString port;
}hmi_server_status_t;

#endif // DATATYPES_H
