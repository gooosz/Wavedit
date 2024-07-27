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
	connect(openFileDialogButton, &QPushButton::released, this, &MyWindow::handleFileDialog);
	// when handleOpenFileDialogButton read all samples in wav file and meta data (signal gotData emitted)
	// then plot the data/fourier transform on button click as well
	connect(wavfourier, &WavFourier::gotData, this, &MyWindow::plotFourierTransform);
	// show a error dialog when reading/opening data file failed
	connect(wavfourier, &WavFourier::failedToGetData, this, &MyWindow::showErrorDialogOpenFile);

	// add button to layout
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

// open a error dialog for showing that opening data file failed
void MyWindow::showErrorDialogOpenFile()
{
	QMessageBox errorMsg;
	errorMsg.critical(0, "Error", "Could not open file");
	errorMsg.setFixedSize(500,200);
}

void MyWindow::handleFileDialog()
{
	QString filename = QFileDialog::getOpenFileName(nullptr, "Open WAV File", "/home", "(*.wav)");
	wavfourier->populateData(filename);
}


// size of a vector that stores _n numbers with equal distance _step to the next one
#define VECSIZE(_n, _step) (_n * _step)
/*
 * creates a QVector of type _type with name _name
 * and fills it with _n numbers with equal distance _step to the next one
*/
#define CREATE_AND_FILL_QVEC(_name, _type, _n, _step) \
				QVector<_type> _name(_n / _step); \
				std::iota(std::begin(_name), std::end(_name), double_iota(_step))


//#define PLOT_TEST

void MyWindow::plotFourierTransform()
{
#ifndef PLOT_TEST
	QVector<double> x(wavfourier->getDataSize());
	QVector<double> data = wavfourier->getData();
	std::cout << "getData() done\n";
	QVector<double> freq = wavfourier->Freq(data.size(), wavfourier->getSampleRate());
	std::cout << "Freq() done\n";
	QVector<complex> dft = wavfourier->DFT(data);
	std::cout << "DFT() done\n";
	QVector<double> abs_dft = wavfourier->abs(dft);
	std::cout << "abs() done\n";

	std::cout << "data.size(): "	<< data.size()	<< '\n'
		  << "freq.size(): "	<< freq.size()	<< '\n'
		  << "dft.size(): "	<< dft.size() 	<< '\n';

	plot->makePlot(freq, abs_dft, 0, true, PLOT, Qt::darkYellow);
	std::cout << "makePlot() done\n";
	plot->markNyquistFreq(freq[freq.size()/2], Qt::darkMagenta);
	std::cout << "markNyquistFreq() done\n";

	// the x values in x = [0, getDataSize) are ok
	// because the y values are of importance are they oscillate, so
	// just use 1 as distance between every sample point
	// but in real life the distance between sample points is 1/sampleRate
	//std::iota(std::begin(x), std::end(x), 0);
#else
	CREATE_AND_FILL_QVEC(x, double, 10, 0.01);
	QVector<double> y(x.size());
	for (int i=0; i<y.size(); i++) {
		y[i] = std::sin(1.0*2.0*M_PI*x[i]) + 1.0/2.0*std::sin(17.0*2*M_PI*x[i]);
	}
	QVector<double> freq = wavfourier->Freq(y.size(), 100);
	std::cout << "freq.size(): " << freq.size() << '\n';
	QVector<double> dft = wavfourier->abs(wavfourier->DFT(y));
	std::cout << "dft.size(): " << dft.size() << '\n';

	// Frequencies: 10 Hz
	//std::for_each(y.begin(), y.end(), [](double &x1){ return sin(10*2*M_PI*x1); });

	std::cout << y.size() << " samples in QVector\n";
	//plot->makePlot(x, y, 0, true, SCATTER, Qt::white);
	plot->makePlot(freq, dft, 0, true, PLOT, Qt::yellow);
	plot->markNyquistFreq(freq[freq.size()/2]);

	/*std::cout << "=========\n";
	for (int i=0; i<freq.size(); i++) {
		std::cout << "( " << freq[i] << ", " << dft[i] << ")\n";
	}*/
#endif // PLOT_TEST
}
