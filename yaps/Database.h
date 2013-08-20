#ifndef DATABASE_H
#define DATABASE_H

class QObject;
class PasswordsModel;

PasswordsModel* makeModel(QObject* parent);

bool setupDatabase();

#endif // DATABASE_H
