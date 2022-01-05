#include "hmiprotocolpackage.h"

HMIProtocolPackage::HMIProtocolPackage(QObject *parent)
    : QObject{parent}
{

}

bool HMIProtocolPackage::packageReadAvailable()
{
    return (packageReadIndex < packageReadData.length());
}

void HMIProtocolPackage::packageReadReset()
{
    packageReadData.remove(0, packageReadIndex);

    packageReadIndex = 0;
    packageReadState = 0;

    packageReadPayloadLength = 0;
    packageReadCmd = 0x00;
    packageReadPayload.clear();
}

uint8_t HMIProtocolPackage::packageReadNextByte()
{
    if (packageReadAvailable())
    {
        return packageReadData.at(packageReadIndex++);
    }

    return 0x00;
}

