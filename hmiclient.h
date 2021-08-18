/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 18/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Gestiona los eventos del cliente HMI.                     */
/*************************************************************/

#ifndef HMICLIENT_H
#define HMICLIENT_H

#include <QObject>
#include <QTcpSocket>

class HMIClient : public QObject
{
        Q_OBJECT

    public:
        explicit HMIClient(QTcpSocket *tcpSocket, QObject *parent = nullptr);
        ~HMIClient();

    private:
        QTcpSocket *tcpSocket = nullptr;

    public slots:
        void hmiClientDisconnected();
        void hmiClientReadData();

    signals:
        void hmiClientClosed(HMIClient *hmiClient);
};

#endif // HMICLIENT_H
