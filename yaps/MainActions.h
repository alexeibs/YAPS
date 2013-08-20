#ifndef MAINACTIONS_H
#define MAINACTIONS_H

class QString;
class QAction;
class QIcon;
class MainWindow;

class Actions {
public:
    static Actions& instance();
    QAction* addAction(const QString& name, const QIcon& icon);

    void setMainWindow(MainWindow*); //automatically called by MainWindow
private:
    Actions();
    Actions(const Actions&) = delete;
    Actions(Actions&&) = delete;
    void operator=(const Actions&) = delete;
    void operator=(Actions&&) = delete;

private:
    MainWindow* m_mainWindow;
};

#endif // MAINACTIONS_H
