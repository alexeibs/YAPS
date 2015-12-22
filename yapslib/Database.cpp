#include "Database.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QFile>
#include <QMessageBox>

#define MAX_SQL_LINE 16384

bool setupDatabase(const QString& databasePath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.isValid()) {
        QMessageBox::critical(0, QObject::tr("Error")
            , QObject::tr("Unable to load database driver."));
        return false;
    }

    db.setDatabaseName(databasePath);
    if (!db.open()) {
        QMessageBox::critical(0, QObject::tr("Error")
            , QObject::tr("Unable to open database."));
        return false;
    }
    if (db.tables().empty()) {
        db.transaction();
        try {
            QString error;
            QFile sqlFile(":/misc/init.sql");
            sqlFile.open(QIODevice::ReadOnly);
            while (!sqlFile.atEnd()) {
                auto line = QString::fromUtf8(sqlFile.readLine(MAX_SQL_LINE));
                auto sqlQuery = db.exec(line);
                if (sqlQuery.lastError().type() != QSqlError::NoError) {
                    error = sqlQuery.lastError().text();
                    break;
                }
            }
            if (error.isEmpty()) {
                db.commit();
            } else {
                db.rollback();
                QMessageBox::critical(0
                    , QObject::tr("Recovering database"), error);
                return false;
            }
        } catch (...) {
            db.rollback();
        }
    }
    return true;
}
