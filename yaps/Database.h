#ifndef DATABASE_H
#define DATABASE_H

class QString;
class QObject;
class PasswordsModel;

PasswordsModel* makeModel(QObject* parent);

bool setupDatabase(const QString& databasePath);

#endif // DATABASE_H
