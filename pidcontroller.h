/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 18/02/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase controladora del sistema de control adoptado.       */
/*************************************************************/

#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <QObject>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>

class PIDController : public QObject
{
        Q_OBJECT

    public:
        explicit PIDController(QObject *parent = nullptr);
        ~PIDController();

        bool init();

        float getError();

        bool setKp(float kp);
        float getKp();

        bool setKd(float kd);
        float getKd();

        bool setKi(float ki);
        float getKi();

        float getRPMKp();
        float getRPMKd();
        float getRPMKi();

        bool saveKPID();

        int8_t getFreq(float setPointOD, float od, float temp);

        void resetPID();

    private:
        // Configuracion del sistema
        inline static const QString configSubdir = "config";
        inline static const QString configFileName = "pid.conf";

        // Constantes del controlador
        float kp = 0;
        float kd = 0;
        float ki = 0;

        // Valores del controlador
        float rpmKp = 0;
        float rpmKd = 0;
        float rpmKi = 0;

        // Variables del controlador
        float error = 0;
        float error_prev = 0;
        float error_int = 0;
};

#endif // PIDCONTROLLER_H
