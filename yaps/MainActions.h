#ifndef MAINACTIONS_H
#define MAINACTIONS_H

#include <memory>

#include <QObject>

#include "crypto_status.h"

class QString;
class QAction;
class QIcon;
class QAbstractItemModel;
class QAbstractItemView;
class MainWindow;
class PasswordsModel;
class QListView;

namespace yaps {
    struct Crypto;
    struct CryptoFactory;
}

class Actions : public QObject, public yaps::CryptoStatusView { Q_OBJECT
public:
    static std::shared_ptr<Actions> instance();

    void initialize();
    void setMainWindow(MainWindow*); //automatically called by MainWindow

    QAbstractItemModel* model();
    QAbstractItemView* view();

    void setCryptoFactory(std::weak_ptr<yaps::CryptoFactory>);
    void setCryptoStatus(std::weak_ptr<yaps::CryptoStatus>);
    void updateCryptoStatus() override;

public slots:
    void copyToClipboard();
    void copyPasswordToClipboard();
    void clipboardPasted();

    void addPassword();
    void editPassword();
    void deletePassword();

    void clearGlobalPassword();

private:
    Actions();
    Actions(const Actions&) = delete;
    Actions(Actions&&) = delete;
    void operator=(const Actions&) = delete;
    void operator=(Actions&&) = delete;

    QAction* createAction(const QString& name, const QIcon& icon);

    std::unique_ptr<yaps::Crypto> getCrypto();

private:
    MainWindow* m_mainWindow;
    PasswordsModel* m_model;
    QListView* m_view;
    QAction* m_expireAction;
    std::weak_ptr<yaps::CryptoFactory> m_cryptoFactory;
    std::weak_ptr<yaps::CryptoStatus> m_cryptoStatus;
};

#endif // MAINACTIONS_H
