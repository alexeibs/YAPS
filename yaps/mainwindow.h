#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QWidget>
#include <QSystemTrayIcon>

#include "view_state.h"

class QListView;
class QAbstractItemModel;

namespace yaps {
    struct Controller;
    struct CryptoStatusView;
    struct ToolbarView;
}

class MainWindow : public QWidget, public yaps::ViewState {
  Q_OBJECT
public:
    explicit MainWindow(QAbstractItemModel* passwordsModel, std::shared_ptr<yaps::Controller>);
    ~MainWindow();

    // ViewState interface
    int currentRecordIndex() const override;
    void setCurrentRecordIndex(int index) override;
    void toggleMainWindow() override;

    yaps::CryptoStatusView* cryptoStatusView();

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
    QListView* m_passwordList;
    yaps::ToolbarView* m_toolbar;

    std::shared_ptr<yaps::Controller> m_controller;

#ifdef Q_OS_WIN
    HWND m_lastForeground;
#endif
};

#endif // MAINWINDOW_H
