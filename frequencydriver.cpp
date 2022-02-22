#include "frequencydriver.h"

FrequencyDriver::FrequencyDriver(QObject *parent)
    : QObject{parent}
{
    // Inicio de los pines gpio
    wiringPiSetup();

    pinMode(GPIO::PIN_ON_OFF, INPUT);

    pinMode(GPIO::PIN_VEL_BYTE1, INPUT);
    pinMode(GPIO::PIN_VEL_BYTE2, INPUT);
    pinMode(GPIO::PIN_VEL_BYTE3, INPUT);
    pinMode(GPIO::PIN_VEL_BYTE4, INPUT);

    digitalWrite(GPIO::PIN_ON_OFF, LOW);

    digitalWrite(GPIO::PIN_VEL_BYTE1, HIGH);
    digitalWrite(GPIO::PIN_VEL_BYTE2, HIGH);
    digitalWrite(GPIO::PIN_VEL_BYTE3, HIGH);
    digitalWrite(GPIO::PIN_VEL_BYTE4, HIGH);
}

FrequencyDriver::~FrequencyDriver()
{

}

void FrequencyDriver::start()
{
    digitalWrite(GPIO::PIN_ON_OFF, HIGH);
}

void FrequencyDriver::stop()
{
    digitalWrite(GPIO::PIN_ON_OFF, LOW);
}

void FrequencyDriver::setFreq(uint8_t freq)
{
    uint8_t pinState = ~(freq / 4);

    digitalWrite(PIN_VEL_BYTE1, pinState & (1 << 0));
    digitalWrite(PIN_VEL_BYTE2, pinState & (1 << 1));
    digitalWrite(PIN_VEL_BYTE3, pinState & (1 << 2));
    digitalWrite(PIN_VEL_BYTE4, pinState & (1 << 3));
}

void FrequencyDriver::setVel(uint16_t rpm)
{
    uint8_t freq = rpm / 56;

    setFreq(freq);
}
