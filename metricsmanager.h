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

#include <QSqlDatabase>

class MetricsManager : public QObject
{
        Q_OBJECT

    public:
        explicit MetricsManager(QObject *parent = nullptr);
        ~MetricsManager();

    private:
        QSqlDatabase *database = nullptr;
};

#endif // METRICSMANAGER_H
