#include <QCoreApplication>

#include <gmock/gmock.h>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  app.setAttribute(Qt::AA_Use96Dpi, true);

  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}
