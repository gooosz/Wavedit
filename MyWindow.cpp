#include "MyWindow.h"

MyWindow::MyWindow(QWidget *parent)
	: QWidget(parent), mainLayout(new QHBoxLayout), button1(this)
{
	setParent(parent);
	setWindowTitle(QString("Wavedit"));

	setDefaultLayout();
}

void MyWindow::setDefaultLayout()
{
	/* -- window related -- */
	resize(600, 400);

	/* -- Components of Layouts -- */
	button1.setText("Button1");
	button1.move(width()/4-button1.size().width()/2, height()/2-button1.size().height()/2);


}
