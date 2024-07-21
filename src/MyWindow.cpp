#include "MyWindow.h"

MyWindow::MyWindow(QWidget *parent)
	: QWidget(parent), wavfourier(new WavFourier)
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

	resize(1100,600);
}

// creates layout box on the left 1/3 of window to choose a file
void MyWindow::createFileChooserBox()
{
	fileChooserBox = new QGroupBox(QString("Choose a .wav file"));
	QVBoxLayout *layout = new QVBoxLayout;

	openFileDialogButton = new QPushButton(QString("Open file"), fileChooserBox);
	// connect button signal to slot that handles opening a file dialog on button click release
	connect(openFileDialogButton, &QPushButton::released, wavfourier, &WavFourier::handleOpenFileDialogButton);
	// when handleOpenFileDialogButton read all samples in wav file and meta data (signal gotData emitted)
	// then plot the data/fourier transform on button click as well
	connect(wavfourier, &WavFourier::gotData, this, &MyWindow::plotFourierTransform);
	layout->addWidget(openFileDialogButton);

	fileChooserBox->setLayout(layout);
}

// creates layout box on the right 2/3 of window to plot Fourier Transforms
void MyWindow::createPlotBox()
{
	plotBox = new QGroupBox("Plot");
	QGridLayout *layout = new QGridLayout;

	plot = new PlotWidget(plotBox);
	plot->setMinimumSize(layout->maximumSize());
	layout->addWidget(plot);

	plotBox->setLayout(layout);
}

void MyWindow::plotFourierTransform()
{
	QVector<double> x(wavfourier->getDataSize());
	// the x values in x = [0, getDataSize) are ok
	// because the y values are of importance are they oscillate, so
	// just use 1 as distance between every sample point
	// but in real life the distance between sample points is 1/sampleRate
	std::iota(std::begin(x), std::end(x), 0);

	// create QVector<double> out of QList<quint16> to plot
	QList<quint16> &y_samples = wavfourier->getData();
	QVector<double> y;
	for (int i=0; i<y_samples.size(); i++) {
		double dlistel = static_cast<double>(y_samples[i]);
		y.push_back(dlistel);
	}
	std::reverse(y.begin(), y.end());
	std::cout << y_samples.size() << " samples in QList\n";
	std::cout << y.size() << " samples in QVector\n";
	plot->makePlot(x, y, 0, true, SCATTER, Qt::white);
	/*QVector<double> x2(10e6);
	std::iota(std::begin(x2), std::end(x2), 0);
	plot->makePlot(x2, [&](double x1){ return 0.5; }, 1, false, Qt::red);*/
}
