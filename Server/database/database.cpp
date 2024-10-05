#include "database.h"
#include <qsqlerror.h>
Database::Database(QObject *parent)
    : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QODBC");
    // m_db.setDatabaseName("Driver={ODBC Driver 17 for SQL Server};Server=haohao.iok.la,23693;Database=DBChat;UID=sa;PWD=123456;");
    m_db.setDatabaseName("Driver={ODBC Driver 17 for SQL Server};Server=127.0.0.1,1433;Database=DBChat;UID=sa;PWD=123456;");
}

Database::~Database()
{
    m_db.close();
}

bool Database::openDatabase()
{
    bool isOpen = m_db.open();
    if (!isOpen)
        return false;
    else
        return true;
}

void Database::log(QString msg, Logger::LogLevel level)
{
    Logger::getInstance().log(msg, level);
}
