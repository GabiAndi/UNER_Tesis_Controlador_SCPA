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

#include "logfile.h"
#include "hmiprotocolmanager.h"
#include "datatypes.h"
#include "hmiusersmanager.h"

#define HMI_SERVER_PORT           33600

class HMIServerManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIServerManager(QObject *parent = nullptr);
        ~HMIServerManager();

    signals:
        void readProtocol(const QByteArray data);

        void hmiServerStatus(hmi_server_status_t status);

    public slots:
        void init();

        void getHmiServerStatus();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Conexiones
        QTcpServer *hmiServer = nullptr;    // Servidor HMI

        QTcpSocket *clientTcpSocket = nullptr;

        QThread *clientProtocolThread = nullptr;
        HMIProtocolManager *clientProtocolManager = nullptr;

        // Usuarios
        HMIUsersManager *hmiUsersManager = nullptr;

    private slots:
        // Conexiones
        void newConnection();
        void newConnectionError(const QAbstractSocket::SocketError socketError);

        void clientDisconnected();

        // Datos
        void readData();
};

#endif // HMISERVERMANAGER_H
