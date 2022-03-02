#include "logfile.h"

LogFile::LogFile(QObject *parent)
    : QObject{parent}
{
    // Se crea la carpeta en donde ira el archivo
    if (!QDir(logSubdir).exists())
    {
        QDir().mkdir(logSubdir);
    }

    file.setFileName(QDir::currentPath() + "/" + logSubdir + "/" + parent->objectName() + ".log");

    // Se abre el archivo para añadir contenido al final
    file.open(QIODevice::OpenModeFlag::ReadWrite | QIODevice::OpenModeFlag::Truncate);
}

LogFile::~LogFile()
{
    if (file.isOpen())
    {
        file.flush();
        file.close();
    }
}

QString LogFile::getTimeLog()
{
    return QDate::currentDate().toString("yyyy-MM-dd") +
            "-" +
            QTime::currentTime().toString("hh-mm-ss-zzz");
}

bool LogFile::print(const QString &data)
{
    file.write(QString(getTimeLog() + ": " + data).toLatin1());

    return file.flush();
}

bool LogFile::println(const QString &data)
{
    file.write(QString(getTimeLog() + ": " + data + "\r\n").toLatin1());

    return file.flush();
}
