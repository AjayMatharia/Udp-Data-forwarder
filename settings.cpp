#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    _stationPort = new QVector<QString>();
    _stationName = new QVector<QString>();
    _recieverIp = new QVector<QString>();
    _recieverName = new QVector<QString>();
    _recieverPorts = new QVector<QPushButton *>();


    // QList temp = {"a","b","c","d"};
    // qDebug()<<temp;
    // temp.removeAt(1);
    // qDebug()<<temp;

    QObject :: connect(ui->_des_pushButton_station_info_addStation ,&QPushButton::clicked, this, &Settings :: addIp);
    QObject :: connect(ui->_des_pushButton_station_info_removeStation ,&QPushButton::clicked, this, &Settings :: removeIp);
    QObject :: connect(ui->_des_pushButton_reciever_info_addReciever ,&QPushButton::clicked, this, &Settings :: addIp);
    QObject :: connect(ui->_des_pushButton_reciever_info_removeReciever ,&QPushButton::clicked, this, &Settings :: removeIp);
    QObject :: connect(ui->_des_pushButton_save ,&QPushButton::clicked, this, &Settings :: saveSettings);



}

Settings::~Settings()
{
    delete ui;
}

void Settings::addIp(){

    QObject* signalSender = sender();
    if (signalSender == ui->_des_pushButton_station_info_addStation) {
        // 1. Get the currently selected row index
        int currentRow = ui->_des_tableWidget_station_info->currentRow();

        // 2. Determine the insertion target index
        // If no row is selected (currentRow is -1), append to the end of the table
        int targetRow = (currentRow != -1) ? (currentRow + 1) : ui->_des_tableWidget_station_info->rowCount();

        // 3. Insert the blank row
        ui->_des_tableWidget_station_info->insertRow(targetRow);


        QTableWidgetItem *_tableWidgetItemstationname = new QTableWidgetItem("Station Name");
        QTableWidgetItem *_tableWidgetItemstationport = new QTableWidgetItem("0");
        _tableWidgetItemstationname->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemstationport->setTextAlignment(Qt::AlignCenter);
        ui->_des_tableWidget_station_info->setItem(targetRow, 0, _tableWidgetItemstationname);
        ui->_des_tableWidget_station_info->setItem(targetRow, 1, _tableWidgetItemstationport);

    } else if (signalSender == ui->_des_pushButton_reciever_info_addReciever) {
        // 1. Get the currently selected row index
        int currentRow = ui->_des_tableWidget_reciever_info->currentRow();

        // 2. Determine the insertion target index
        // If no row is selected (currentRow is -1), append to the end of the table
        int targetRow = (currentRow != -1) ? (currentRow + 1) : ui->_des_tableWidget_reciever_info->rowCount();

        // 3. Insert the blank row
        ui->_des_tableWidget_reciever_info->insertRow(targetRow);

        // ui->_des_tableWidget_station_info->insertRow(ui->_des_tableWidget_station_info->rowCount());
        // ui->_des_tableWidget_station_info->setRowCount(ui->_des_tableWidget_station_info->rowCount()+1);
        QTableWidgetItem *_tableWidgetItemrecievername = new QTableWidgetItem("Reciever Name");
        QTableWidgetItem *_tableWidgetItemrecieverip = new QTableWidgetItem("0.0.0.0");
        QPushButton *temp = new QPushButton("Ports");
        QObject :: connect(temp ,&QPushButton::clicked, this, &Settings :: readSettings);
        _recieverPorts->insert(targetRow,temp);
        qDebug()<<*_recieverPorts;
        _tableWidgetItemrecievername->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemrecieverip->setTextAlignment(Qt::AlignCenter);
        ui->_des_tableWidget_reciever_info->setItem(targetRow, 0, _tableWidgetItemrecievername);
        ui->_des_tableWidget_reciever_info->setItem(targetRow, 1, _tableWidgetItemrecieverip);
        ui->_des_tableWidget_reciever_info->setCellWidget(targetRow, 2,temp);
    }


}
void Settings::removeIp(){
    QObject* signalSender = sender();
    if (signalSender == ui->_des_pushButton_station_info_removeStation) {
        int currentRow = ui->_des_tableWidget_station_info->currentRow();
        if (currentRow >= 0) {
            ui->_des_tableWidget_station_info->removeRow(currentRow);
        }
    } else if (signalSender == ui->_des_pushButton_reciever_info_removeReciever) {
        int currentRow = ui->_des_tableWidget_reciever_info->currentRow();
        if (currentRow >= 0) {
            ui->_des_tableWidget_reciever_info->removeRow(currentRow);
            qDebug()<<"deleted:"<<_recieverPorts->at(currentRow);
            qDebug()<<"_currentRow:"<<currentRow;

            // delete _recieverPorts->at(currentRow);
            QPushButton *temp =  _recieverPorts->at(currentRow);
            qDebug()<<temp;
            _recieverPorts->removeAt(currentRow);
            // qDebug()<<temp;
            delete temp;
            qDebug()<<*_recieverPorts;
        }
    }

}
void Settings::saveSettings(){

    _stationName->clear();
    _stationPort->clear();
    _recieverName->clear();
    _recieverIp->clear();
    // Get row and column counts

    int rows = ui->_des_tableWidget_station_info->rowCount();
    int rows2 = ui->_des_tableWidget_reciever_info->rowCount();
    // int cols = ui->_des_tableWidget_station_info->columnCount();

    // Loop through every row

    for (int r = 0; r < rows; ++r) {
        _stationName->append(ui->_des_tableWidget_station_info->item(r, 0)->text());
        _stationPort->append(ui->_des_tableWidget_station_info->item(r, 1)->text());

        // qDebug()<<_stationPort;
    }
    for (int r = 0; r < rows2; ++r) {
        _recieverName->append(ui->_des_tableWidget_reciever_info->item(r, 0)->text());
        _recieverIp->append(ui->_des_tableWidget_reciever_info->item(r, 1)->text());

        // qDebug()<<_stationPort;
    }
    // qDebug()<<*_stationName;


    // 1. Specify the custom INI path and set the format

    QString configPath = QCoreApplication::applicationDirPath() + "/Settings/config.ini";
    qDebug()<<configPath;
    QSettings settings(configPath, QSettings::IniFormat);
    settings.clear();


      //Saving Station Info
    settings.beginWriteArray("Stations_info");


    for (int r = 0; r < rows; ++r) {
        settings.setArrayIndex(r);
        settings.setValue("Station_Name: ", _stationName->at(r));
        settings.setValue("Port: ", _stationPort->at(r));

    }

    // 3. Save values inside the group

    // 4. Close the group
    settings.endArray();


    //Saving Reviever Info

    settings.beginWriteArray("Reciever_info");


    for (int r = 0; r < rows2; ++r) {
        settings.setArrayIndex(r);
        settings.setValue("Reciever_Name: ", _recieverName->at(r));
        settings.setValue("IP_Address: ", _recieverIp->at(r));

    }

    // 3. Save values inside the group

    // 4. Close the group
    settings.endArray();




}
void Settings::readSettings(){
    qDebug()<<"Hi";
    // QSettings settings("config.ini", QSettings::IniFormat);


}

