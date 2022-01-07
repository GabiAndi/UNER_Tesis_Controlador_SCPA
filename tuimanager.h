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

#include <ncurses.h>
#undef timeout
#include <menu.h>

#include <gnu/libc-version.h>

#include "logfile.h"
#include "consolelistener.h"
#include "datatypes.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

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

        void consoleWelcome();
        void consoleClear();
        void consoleWait();

        // Aplicacion
        void closeApplication();

    private slots:
        void consoleNewKey(const int key);
};

#endif // TUIMANAGER_H
