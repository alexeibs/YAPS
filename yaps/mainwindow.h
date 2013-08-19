#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSystemTrayIcon>

class MainWindow : public QWidget { Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    QAction* m_quitAction;
    QMenu* m_trayMenu;
    QSystemTrayIcon* m_trayIcon;
};

#endif // MAINWINDOW_H
