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

        void consoleWelcome();
        void consoleMenu();
        void consolePrintKeyboardCode(int key);

        QSize screenGetSize(WINDOW *win = stdscr);

        void consoleInitColors();

        enum ConsoleColor : short
        {
            ALERT = 1,
            WARNING,
            INFO
        };

        enum ConsoleCommand : short
        {
            EXIT,
            HMI_SERVER_INFO
        };

        typedef struct
        {
            QString text;
            ConsoleCommand id;
        }tui_menu_command_t;

        typedef struct
        {
            int menuIndex;
        }tui_state_t;

        tui_state_t *tuiState = nullptr;

        QList<tui_menu_command_t> *menuCommands = nullptr;

        // Comandos
        void executeCommand(const ConsoleCommand id);

        // Aplicacion
        void closeApplication();

    private slots:
        void consoleNewKey(const int key);
};

#endif // TUIMANAGER_H
