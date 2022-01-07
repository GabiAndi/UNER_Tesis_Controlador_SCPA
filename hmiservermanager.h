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
/* El servidor solo aceptará una sesión de HMI, cualquier    */
/* intento de conexión pedirá cerrar la sesión activa.       */
/*************************************************************/

#ifndef HMISERVERMANAGER_H
#define HMISERVERMANAGER_H

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QByteArray>

#include "logfile.h"
#include "hmiprotocolmanager.h"

#define HMI_SERVER_PORT           33600

class HMIServerManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIServerManager(QObject *parent = nullptr);
        ~HMIServerManager();

    signals:
        void readProtocol(const QByteArray data);

    public slots:
        void init();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Conexiones
        QTcpServer *hmiServer = nullptr;    // Servidor HMI

        QTcpSocket *clientTcpSocket = nullptr;

        QThread *clientProtocolThread = nullptr;
        HMIProtocolManager *clientProtocolManager = nullptr;

    private slots:
        // Conexiones
        void newConnection();
        void newConnectionError(const QAbstractSocket::SocketError socketError);

        void clientDisconnected();

        // Datos
        void readData();
};

#endif // HMISERVERMANAGER_H
