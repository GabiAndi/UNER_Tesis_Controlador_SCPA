/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 18/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Gestion de la conexion del sistema control (servidor      */
/* de configuracion) y un HMI que se conecte (cliente de     */
/* interfaz grafica).                                        */
/* El servidor escuchara por el puerto 33600 las conexiones  */
/* entrantes.                                                */
/*************************************************************/

#ifndef HMISERVERMANAGER_H
#define HMISERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "logfile.h"
#include "hmiclient.h"

class HMIServerManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIServerManager(QObject *parent = nullptr);
        ~HMIServerManager();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Conexiones
        const int hmiServerPort = 33600;

        QTcpServer *hmiServer = nullptr;    // Servidor HMI

        QList<HMIClient *> *hmiClients = nullptr;   // Sesiones activas HMI

        uint8_t hmiClientsMax = 5;  // Numero maximo de sesiones por defecto

    public slots:
        void init();

        // Conexiones
        void newConnection();
        void newConnectionError(const QAbstractSocket::SocketError socketError);

    private slots:
        void hmiClientDisconnected(HMIClient *hmiClient);
};

#endif // HMISERVERMANAGER_H
