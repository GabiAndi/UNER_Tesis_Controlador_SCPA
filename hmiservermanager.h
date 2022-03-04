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
#include "hmiusersmanager.h"
#include "hmiclient.h"
#include "hmiuser.h"

#define HMI_SERVER_PORT                 33600

class HMIServerManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIServerManager(QObject *parent = nullptr);
        ~HMIServerManager();

    public slots:
        void init();

        // Se envia los parametros de los sensores
        void sendSensorValue(hmiprotocoldata::Sensor sensor, float value);

        // Estado del sistema
        void sendSystemState(hmiprotocoldata::SystemState state, float value);

    signals:
        // Sensores
        void getSensorValue(hmiprotocoldata::Sensor sensor);
        void setSensorValue(hmiprotocoldata::Sensor sensor, float value);

        // Estado del sistema
        void getSystemState(hmiprotocoldata::SystemState state);
        void setSystemState(hmiprotocoldata::SystemState state, float value);

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Conexiones
        QTcpServer *hmiServer = nullptr;

        // Usuario activo
        HMIUser *activeUser = nullptr;

        // Marco el usuario actual como el activo
        void setActiveUser(HMIUser *user);

    private slots:
        // Conexiones
        void clientConnection();
        void clientConnectionError(const QAbstractSocket::SocketError socketError);
        void clientDisconnection(HMIClient *client);

        void clientLogin(HMIClient *client, const QString userName, const QString password);

        void userForceLogin(HMIUser *user, bool connecting);
        void userDisconnection(HMIUser *user);
};

#endif // HMISERVERMANAGER_H
