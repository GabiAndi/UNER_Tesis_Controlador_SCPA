/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 20/07/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que controla el protocolo de comunicación propio.   */
/*************************************************************/

#ifndef SCPAPROTOCOL_H
#define SCPAPROTOCOL_H

#include <QThread>
#include <QByteArray>
#include <QTimer>

class SCPAProtocol : public QThread
{
        Q_OBJECT

    public:
        explicit SCPAProtocol(QObject *parent = nullptr);
        ~SCPAProtocol();

        void setExitPending();
        bool getExitPending();

        void readProtocol(const QByteArray dataToRead);
        static uint8_t checksum(QByteArray &data);

        static const uint16_t PACKAGE_MAX_LENGTH = 2;  // Tamaño maximo en kilobytes del buffer de lectura

    protected:
        void run() override;

    private:
        QByteArray pendingDataToRead;

        bool exitPending = false;
};

#endif // SCPAPROTOCOL_H
