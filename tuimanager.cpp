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
    delete tuiState;
    delete menuCommands;

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

    consoleInitColors();

    // Consola
    tuiState = new tui_state_t;

    tuiState->menuIndex = 0;

    // Iniciamos la captura de la consola
    consoleThread = new QThread(this);
    consoleListener = new ConsoleListener();

    consoleListener->moveToThread(consoleThread);

    connect(consoleThread, &QThread::started, consoleListener, &ConsoleListener::init);

    connect(consoleListener, &ConsoleListener::newKey, this, &TUIManager::consoleNewKey);

    consoleThread->start();

    // Pantalla de bienvenida
    consoleWelcome();

    // Comandos
    menuCommands = new QList<tui_menu_command_t>;

    menuCommands->append(tui_menu_command_t {"Ver informacion de HMI server", ConsoleCommand::HMI_SERVER_INFO});
    menuCommands->append(tui_menu_command_t {"Salir", ConsoleCommand::EXIT});

    // Mensaje de inicio
    logFile->println("Cargado");
}

void TUIManager::hmiServerStatus(hmi_server_status_t status)
{
    /**consoleOutput << "IP servidor: " << status.serverIP << Qt::endl;
    *consoleOutput << "IP cliente: " << status.clientIP << Qt::endl;
    *consoleOutput << "Puerto usado: " << status.port << Qt::endl;

    consoleWait();*/
}

void TUIManager::consoleNewKey(const int key)
{
    // Captura de tecla
    switch (key)
    {
        // Flecha hacia abajo
        case 258:
            tuiState->menuIndex++;

            if (tuiState->menuIndex >= menuCommands->length())
                tuiState->menuIndex = 0;

            break;

        // Flecha hacia arriba
        case 259:
            tuiState->menuIndex--;

            if (tuiState->menuIndex < 0)
                tuiState->menuIndex = menuCommands->length() - 1;

            break;

        // Es un enter
        case 10:
            executeCommand(menuCommands->at(tuiState->menuIndex).id);

            break;

        default:
            break;
    }

    /*// Comando para limpiar la pantalla
    else if (cmd == "clear")
    {
        consoleClear();
        consoleWelcome();
        consoleWait();
    }

    // Comando para consultar el estado del servidor HMI
    else if (cmd == "hmi status")
    {
        emit getHmiServerStatus();
    }

    // Comando no conocido
    else
    {
        *consoleOutput << "Comando no valido para: " << line << Qt::endl;
        consoleWait();
    }*/

    // Borramos todo
    clear();

    // Ventana
    refresh();

    // Ventana de menu
    consoleMenu();

    // Utilizado para ver el codigo de tecla
    //consolePrintKeyboardCode(key);
}

void TUIManager::consoleWelcome()
{
    // Mensajes
    QString titulo = " Sistema de control de planta de aireacion ";
    QString descripcion = "Tesis de grado para recibir el titulo de Ingeniero en Mecatronica.";
    QString autores = "Autores: Gianluca Lovatto y Gabriel Aguirre.";
    QString universidad = "Facultad de Ciencias de la Alimentacion de la Universidad Nacional de Entre Rios.";

    QString qtVersion = QString::asprintf("Qt: %s", QT_VERSION_STR);
    QString glibcVersion = QString::asprintf("GLibc: %s", gnu_get_libc_version());
    QString ncursesVersion = QString::asprintf("NCurses: %s", NCURSES_VERSION);

    // Dialogo de bienvenida
    WINDOW *welcomeWindow = newwin(10, screenGetSize().width() - 6, 1, 3);

    box(welcomeWindow, 0, 0);

    wmove(welcomeWindow, 0, ((screenGetSize().width() - 8 - titulo.length()) / 2));
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

    move(12, 0);
    printw("Precione una tecla para continuar...");

    refresh();
    wrefresh(welcomeWindow);

    delwin(welcomeWindow);
}

void TUIManager::consoleMenu()
{
    QString menuTitle = " Menu de comandos ";

    WINDOW *menuWindow = newwin(10, screenGetSize().width() - 6, 1, 3);

    box(menuWindow, 0, 0);

    wmove(menuWindow, 0, (screenGetSize().width() - menuTitle.length()) / 2);
    wprintw(menuWindow, menuTitle.toStdString().c_str());

    for (int i = 0 ; i < menuCommands->length() ; i++)
    {
        wmove(menuWindow, i + 1, 5);

        if (menuCommands->at(i).id == ConsoleCommand::EXIT)
        {
            wattron(menuWindow, COLOR_PAIR(ConsoleColor::ALERT));
            wprintw(menuWindow, menuCommands->at(i).text.toStdString().c_str());
            wattroff(menuWindow, COLOR_PAIR(ConsoleColor::ALERT));
        }

        else
        {
            wprintw(menuWindow, menuCommands->at(i).text.toStdString().c_str());
        }
    }

    wmove(menuWindow, tuiState->menuIndex + 1, 2);
    wprintw(menuWindow, "->");

    wrefresh(menuWindow);

    delwin(menuWindow);
}

void TUIManager::consolePrintKeyboardCode(int key)
{
    clear();
    refresh();

    QString menuTitle = " Testeo ";

    WINDOW *testWindow = newwin(3, screenGetSize().width() - 6, 1, 3);

    box(testWindow, 0, 0);

    wmove(testWindow, 0, (screenGetSize().width() - menuTitle.length()) / 2);
    wprintw(testWindow, menuTitle.toStdString().c_str());

    wmove(testWindow, 1, 3);

    wprintw(testWindow, "Codigo de tecla: %d", key);

    wrefresh(testWindow);

    delwin(testWindow);
}

QSize TUIManager::screenGetSize(WINDOW *win)
{
    return QSize(getmaxx(win), getmaxy(win));
}

void TUIManager::consoleInitColors()
{
    start_color();

    // Iniciamos los colores
    init_pair(ConsoleColor::ALERT, COLOR_YELLOW, COLOR_RED);
    init_pair(ConsoleColor::WARNING, COLOR_GREEN, COLOR_YELLOW);
    init_pair(ConsoleColor::INFO, COLOR_YELLOW, COLOR_BLUE);
}

void TUIManager::executeCommand(const ConsoleCommand id)
{
    switch (id)
    {
        case TUIManager::ConsoleCommand::EXIT:
            emit closedApplication();

            break;

        case TUIManager::ConsoleCommand::HMI_SERVER_INFO:
            break;

        default:
            break;
    }
}

void TUIManager::closeApplication()
{
    endwin();

    emit closedApplication();
}
