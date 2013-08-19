#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QApplication::setWindowIcon(QIcon(":/icons/app.png"));

    MainWindow mainWindow;
    mainWindow.toggleWindow();

    return application.exec();
}
