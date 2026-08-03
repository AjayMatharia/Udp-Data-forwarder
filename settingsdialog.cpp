#include "settingsdialog.h"




SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {


    setWindowTitle("Settings");
    resize(350, 500);

    _configPath = QCoreApplication::applicationDirPath() + "/Settings/configPort.ini";

    //Layout
    auto layout = new QVBoxLayout(this);

    //Table
    _portTable = new QTableWidget(this);

    _portTable->setColumnCount(1);
    // _portTable->setRowCount(10);
    _portTable->horizontalHeader()->setDefaultSectionSize(150);
    _portTable->setHorizontalHeaderItem(0,new QTableWidgetItem("PORTS"));

    // Header
    header = new QLabel("⚙️ Ports Settings", this);
    header->setStyleSheet("font-size: 16px; font-weight: bold; color: navy;");
    layout->addWidget(header);

    // Options
    // darkModeCheck = new QCheckBox("Enable Dark Mode", this);
    // notificationsCheck = new QCheckBox("Enable Notifications", this);

    // OK button
    okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::saveSettings);

    layout->addWidget(_portTable);
    // layout->addWidget(notificationsCheck);
    // layout->addStretch();
    layout->addWidget(okButton);
}
void SettingsDialog::readSettings(){
    QSettings settings(_configPath , QSettings::IniFormat);

    // for (int i = 0; var < total; ++var) {
    //     int size = settings.beginReadArray("Ports");

    // }

}

void SettingsDialog::saveSettings(){
    ports2.clear();
    for (int i = 0; i < _portTable->rowCount(); ++i) {
        ports2.append(_portTable->item(i,0)->text());
    }

    emit sendData(ports2);
    this->close();
}
void SettingsDialog::setData(QVector<QString> ports){
    // qDebug()<<ports;
    _portTable->setRowCount(ports.size());
    for (int i = 0; i < ports.size(); ++i) {
        // QTableWidgetItem *portitem = new QTableWidgetItem(ports->at(i));

        QTableWidgetItem *item = new QTableWidgetItem(ports.at(i));
        item->setTextAlignment(Qt::AlignCenter);
        _portTable->setItem(i, 0, item);

    }
}
