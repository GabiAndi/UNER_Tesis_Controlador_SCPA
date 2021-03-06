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
#include <QTimer>
#include <QSize>

#include <ncurses.h>
#undef timeout

#include <gnu/libc-version.h>

#include "logfile.h"
#include "consolelistener.h"
#include "hmiusersmanager.h"

#define WINDOW_PADDING      6
#define TEXT_CENTER(str)    ((screenGetSize().width() - WINDOW_PADDING - 2 - str.length()) / 2)

class TUIManager : public QObject
{
        Q_OBJECT

    public:
        explicit TUIManager(QObject *parent = nullptr);
        ~TUIManager();

    signals:
        void closedApplication();

    public slots:
        void init();

    private:
        // Archivo de logs
        LogFile *logFile = nullptr;

        // Evento de consola
        QThread *consoleThread = nullptr;
        ConsoleListener *consoleListener = nullptr;

        // Pantalla de consola
        void refreshScreen();
        QSize screenGetSize();

        void welcomeScreen();
        void homeScreen();
        void exitScreen();
        void testScreen();
        void usersScreen();

        void initColors();

        enum Color : short
        {
            ALERT_COLORS = 1,
            WARNING_COLORS,
            INFO_COLORS
        };

        enum Screen : uint8_t
        {
            NO_IMPLEMENTED_SCREEN,
            WELLCOME_SCREEN,
            HOME_SCREEN,
            EXIT_SCREEN,
            TEST_SCREEN,
            USERS_SCREEN
        };

        typedef struct home_screen_option
        {
            QString text = "Sin titulo";
            Screen screen = NO_IMPLEMENTED_SCREEN;
        }home_screen_option_t;

        typedef struct state
        {
            Screen currentScreen;
            int homeScreenIndex;
            int lastKey;
        }state_t;

        state_t *tuiState = nullptr;

        QList<home_screen_option_t> *homeScreenOptions = nullptr;

        // Aplicacion
        void closeApplication();

    private slots:
        // Evento de recepci??n de tecla
        void consoleNewKey(const int key);
};

#endif // TUIMANAGER_H
