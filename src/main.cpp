#include "mainwindow.h"
#include <QApplication>
#include <QBasicTimer>
#include <QThread>
#include <QImage>
#include <QWidget>
#include <QException>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file("resources/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString style = QLatin1String(file.readAll());
    qApp->setStyleSheet(style);

    MainWindow *w = new MainWindow();
    w->setAttribute(Qt::WA_QuitOnClose, true);
    w->setWindowIcon(QIcon("resources/icon.png"));
    w->setWindowState(Qt::WindowMaximized);
    w->show();

    return a.exec();
}
