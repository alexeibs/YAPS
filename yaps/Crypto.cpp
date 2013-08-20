#include "Crypto.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-fpermissive"

Crypto& Crypto::instance()
{
    static Crypto single;
    return single;
}

Crypto::Crypto()
{
}

void Crypto::encrypt(QString& input, QString& output)
{
    output = input;
    erase(input);
}

void Crypto::decrypt(const QString& input, QString& output)
{
    output = input;
}

void Crypto::erase(QString& stringToErase)
{
    stringToErase.fill('?');
}

#pragma GCC diagnostic pop
