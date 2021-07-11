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
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class HMIManager : public QThread
{
        Q_OBJECT

    public:
        explicit HMIManager(QObject *parent = nullptr);
        ~HMIManager();

    protected:
        void run() override;

    private:
        QTcpServer *hmiServer = nullptr;
        QTcpSocket *hmiClient = nullptr;

    public slots:
        void newConnection();
};

#endif // HMIMANAGER_H
