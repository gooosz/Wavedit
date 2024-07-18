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
	mainLayout = new QGridLayout;

	createFileChooserBox();
	createPlotBox();

	mainLayout->addWidget(fileChooserBox, 0, 0, 1, 1); // fileChooserBox on the left side, taking up 1/3 of mainLayout
	mainLayout->addWidget(plotBox, 0, 2, 1, 2); // plotBox on the right side, taking up 2/3 of mainLayout
	setLayout(mainLayout);

	resize(1000,600);
}

// creates layout box on the left 1/3 of window to choose a file
void MyWindow::createFileChooserBox()
{
	fileChooserBox = new QGroupBox(QString("Choose a .wav file"));
	QVBoxLayout *layout = new QVBoxLayout;

	button1 = new QPushButton(QString("Button1"), fileChooserBox);
	layout->addWidget(button1);

	fileChooserBox->setLayout(layout);
}

// creates layout box on the right 2/3 of window to plot Fourier Transforms
void MyWindow::createPlotBox()
{
	plotBox = new QGroupBox(QString("Plot"));
	QGridLayout *layout = new QGridLayout;

	// text = new QTextEdit("This is a QTextEdit");
	// layout->addWidget(text);

	plot = new PlotWidget(plotBox);
	plot->setMinimumSize(layout->maximumSize());
	layout->addWidget(plot);

	plotBox->setLayout(layout);
}
