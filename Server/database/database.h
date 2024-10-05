#pragma once
#include <QObject>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include "../Logger/logger.h"
class Database : public QObject
{
    Q_OBJECT
  public:
    Database(QObject *parent = nullptr);
    ~Database();
    bool openDatabase();

  private:
    QSqlDatabase m_db;
    void log(QString msg,Logger::LogLevel level);
  private slots:
  signals:
};