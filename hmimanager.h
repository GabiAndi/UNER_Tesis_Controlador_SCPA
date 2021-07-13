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

class HMIManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIManager(QObject *parent = nullptr);
        ~HMIManager();

    private:
        // Conexiones
        QTcpServer *hmiServer = nullptr;
        QTcpSocket *hmiClient = nullptr;

        // Cliente
        void clientDisconnected();
        void clientReadData();

    public slots:
        // Conexiones
        void newConnection();
        void newConnectionError(const QAbstractSocket::SocketError socketError);
};

#endif // HMIMANAGER_H
