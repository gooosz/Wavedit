#include <QApplication>
#include <QMainWindow>

#include "ui_mainwindow.h"




int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMainWindow w;
    Ui::MainWindow main;
    main.setupUi(&w);
    w.setObjectName("QMainWindow");
    w.setStyleSheet("background-color: snow");
    w.show();

    return app.exec();
}
