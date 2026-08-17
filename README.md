# UDP Data Forwarder & Packet Relay Station

A high-performance, cross-platform UDP network tool built with **C++** and the **Qt 6 Framework**. This utility acts as a specialized data pipeline—efficiently intercepting incoming network datagrams from local ports and seamlessly routing them to a designated target receiver with zero processing overhead.

## 🚀 Key Features

*   **Non-Blocking Architecture:** Offloads socket network polling to a dedicated background worker (`UdpStationWorker`) via a `QThread` pool to guarantee the Main GUI thread remains fluid and responsive.
*   **Intelligent Port Sharing:** Explicitly combines `QUdpSocket::ShareAddress` and `QUdpSocket::ReuseAddressHint` system socket flags. This allows multiple local instances of the application to cleanly bind to the exact same broadcast port on a machine without encountering "Address Already In Use" binding errors.
*   **Modern, Dark-Themed UI:** Features customized Qt Style Sheets (QSS) layout containing responsive `:hover` states, interactive control panels, and custom alignment formatting.
*   **Vector Engine Scaling:** Utilizes an integrated Qt Resource System (`.qrc`) workflow to render a custom SVG graphic asset seamlessly across high-DPI displays without resolution loss.

## 🛠️ System Architecture

The application splits responsibilities cleanly into two layers:
1.  **Frontend GUI Context:** Manages the configuration settings views (`settings.ui`), layout alignment parameters, custom clickable panels, and real-time log displays.
2.  **Core Network Layer:** Instantiates independent asynchronous sockets that read raw payloads out of incoming network datagram buffers and immediately invoke automated outbound retransmission streams.

## 📦 Build Configurations (.pro)

The project compiles cleanly using standard modern desktop environments via `qmake`:

```pro
QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG   += c++17
RESOURCES += resources.qrc
```

## ⚙️ Quick Installation

1. Clone this repository locally:
   ```bash
   gh repo clone AjayMatharia/Udp-Data-forwarder
   ```
2. Open the project profile configuration `udp-data-forwarder.pro` inside your local **Qt Creator IDE**.
3. Hit **Build and Run** (`Ctrl + R`) to compile the optimized binary.
