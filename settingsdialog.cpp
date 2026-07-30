#include "settingsdialog.h"




SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Settings");
    resize(350, 200);

    auto layout = new QVBoxLayout(this);
    qDebug()<<layout;

    // Header
    header = new QLabel("⚙️ Application Settings", this);
    header->setStyleSheet("font-size: 16px; font-weight: bold; color: navy;");
    layout->addWidget(header);

    // Options
    darkModeCheck = new QCheckBox("Enable Dark Mode", this);
    notificationsCheck = new QCheckBox("Enable Notifications", this);

    // OK button
    okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(darkModeCheck);
    layout->addWidget(notificationsCheck);
    layout->addStretch();
    layout->addWidget(okButton);
}
