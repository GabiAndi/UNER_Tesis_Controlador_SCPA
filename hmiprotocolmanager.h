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
};

#endif // HMIPROTOCOLMANAGER_H
