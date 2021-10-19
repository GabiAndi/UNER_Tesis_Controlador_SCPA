/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 17/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que proporciona una interfaz de consola.            */
/*************************************************************/

#ifndef TUIMANAGER_H
#define TUIMANAGER_H

#include <QObject>
#include <QTextStream>
#include <QProcess>

#include <gnu/libc-version.h>

#include "logfile.h"
#include "consolelistener.h"

class TUIManager : public QObject
{
        Q_OBJECT

    public:
        explicit TUIManager(QObject *parent = nullptr);
        ~TUIManager();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Evento de consola
        ConsoleListener *consoleListener = nullptr;

        // Salida de consola
        QTextStream *consoleOutput = nullptr;

    signals:
        void closeApplication();

    public slots:
        void init();

        void consoleWelcome();
        void consoleClear();
        void consoleWait();

    private slots:
        void consoleReadyLine(const QString &line);
};

#endif // TUIMANAGER_H
