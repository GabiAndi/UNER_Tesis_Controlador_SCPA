/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 17/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que proporciona una interfaz de consola de manera   */
/* asincrona.                                                */
/*************************************************************/

#ifndef TUIMANAGER_H
#define TUIMANAGER_H

#include <QObject>

#include <QThread>
#include <QTextStream>
#include <QProcess>

#include <gnu/libc-version.h>

#include "logfile.h"
#include "consolelistener.h"
#include "datatypes.h"

class TUIManager : public QObject
{
        Q_OBJECT

    public:
        explicit TUIManager(QObject *parent = nullptr);
        ~TUIManager();

    signals:
        void closedApplication();

        void getHmiServerStatus();

    public slots:
        void init();

        void hmiServerStatus(hmi_server_status_t status);

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Evento de consola
        QThread *consoleThread = nullptr;
        ConsoleListener *consoleListener = nullptr;

        // Salida de consola
        QTextStream *consoleOutput = nullptr;

        void consoleWelcome();
        void consoleClear();
        void consoleWait();

    private slots:
        void consoleReadyLine(const QString line);
};

#endif // TUIMANAGER_H
