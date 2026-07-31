#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVector2D>
#include <QVector>
#include <QMap>
#include <QSettings>



namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    //2D vector
    QVector2D *_ports;
    QVector<QString> *_stationPort;
    QVector<QString> *_stationName;
    QVector<QString> *_recieverIp;
    QVector<QString> *_recieverName;
    QVector<QPushButton *> *_recieverPorts;


    QMap<QString, QString> _stationIpMap;

public slots:
    void readSettings();
    void saveSettings();
    void addIp();
    void removeIp();


};

#endif // SETTINGS_H
