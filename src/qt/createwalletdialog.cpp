// Copyright (c) 2019 The Bitcoin Core developers
// Copyright (c) 2021-2022 The Labyrinth Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include <config/labyrinth-config.h>
#endif

#include <qt/createwalletdialog.h>
#include <qt/forms/ui_createwalletdialog.h>

#include <QPushButton>

CreateWalletDialog::CreateWalletDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CreateWalletDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Create"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->wallet_name_line_edit->setFocus(Qt::ActiveWindowFocusReason);

    connect(ui->wallet_name_line_edit, &QLineEdit::textEdited, [this](const QString& text) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
    });

    connect(ui->disable_privkeys_checkbox, &QCheckBox::toggled, [this](bool checked) {
        // Wallets without private keys start out blank
        if (checked) {
            ui->blank_wallet_checkbox->setChecked(true);
        }
    });

    #ifndef USE_SQLITE
        ui->descriptor_checkbox->setToolTip(tr("Compiled without sqlite support (required for descriptor wallets)"));
        ui->descriptor_checkbox->setEnabled(false);
        ui->descriptor_checkbox->setChecked(false);
    #endif

}

CreateWalletDialog::~CreateWalletDialog()
{
    delete ui;
}

QString CreateWalletDialog::walletName() const
{
    return ui->wallet_name_line_edit->text();
}

bool CreateWalletDialog::isDisablePrivateKeysChecked() const
{
    return ui->disable_privkeys_checkbox->isChecked();
}

bool CreateWalletDialog::isMakeBlankWalletChecked() const
{
    return ui->blank_wallet_checkbox->isChecked();
}

bool CreateWalletDialog::isDescriptorWalletChecked() const
{
    return ui->descriptor_checkbox->isChecked();
}
