#include "MyWindow.h"

MyWindow::MyWindow(QWidget *parent)
	: QWidget(parent)
{
	setParent(parent);
	setWindowTitle(QString("Wavedit"));

	setDefaultLayout();
}

void MyWindow::setDefaultLayout()
{
	resize(800,500);
	mainLayout = new QGridLayout;

	createFileChooserBox();
	createPlotBox();

	mainLayout->addWidget(fileChooserBox, 0, 0, 1, 1); // fileChooserBox on the left side, taking up 1/3 of mainLayout
	mainLayout->addWidget(plotBox, 0, 2, 1, 2); // plotBox on the right side, taking up 2/3 of mainLayout
	setLayout(mainLayout);
}

// creates layout box on the left 1/3 of window to choose a file
void MyWindow::createFileChooserBox()
{
	fileChooserBox = new QGroupBox(QString("Choose a .wav file"));
	QVBoxLayout *layout = new QVBoxLayout;

	button1 = new QPushButton(QString("Button1"), fileChooserBox);
	// button1->move(height()/4-button1->size().width()/2, height()/2-button1->size().height()/2);
	layout->addWidget(button1);

	fileChooserBox->setLayout(layout);
}

// creates layout box on the right 2/3 of window to plot Fourier Transforms
void MyWindow::createPlotBox()
{
	plotBox = new QGroupBox(QString("Plot"));
	QGridLayout *layout = new QGridLayout;

	plot = new PlotWidget;
	layout->addWidget(plot, 0, 2, 4, 1);

	plotBox->setLayout(layout);
}
