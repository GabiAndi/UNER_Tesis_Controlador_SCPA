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
    QString serverIP;
    QString clientIP;
    QString port;
}hmi_server_status_t;

#endif // DATATYPES_H
