/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 20/02/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que administra el envio de datos de metricas a la   */
/* base de datos que almacenará todos los valores del        */
/* proceso.                                                  */
/*************************************************************/

#ifndef METRICSMANAGER_H
#define METRICSMANAGER_H

#include <QObject>

#include <QTimer>

#include "logfile.h"
#include "mysqldatabase.h"
#include "hmiprotocoldata.h"

using namespace hmiprotocoldata;

class MetricsManager : public QObject
{
        Q_OBJECT

    public:
        explicit MetricsManager(QObject *parent = nullptr);
        ~MetricsManager();

    signals:
        void getMetricSensorValue(hmiprotocoldata::Sensor sensor);
        void getMetricSystemState(hmiprotocoldata::SystemState state);

    public slots:
        void init();

        // Se envia los parametros de los sensores
        void sendMetricSensorValue(hmiprotocoldata::Sensor sensor, float value);
        void sendMetricSystemState(hmiprotocoldata::SystemState state, float value);

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Base de datos
        MySQLDatabase *database = nullptr;

        // Timer de envio de datos
        QTimer *timer = nullptr;

    private slots:
        void sendData();
};

#endif // METRICSMANAGER_H
