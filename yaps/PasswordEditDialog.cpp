#include "PasswordEditDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "PasswordsModel.h"

PasswordEditDialog::PasswordEditDialog(QWidget* parent
    , const QString& title
    , PasswordRecord& record)
    : QDialog(parent)
    , m_record(record)
{
    m_name = new QLineEdit;
    m_name->setText(m_record.name);
    m_password = new QLineEdit;
    m_password->setText(m_record.password);
    m_password->setEchoMode(QLineEdit::Password);
    auto okButton = new QPushButton(tr("Ok"));
    auto cancelButton = new QPushButton(tr("Cancel"));
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(commit()));
    // layout
    auto formLayout = new QFormLayout;
    formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Name")));
    formLayout->setWidget(0, QFormLayout::FieldRole, m_name);
    formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Password")));
    formLayout->setWidget(1, QFormLayout::FieldRole, m_password);
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    setFixedHeight(sizeHint().height());
    setWindowTitle(title);
}

void PasswordEditDialog::setNameReadOnly(bool readOnly)
{
    m_name->setReadOnly(readOnly);
}

void PasswordEditDialog::accept()
{
    if (m_name->text().isEmpty() || m_password->text().isEmpty())
        return;
    QDialog::accept();
}

void PasswordEditDialog::commit()
{
    m_record.name = m_name->text();
    m_record.password = m_password->text();
}
