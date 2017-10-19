#include "mainwindow.h"
#include <QApplication>

#include <QSslSocket>
#include <QDebug>

int main(int argc, char *argv[])
{
    //  Check the existens of the SSL Libraries
    //  In Windows they have to be installed separatley
    if (!QSslSocket::supportsSsl()) {
        qWarning () << "No SSL Support";
        exit (1);
    }
    qDebug () << QSslSocket::sslLibraryVersionString();

    QApplication a(argc, argv);
    mainWindow = new MainWindow();
    mainWindow->show();
//    mainWindow = MainWindow;
//    w.show();

    return a.exec();
}
