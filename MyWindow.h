#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
// #include "ui_mainwindow.h"


/*
 * Customize QMainWindow to my liking regarding size, content, etc.
 *
*/
class MyWindow : public QWidget {
	Q_OBJECT

private:
	/* Layouts */
	QBoxLayout *mainLayout;

	/* Components of Layouts */
	QWidget *widgetButton1;
	QPushButton button1;

	void setDefaultLayout();

public:
	explicit MyWindow(QWidget *parent = 0);

signals:
public slots:
};


#endif // MYWINDOW_H
