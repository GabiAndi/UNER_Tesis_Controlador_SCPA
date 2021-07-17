#ifndef SCPAPROTOCOL_H
#define SCPAPROTOCOL_H

#include <QThread>
#include <QByteArray>
#include <QTimer>
#include <QDebug>

#define PACKAGE_MAX_TIMEOUT_MS 100

class SCPAProtocol : public QThread
{
        Q_OBJECT

    public:
        explicit SCPAProtocol(QObject *parent = nullptr);
        ~SCPAProtocol();

        void readProtocol(const QByteArray dataToRead);

    protected:
        void run() override;

    private:
        QByteArray pendingDataToRead;
        QByteArray pendingPackageToRead;

        uint8_t packageState = 0;

        QTimer *packageTimeOut = nullptr;

    private slots:
        void packageTimeOutTrigger();
};

#endif // SCPAPROTOCOL_H
