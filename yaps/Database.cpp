#include "Database.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QFile>
#include <QMessageBox>

#include "PasswordsModel.h"

#define MAX_SQL_LINE 16384

PasswordsModel* makeModel(QObject* parent)
{
    PasswordsModel* model = new PasswordsModel(parent);
    model->setQuery("SELECT * FROM passwords ORDER BY name");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Password"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    return model;
}

bool setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.isValid()) {
        QMessageBox::critical(0, QObject::tr("Error")
            , QObject::tr("Unable to load database driver."));
        return false;
    }
    db.setDatabaseName("yaps.db");
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
