#include "mainwindow.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QAction>
#include <QMenu>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QToolBar>

#ifdef Q_OS_WIN
#include "qt_windows.h"
#define TOGGLE_WINDOW_HOTKEY 0x0001
 // Ctrl + Shift + Q
#define TOGGLE_WINDOW_HOTKEY_MOD (MOD_CONTROL | MOD_SHIFT)
#define TOGGLE_WINDOW_HOTKEY_CODE (0x51)
#include "kbhook/winkbhook.h"
#endif

#include "MainActions.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    m_toolbar = new QToolBar;
    m_toolbar->setOrientation(Qt::Vertical);

    auto label = new QLabel("YAPS", this);
    auto layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(m_toolbar);
    layout->addWidget(label, 1, Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
    setWindowTitle("YAPS");
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    setMinimumSize(QSize(150, 200));
    createTrayIcon();

    Actions::instance().setMainWindow(this);
}

MainWindow::~MainWindow()
{
    Actions::instance().setMainWindow(nullptr);
#ifdef Q_OS_WIN
    removeKeyBoardHook();
    // register global hotkey
    UnregisterHotKey((HWND)winId(), TOGGLE_WINDOW_HOTKEY);
#endif
}

void MainWindow::addActionIntoToolbar(QAction* action)
{
    if (action)
        m_toolbar->addAction(action);
    else
        m_toolbar->addSeparator();
}

void MainWindow::setMainWidget(QWidget* widget)
{
    auto mainLayout = dynamic_cast<QBoxLayout*>(layout());
    auto oldItem = mainLayout->itemAt(mainLayout->count() - 1);
    auto oldWidget = oldItem->widget();
    mainLayout->removeItem(oldItem);
    mainLayout->addWidget(widget, 1);
    delete oldWidget;
}

void MainWindow::createTrayIcon()
{
    m_toggleAction = new QAction(tr("&Hide"), this);
    m_toggleAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Q));
    auto quitAction = new QAction(tr("&Quit"), this);
    connect(m_toggleAction, SIGNAL(triggered()), this, SLOT(toggleWindow()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_trayMenu = new QMenu(this);
    m_trayMenu->addAction(m_toggleAction);
    m_trayMenu->addAction(quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->setIcon(windowIcon());
    m_trayIcon->setToolTip(windowTitle());

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    m_trayIcon->show();

#ifdef Q_OS_WIN
    // register global hotkey
    auto hwnd = (HWND)winId();
    RegisterHotKey(hwnd, TOGGLE_WINDOW_HOTKEY
        , TOGGLE_WINDOW_HOTKEY_MOD, TOGGLE_WINDOW_HOTKEY_CODE);
    setupKeyBoardHook(hwnd);
#endif
}

void MainWindow::toggleWindow()
{
    if (isVisible()) {
        hide();
        m_toggleAction->setText(tr("&Show"));
#ifdef Q_OS_WIN
        if (m_lastForeground) {
            if (IsIconic(m_lastForeground))
                ::ShowWindow(m_lastForeground, SW_RESTORE);
            ::SetForegroundWindow(m_lastForeground);
        }
#endif
    } else {
#ifdef Q_OS_WIN
        m_lastForeground = ::GetForegroundWindow();
        ::SetForegroundWindow((HWND)winId());
#endif
        showNormal();
        m_toggleAction->setText(tr("&Hide"));
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
        toggleWindow();
}

#ifndef QT_NO_DEBUG
void MainWindow::closeEvent(QCloseEvent*) {}
#else
void MainWindow::closeEvent(QCloseEvent* event)
{
    toggleWindow();
    event->ignore();
}
#endif

#ifdef Q_OS_WIN
bool MainWindow::nativeEvent(const QByteArray&, void *message, long *result)
{
    auto msg = reinterpret_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY && msg->wParam == TOGGLE_WINDOW_HOTKEY) {
        toggleWindow();
        *result = 0;
        return true;
    }
    if (msg->message == WM_CLIPBOARD_PASTE)
        Actions::instance().clipboardPasted();
    return false;
}
#endif
