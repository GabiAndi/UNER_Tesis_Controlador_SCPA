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

#include "logfile.h"
#include "datatypes.h"
#include "hmiclientmanager.h"

#define HMI_SERVER_PORT         33600

class HMIServerManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIServerManager(QObject *parent = nullptr);
        ~HMIServerManager();

    signals:
        //void hmiServerStatus(hmi_server_status_t status);

    public slots:
        void init();

        //void getHmiServerStatus();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Conexiones
        QTcpServer *hmiServer = nullptr;

    private slots:
        // Conexiones entrantes
        void clientConnection();
        void clientConnectionError(const QAbstractSocket::SocketError socketError);
        void clientDisconnection(HMIClientManager *client);

        // Conexiones autenticadas
        void userConnection(HMIClientManager *client);
        void userDisconnection(HMIClientManager *client);
};

#endif // HMISERVERMANAGER_H
