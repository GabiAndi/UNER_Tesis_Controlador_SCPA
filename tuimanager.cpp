#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent)
    : QObject{parent}
{

}

TUIManager::~TUIManager()
{
    // Detenemos el controlador de consola
    consoleThread->quit();
    consoleThread->wait();

    delete consoleListener;
    delete consoleThread;

    // Consola
    endwin();

    delete tuiState;

    delete homeScreenOptions;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void TUIManager::init()
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Iniciamos la pantalla
    // Iniciamos ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    initColors();

    // Consola
    tuiState = new state_t;

    tuiState->currentScreen = Screen::WELLCOME_SCREEN;
    tuiState->homeScreenIndex = 0;

    // Iniciamos la captura de la consola
    consoleThread = new QThread(this);
    consoleListener = new ConsoleListener();

    consoleListener->moveToThread(consoleThread);

    connect(consoleThread, &QThread::started, consoleListener, &ConsoleListener::init);

    connect(consoleListener, &ConsoleListener::newKey, this, &TUIManager::consoleNewKey);

    consoleThread->start();

    // Comandos
    homeScreenOptions = new QList<home_screen_option_t>;

    homeScreenOptions->append(home_screen_option_t {
                                  "Usuarios", Screen::USERS_SCREEN
                              });

    homeScreenOptions->append(home_screen_option_t {
                                  "Testeo", Screen::TEST_SCREEN
                              });

    homeScreenOptions->append(home_screen_option_t {
                                  "Salir", Screen::EXIT_SCREEN
                              });

    // Mensaje de inicio
    logFile->println("Cargado");

    // Mostramos la pantalla
    refreshScreen();
}

void TUIManager::refreshScreen()
{
    // Borramos el contenido de la pantalla principal
    clear();
    refresh();

    switch (tuiState->currentScreen)
    {
        case Screen::WELLCOME_SCREEN:
            welcomeScreen();

            break;

        case Screen::HOME_SCREEN:
            homeScreen();

            break;

        case Screen::USERS_SCREEN:
            usersScreen();

            break;

        case Screen::TEST_SCREEN:
            testScreen();

            break;

        case Screen::EXIT_SCREEN:
            exitScreen();

            break;

        default:
            break;
    }
}

void TUIManager::consoleNewKey(const int key)
{
    // Captura de tecla
    switch (key)
    {
        // F5
        case 269:
            // Reservado para la actualización manual de la pantalla
            break;

        // Flecha hacia abajo
        case 258:
            if (tuiState->currentScreen == Screen::HOME_SCREEN)
            {
                tuiState->homeScreenIndex++;

                if (tuiState->homeScreenIndex >= homeScreenOptions->length())
                    tuiState->homeScreenIndex = 0;
            }

            break;

        // Flecha hacia arriba
        case 259:
            if (tuiState->currentScreen == Screen::HOME_SCREEN)
            {
                tuiState->homeScreenIndex--;

                if (tuiState->homeScreenIndex < 0)
                    tuiState->homeScreenIndex = homeScreenOptions->length() - 1;

                break;
            }

        // Enter
        case 10:
            // Si estamos en el menu principal vamos a la pantalla seleccionada
            if (tuiState->currentScreen == Screen::HOME_SCREEN)
            {
                tuiState->currentScreen = homeScreenOptions->at(tuiState->homeScreenIndex).screen;
            }

            // Si estamos en el mensaje de bienvenida nos vamos al menu principal
            else if (tuiState->currentScreen == Screen::WELLCOME_SCREEN)
            {
                tuiState->currentScreen = Screen::HOME_SCREEN;
            }

            // Si estamos la pantalla de salida
            else if (tuiState->currentScreen == Screen::EXIT_SCREEN)
            {
                closeApplication();
            }

            break;

        // Escape
        case 27:
            // Salimos de la pantalla de test si presionamos dos veces escape
            if ((tuiState->currentScreen == Screen::TEST_SCREEN) &&
                     (tuiState->lastKey != key))
            {
                tuiState->currentScreen = Screen::TEST_SCREEN;
            }

            else
            {
                tuiState->currentScreen = Screen::HOME_SCREEN;
            }

            break;

        default:
            break;
    }

    // Actualizamos la tecla anterior
    tuiState->lastKey = key;

    // Actualizamos
    refreshScreen();
}

void TUIManager::welcomeScreen()
{
    // Mensajes
    QString buttonText = "-> Continuar <-";

    QString titulo = " Sistema de control de planta de aireacion ";
    QString descripcion = "Tesis de grado para recibir el titulo de Ingeniero en Mecatronica.";
    QString autores = "Autores: Gianluca Lovatto y Gabriel Aguirre.";
    QString universidad = "Facultad de Ciencias de la Alimentacion de la Universidad Nacional de Entre Rios.";

    QString qtVersion = QString::asprintf("Qt: %s", QT_VERSION_STR);
    QString glibcVersion = QString::asprintf("GLibc: %s", gnu_get_libc_version());
    QString ncursesVersion = QString::asprintf("NCurses: %s", NCURSES_VERSION);

    // Dialogo de bienvenida
    WINDOW *welcomeWindow = newwin(12, screenGetSize().width() - WINDOW_PADDING, 1, 3);

    box(welcomeWindow, 0, 0);

    wmove(welcomeWindow, 0, TEXT_CENTER(titulo));
    wprintw(welcomeWindow, titulo.toStdString().c_str());

    wmove(welcomeWindow, 1, 3);
    wprintw(welcomeWindow, descripcion.toStdString().c_str());

    wmove(welcomeWindow, 3, 3);
    wprintw(welcomeWindow, autores.toStdString().c_str());

    wmove(welcomeWindow, 4, 3);
    wprintw(welcomeWindow, universidad.toStdString().c_str());

    wmove(welcomeWindow, 6, 3);
    wprintw(welcomeWindow, qtVersion.toStdString().c_str());

    wmove(welcomeWindow, 7, 3);
    wprintw(welcomeWindow, glibcVersion.toStdString().c_str());

    wmove(welcomeWindow, 8, 3);
    wprintw(welcomeWindow, ncursesVersion.toStdString().c_str());

    wmove(welcomeWindow, 10, TEXT_CENTER(buttonText));
    wprintw(welcomeWindow, buttonText.toStdString().c_str());

    refresh();
    wrefresh(welcomeWindow);

    delwin(welcomeWindow);
}

void TUIManager::homeScreen()
{
    QString title = " Pantalla principal ";

    WINDOW *menuWindow = newwin(2 + homeScreenOptions->length(), screenGetSize().width() - WINDOW_PADDING, 1, 3);

    box(menuWindow, 0, 0);

    wmove(menuWindow, 0, TEXT_CENTER(title));
    wprintw(menuWindow, title.toStdString().c_str());

    for (int i = 0 ; i < homeScreenOptions->length() ; i++)
    {
        wmove(menuWindow, i + 1, 5);

        if (homeScreenOptions->at(i).screen == Screen::EXIT_SCREEN)
        {
            wattron(menuWindow, COLOR_PAIR(Color::ALERT_COLORS));
            wprintw(menuWindow, homeScreenOptions->at(i).text.toStdString().c_str());
            wattroff(menuWindow, COLOR_PAIR(Color::ALERT_COLORS));
        }

        else if (homeScreenOptions->at(i).screen == Screen::NO_IMPLEMENTED_SCREEN)
        {
            wattron(menuWindow, COLOR_PAIR(Color::WARNING_COLORS));
            wprintw(menuWindow, homeScreenOptions->at(i).text.toStdString().c_str());
            wattroff(menuWindow, COLOR_PAIR(Color::WARNING_COLORS));
            wprintw(menuWindow, " (Pantalla no implementada)");
        }

        else
        {
            wprintw(menuWindow, homeScreenOptions->at(i).text.toStdString().c_str());
        }
    }

    wmove(menuWindow, tuiState->homeScreenIndex + 1, 2);
    wprintw(menuWindow, "->");

    wrefresh(menuWindow);

    delwin(menuWindow);
}

void TUIManager::exitScreen()
{
    QString title = " Salir del programa ";

    QString buttonText = "-> SALIR <-";

    WINDOW *exitWindow = newwin(7, screenGetSize().width() - WINDOW_PADDING, 1, 3);

    box(exitWindow, 0, 0);

    wmove(exitWindow, 0, TEXT_CENTER(title));
    wprintw(exitWindow, title.toStdString().c_str());

    wmove(exitWindow, 1, 3);

    wattron(exitWindow, COLOR_PAIR(Color::ALERT_COLORS));
    wprintw(exitWindow, "ALERTA!!!");
    wattroff(exitWindow, COLOR_PAIR(Color::ALERT_COLORS));

    wprintw(exitWindow, " Esta por salir del programa");

    wmove(exitWindow, 2, 3);
    wprintw(exitWindow, "Esto causara que el control del proceso se detenga por completo.");

    wmove(exitWindow, 3, 3);
    wprintw(exitWindow, "Solo haga esto si esta realmente consiente de lo que hace.");

    wmove(exitWindow, 5, TEXT_CENTER(buttonText));
    wprintw(exitWindow, buttonText.toStdString().c_str());

    wrefresh(exitWindow);

    delwin(exitWindow);
}

void TUIManager::testScreen()
{
    QString title = " Testeo ";

    WINDOW *testWindow = newwin(3, screenGetSize().width() - WINDOW_PADDING, 1, 3);

    box(testWindow, 0, 0);

    wmove(testWindow, 0, TEXT_CENTER(title));
    wprintw(testWindow, title.toStdString().c_str());

    wmove(testWindow, 1, 3);
    wprintw(testWindow, "Codigo de tecla: %d", tuiState->lastKey);

    wrefresh(testWindow);

    delwin(testWindow);
}

void TUIManager::usersScreen()
{
    QList listUsers = HMIUsersManager::listUsers();

    QString title = " Usuarios ";

    WINDOW *usersWindow = newwin(2 + listUsers.length(), screenGetSize().width() - WINDOW_PADDING, 1, 3);

    box(usersWindow, 0, 0);

    wmove(usersWindow, 0, TEXT_CENTER(title));
    wprintw(usersWindow, title.toStdString().c_str());

    for (int i = 0 ; i < listUsers.length() ; i++)
    {
        wmove(usersWindow, i + 1, 3);
        wprintw(usersWindow,
                "Usuario: %s -- Contrasena: %s",
                listUsers.at(i).at(0).toStdString().c_str(),
                listUsers.at(i).at(1).toStdString().c_str());
    }

    wrefresh(usersWindow);

    delwin(usersWindow);
}

QSize TUIManager::screenGetSize()
{
    return QSize(getmaxx(stdscr), getmaxy(stdscr));
}

void TUIManager::initColors()
{
    start_color();

    // Iniciamos los colores
    init_pair(Color::ALERT_COLORS, COLOR_YELLOW, COLOR_RED);
    init_pair(Color::WARNING_COLORS, COLOR_BLUE, COLOR_YELLOW);
    init_pair(Color::INFO_COLORS, COLOR_YELLOW, COLOR_BLUE);
}

void TUIManager::closeApplication()
{
    emit closedApplication();
}
