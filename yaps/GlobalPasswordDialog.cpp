#include "GlobalPasswordDialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

GlobalPasswordDialog::GlobalPasswordDialog(QString& password)
    : m_password(password)
{
    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    auto okButton = new QPushButton(tr("OK"));
    auto cancelButton = new QPushButton(tr("Cancel"));
    okButton->setDefault(true);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(commit()));

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("Enter password"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(sizeHint());
}

void GlobalPasswordDialog::accept()
{
    if (m_passwordEdit->text().isEmpty())
        return;
    QDialog::accept();
}

void GlobalPasswordDialog::commit()
{
    m_password = m_passwordEdit->text();
    QString eraser;
    eraser.fill('?', m_password.size());
    m_passwordEdit->setText(eraser);
}
