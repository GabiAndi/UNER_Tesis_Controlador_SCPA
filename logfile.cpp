#include "logfile.h"

LogFile::LogFile(QObject *parent) : QObject(parent)
{

}

LogFile::~LogFile()
{
    close();
}

bool LogFile::create(const QString &fileName)
{
    // Se crea la carpeta en donde ira el archivo
    if (!QDir(logSubdir).exists())
    {
        QDir().mkdir(logSubdir);
    }

    file.setFileName(logSubdir + "/" + fileName + ".log");

    // Si el archivo existe se elimina
    if (file.exists())
    {
        file.remove();
    }

    // Se abre el archivo para añadir contenido al final
    return file.open(QIODevice::OpenModeFlag::ReadWrite | QIODevice::OpenModeFlag::Append);
}

void LogFile::close()
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
