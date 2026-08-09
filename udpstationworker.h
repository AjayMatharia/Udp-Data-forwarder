#ifndef UDPSTATIONWORKER_H
#define UDPSTATIONWORKER_H

#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include <QDebug>
#include <QMutex>

class UdpStationWorker : public QObject {
    Q_OBJECT
private:
    QString m_stationName;
    quint16 m_port;
    QUdpSocket *m_socket;
    // Holds a list of (IP Address, Port) for the receivers linked to this station
    QList<QPair<QString, quint16>> m_forwardingTargets;
    mutable QMutex m_targetsMutex;

public:
    // UdpStationWorker(const QString &name, quint16 port, QObject *parent = nullptr);
    // Update constructor to take targets right away:
    UdpStationWorker(const QString &name, quint16 port,
                     const QList<QPair<QString, quint16>> &initialTargets,
                     QObject *parent = nullptr);

    // Setter to tell this worker exactly who to forward data to

public slots:
    void startListening(); // Handles thread-safe socket setup
    void stopListening();  // Safely shuts down the socket
    void setForwardingTargets(const QList<QPair<QString, quint16>> &targets);

private slots:
    void handleReadyRead(); // Ingests incoming UDP datagram packets

signals:
    // Pushes extracted string back across threads to the UI layer
    void datagramReceived(const QString &stationName, const QString &textData, const QString &senderIp,int packetSize );
    void finished();
};

#endif // UDPSTATIONWORKER_H
