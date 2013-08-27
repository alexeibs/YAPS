#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSystemTrayIcon>

class QToolBar;

class MainWindow : public QWidget { Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addActionIntoToolbar(QAction*);
    void setMainWidget(QWidget*);

public slots:
    void toggleWindow();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);

protected:
    void closeEvent(QCloseEvent* event) override;
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray&, void* message, long* result) override;
#endif

private:
    void createTrayIcon();

private:
    QAction* m_toggleAction;
    QMenu* m_trayMenu;
    QSystemTrayIcon* m_trayIcon;
    QToolBar* m_toolbar;
#ifdef Q_OS_WIN
    HWND m_lastForeground;
#endif
};

#endif // MAINWINDOW_H
