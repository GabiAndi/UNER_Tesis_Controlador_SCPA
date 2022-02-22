/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 20/02/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que envia instrucciones al variador de velocidad.   */
/*************************************************************/

#ifndef FREQUENCYDRIVER_H
#define FREQUENCYDRIVER_H

#include <QObject>

#include <wiringPi.h>

class FrequencyDriver : public QObject
{
        Q_OBJECT

    public:
        explicit FrequencyDriver(QObject *parent = nullptr);
        ~FrequencyDriver();

        enum GPIO
        {
            PIN_ON_OFF = 21,
            PIN_VEL_BYTE1,
            PIN_VEL_BYTE2,
            PIN_VEL_BYTE3,
            PIN_VEL_BYTE4
        };

        void start();
        void stop();

        void setFreq(uint8_t freq);
        void setVel(uint16_t rpm);
};

#endif // FREQUENCYDRIVER_H
