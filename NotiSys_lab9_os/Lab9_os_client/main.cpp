#include "notisys.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QApplication>
#include <windows.h> // Required for HANDLE and Windows API functions
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    NotiSys w;
    w.show();

    QString pipeName = R"(\\.\pipe\server_pipe)";
    qDebug() << "Connecting to server...";

    HANDLE hPipe = CreateFile(
        reinterpret_cast<LPCWSTR>(pipeName.utf16()), // Використання utf16() напряму
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        qCritical() << "Error: Unable to connect to the server. Exiting...";
        QMessageBox::critical(nullptr, "Connection Error", "Unable to connect to the server. Check server activity.");
        return 1;
    }

    CloseHandle(hPipe);

    qDebug() << "Connected to server successfully.";

    return a.exec();
}


