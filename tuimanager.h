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

    private slots:
        void consoleReadyLine(const QString &line);

    signals:
        void closeApplication();
};

#endif // TUIMANAGER_H
