#include <QApplication>
#include <QMainWindow>
#include <QPushButton>

#include "MyWindow.h"
// #include "ui_mainwindow.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // MyWindow w;
    MyWindow w;
    // Ui::MainWindow main;
    // main.setupUi(&w);
    // w.setStyleSheet("background-color: snow");
    w.show();


    return app.exec();
}
