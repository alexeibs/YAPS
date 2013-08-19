#include "mainwindow.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QApplication>

#ifdef Q_OS_WIN
#include "qt_windows.h"
#define TOGGLE_WINDOW_HOTKEY 0x0001
 // Ctrl + Shift + Y
#define TOGGLE_WINDOW_HOTKEY_MOD (MOD_CONTROL | MOD_SHIFT)
#define TOGGLE_WINDOW_HOTKEY_CODE (0x59)
#endif

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    auto label = new QLabel("YAPS", this);
    auto layout = new QVBoxLayout;
    layout->addWidget(label, 0, Qt::AlignCenter);
    setLayout(layout);
    setWindowTitle("YAPS");
    setWindowFlags(Qt::Window);
    resize(QSize(800, 600));
    createTrayIcon();
}

MainWindow::~MainWindow()
{
#ifdef Q_OS_WIN
    // register global hotkey
    UnregisterHotKey((HWND)winId(), TOGGLE_WINDOW_HOTKEY);
#endif
}

void MainWindow::createTrayIcon()
{
    m_toggleAction = new QAction(tr("&Hide"), this);
    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_toggleAction, SIGNAL(triggered()), this, SLOT(toggleWindow()));
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_trayMenu = new QMenu(this);
    m_trayMenu->addAction(m_toggleAction);
    m_trayMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->setIcon(windowIcon());
    m_trayIcon->setToolTip(windowTitle());

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    m_trayIcon->show();

#ifdef Q_OS_WIN
    // register global hotkey
    RegisterHotKey((HWND)winId(), TOGGLE_WINDOW_HOTKEY
        , TOGGLE_WINDOW_HOTKEY_MOD, TOGGLE_WINDOW_HOTKEY_CODE);
#endif
}

void MainWindow::toggleWindow()
{
    if (isVisible()) {
        hide();
        m_toggleAction->setText(tr("&Show"));
    } else {
        showNormal();
        m_toggleAction->setText(tr("&Hide"));
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
        toggleWindow();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    toggleWindow();
    event->ignore();
}

#ifdef Q_OS_WIN
bool MainWindow::nativeEvent(const QByteArray&, void *message, long *result)
{
    auto msg = reinterpret_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY && msg->wParam == TOGGLE_WINDOW_HOTKEY)
    {
        toggleWindow();
        *result = 0;
        return true;
    }
    return false;
}
#endif
