#include "udpstationworker.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker> // FIX: Add this include for automatic lock management

UdpStationWorker::UdpStationWorker(const QString &name, quint16 port,
                                   const QList<QPair<QString, quint16>> &initialTargets,
                                   QObject *parent)
    : QObject(parent)
    , m_stationName(name)
    , m_port(port)
    , m_socket(nullptr)
    , m_forwardingTargets(initialTargets)
{
    qDebug() << QString("[UDP Worker] Initialized for %1 on Port %2 with %3 active forwarding paths.")
    .arg(m_stationName).arg(m_port).arg(m_forwardingTargets.size());
}

void UdpStationWorker::setForwardingTargets(const QList<QPair<QString, quint16>> &targets) {
    // FIX: Automatically lock the mutex while writing new targets
    QMutexLocker locker(&m_targetsMutex);
    m_forwardingTargets = targets;
}

void UdpStationWorker::startListening() {
    m_socket = new QUdpSocket(this);
    bool success = m_socket->bind(QHostAddress::Any, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    if (success) {
        connect(m_socket, &QUdpSocket::readyRead, this, &UdpStationWorker::handleReadyRead);
        qDebug() << QString("[UDP Thread] Ingestion + Forwarding Active for %1 on Port %2").arg(m_stationName).arg(m_port);
    } else {
        qCritical() << QString("[UDP Thread] FAILED to bind Port %1").arg(m_port);
        emit finished();
    }
}

void UdpStationWorker::handleReadyRead() {
    while (m_socket && m_socket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(static_cast<int>(m_socket->pendingDatagramSize()));

        QHostAddress senderIp;
        quint16 senderPort;
        m_socket->readDatagram(buffer.data(), buffer.size(), &senderIp, &senderPort);

        // FIX: Safely make a local copy of the targets while the mutex is briefly locked
        QList<QPair<QString, quint16>> localTargetsCopy;
        {
            QMutexLocker locker(&m_targetsMutex);
            localTargetsCopy = m_forwardingTargets;
        } // The lock automatically opens here, letting setForwardingTargets execute instantly if needed

        // Forward the exact raw data bytes out to your safe local copy
        for (const auto &target : localTargetsCopy) {
            QHostAddress targetIp(target.first);
            quint16 targetPort = target.second;

            if (!targetIp.isNull() && targetPort > 0) {
                m_socket->writeDatagram(buffer, targetIp, targetPort);
            }
        }

        QString receivedText = QString::fromUtf8(buffer).trimmed();
        QString cleanIp = senderIp.toString().remove("::ffff:");
        emit datagramReceived(m_stationName, receivedText, cleanIp, buffer.size());
    }
}

void UdpStationWorker::stopListening() {
    if (m_socket) {
        m_socket->close();
    }
    emit finished();
}
