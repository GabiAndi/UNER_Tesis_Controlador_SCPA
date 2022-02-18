/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 15/01/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que describe a un usuario identificado.             */
/*************************************************************/

#ifndef HMIUSER_H
#define HMIUSER_H

#include <QObject>

#include "hmiclient.h"
#include "hmiprotocoldata.h"

using namespace hmiprotocoldata;

class HMIUser : public HMIClient
{
        Q_OBJECT

    public:
        explicit HMIUser(QString userName, QString password, QTcpSocket *tcpSocket,
                         QObject *parent = nullptr);
        ~HMIUser();

        QString getUserName();
        QString getPassword();

    signals:
        // Estado de conexion
        void userForceLogin(HMIUser *user, bool connect);

        void userDisconnected(HMIUser *user);

        // Seteo de variables simuladas
        // Pileta
        void setSimulationLvFoso(float lv);
        void setSimulationLvLodo(float lv);
        void setSimulationTemp(float temp);
        void setSimulationOD(float od);
        void setSimulationPhAnox(float ph);
        void setSimulationPhAireacion(float ph);

        // Motores
        void setSimulationMotorCurrent(float current);
        void setSimulationMotorVoltaje(float voltaje);
        void setSimulationMotorTemp(float temp);
        void setSimulationMotorVelocity(float velocity);

    public slots:
        // Conexion
        void sendLoginForceRequired();
        void sendLoginCorrect();

        void sendDisconnectOtherUserLogin();

        // Parametros de simulacion
        void sendRequestSetParam(hmiprotocoldata::SimulationSensor sensor);

    private:
        // Datos de sesion
        QString userName;
        QString password;

    private slots:
        // Conexion
        void tcpSocketDisconnected() override;

        // Protocolo
        void newPackage(const uint8_t cmd, const QByteArray payload) override;
};

#endif // HMIUSER_H
