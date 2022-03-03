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

class PIDController : public QObject
{
        Q_OBJECT

    public:
        explicit PIDController(QObject *parent = nullptr);
        ~PIDController();

        uint8_t getFreq(float setPointOD, float od, float temp);
};

#endif // PIDCONTROLLER_H
