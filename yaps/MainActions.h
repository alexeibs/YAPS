#ifndef MAINACTIONS_H
#define MAINACTIONS_H

#include <QObject>

class QString;
class QAction;
class QIcon;
class QAbstractItemModel;
class QAbstractItemView;
class MainWindow;
class PasswordsModel;
class QListView;

class Actions : public QObject { Q_OBJECT
public:
    static Actions& instance();

    void initialize();
    void setMainWindow(MainWindow*); //automatically called by MainWindow

    QAbstractItemModel* model();
    QAbstractItemView* view();

public slots:
    void copyToClipboard();
    void clipboardPasted();
    void addPassword();
    void editPassword();
    void deletePassword();

private:
    Actions();
    Actions(const Actions&) = delete;
    Actions(Actions&&) = delete;
    void operator=(const Actions&) = delete;
    void operator=(Actions&&) = delete;

    QAction* createAction(const QString& name, const QIcon& icon);

private:
    MainWindow* m_mainWindow;
    PasswordsModel* m_model;
    QListView* m_view;
};

#endif // MAINACTIONS_H
