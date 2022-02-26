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

#include "logfile.h"
#include "hmiprotocoldata.h"

#include "frequencydriver.h"
#include "pidcontroller.h"

using namespace hmiprotocoldata;

class ControlManager : public QObject
{
        Q_OBJECT

    public:
        explicit ControlManager(QObject *parent = nullptr);
        ~ControlManager();

    public slots:
        void init();

        // Se pide los valores de los sensores
        void getParameterValue(hmiprotocoldata::Sensor sensor);

        // Seteo de variables simuladas
        // Pileta
        void setLvFoso(float lv);
        void setLvLodo(float lv);
        void setTemp(float temp);
        void setOD(float od);
        void setPhAnox(float ph);
        void setPhAireacion(float ph);

    signals:
        // Se envia los parametros de los sensores
        void sendParameterValue(hmiprotocoldata::Sensor sensor, float value);

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Estructura de valores de sensores (simulados)
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
        typedef struct set_point
        {
            float od = 3.4;
        }set_point_t;

        set_point_t *set_points = nullptr;

        // Controlador del variador
        FrequencyDriver *frequencyDriver = nullptr;

        // Controlador del sistem
        PIDController *pidController = nullptr;
};

#endif // CONTROLMANAGER_H
