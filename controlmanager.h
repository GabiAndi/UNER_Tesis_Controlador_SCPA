/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 18/02/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase controladora del sistema.                           */
/*************************************************************/

#ifndef CONTROLMANAGER_H
#define CONTROLMANAGER_H

#include <QObject>

#include <QTimer>

#include "logfile.h"
#include "hmiprotocoldata.h"

#include "frequencydriver.h"
#include "pidcontroller.h"
#include "scpa.h"

using namespace hmiprotocoldata;

class ControlManager : public QObject
{
        Q_OBJECT

    public:
        explicit ControlManager(QObject *parent = nullptr);
        ~ControlManager();

    public slots:
        void init();

        // Sensores
        void getSensorValue(hmiprotocoldata::Sensor sensor);
        void setSensorValue(hmiprotocoldata::Sensor sensor, float value);

        // Estado del sistema
        void getSystemState(hmiprotocoldata::SystemState state);
        void setSystemState(hmiprotocoldata::SystemState state, float value);

        // Sensores al servidor de metricas
        void getMetricSensorValue(hmiprotocoldata::Sensor sensor);
        void getMetricSystemState(hmiprotocoldata::SystemState state);

    signals:
        // Sensores
        void sendSensorValue(hmiprotocoldata::Sensor sensor, float value);

        // Estado del sistema
        void sendSystemState(hmiprotocoldata::SystemState state, float value);

        // Sensores
        void sendMetricSensorValue(hmiprotocoldata::Sensor sensor, float value);
        void sendMetricSystemState(hmiprotocoldata::SystemState state, float value);

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Estructura de valores de sensores
        typedef struct sensors_pileta
        {
            float lvFoso = 0;
            float lvLodo = 0;
            float temp = 0;
            float od = 0;
            float phAnox = 0;
            float phAireacion = 0;
        }sensors_pileta_t;

        typedef struct sensors_motor
        {
            float current = 0;
            float voltaje = 0;
            float temperature = 0;
            float velocity = 0;
        }sensors_motor_t;

        typedef struct
        {
            // Pileta
            sensors_pileta_t pileta;

            // Motores
            sensors_motor_t motor;
        }sensors_t;

        sensors_t *sensors = nullptr;

        // Estructura de valores de seteo
        typedef struct system_state
        {
            float od = 0;
            bool active = false;
        }system_state_t;

        system_state_t *systemState = nullptr;

        // Controlador del variador
        FrequencyDriver *frequencyDriver = nullptr;

        // Controlador del sistem
        PIDController *pidController = nullptr;

        // Sensor de OD
        SCPA *scpa = nullptr;

        // Timer de PID
        QTimer *pidTimer = nullptr;

    private slots:
        // Funcion de calculo de PID
        void syncPID();

        // Slot de conexion correcta del sensor de od
        void scpaConnected();
        void scpaErrorConnected(QTcpSocket::SocketError error);
        void scpaDisconnected();
};

#endif // CONTROLMANAGER_H
