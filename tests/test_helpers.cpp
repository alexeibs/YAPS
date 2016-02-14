#include "test_helpers.h"

#include <iostream>
#include <QString>

void PrintTo(const QChar& s, ::std::ostream* os) {
  *os << s.unicode();
}

void PrintTo(const QString& s, ::std::ostream* os) {
  *os << s.toStdString();
}
