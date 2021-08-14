/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 07/07/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Gestion de la conexion del sistema control (servidor      */
/* de configuracion) y un HMI que se conecte (cliente de     */
/* interfaz grafica).                                        */
/* El servidor escuchara por el puerto 33600 las conexiones  */
/* entrantes.                                                */
/*************************************************************/

#ifndef HMIMANAGER_H
#define HMIMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "logfile.h"
#include "hmiclient.h"

class HMIManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIManager(QObject *parent = nullptr);
        ~HMIManager();

        void start();

    private:
        // Log
        LogFile *logFile = nullptr;

        // Conexiones
        QTcpServer *hmiServer = nullptr;    // Servidor HMI

        QList<HMIClient *> *hmiClients = nullptr;   // Sesiones activas HMI

        uint8_t hmiClientsMax = 5;  // Numero maximo de sesiones por defecto

        int getID();    // Devuelve un id para el nuevo cliente si hay lugar

    public slots:
        // Conexiones
        void newConnection();
        void newConnectionError(const QAbstractSocket::SocketError socketError);
        void hmiClientDisconnected(HMIClient *hmiClient);
};

#endif // HMIMANAGER_H
