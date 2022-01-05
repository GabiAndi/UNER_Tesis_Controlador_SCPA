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
#include <QList>

#include "logfile.h"
#include "hmiclient.h"

class HMIServerManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIServerManager(QObject *parent = nullptr);
        ~HMIServerManager();

    public slots:
        void init();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Conexiones
        const int hmiServerPort = 33600;

        QTcpServer *hmiServer = nullptr;    // Servidor HMI

        HMIClient *hmiClient = nullptr;   // Sesion activa de HMI

    private slots:
        // Conexiones
        void newConnection();
        void newConnectionError(const QAbstractSocket::SocketError socketError);

        void hmiClientDisconnected(HMIClient *hmiClient);
};

#endif // HMISERVERMANAGER_H
