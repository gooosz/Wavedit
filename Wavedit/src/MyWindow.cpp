#include "MyWindow.h"

MyWindow::MyWindow(QWidget *parent)
	: QWidget(parent), wavfourier(new WavFourier)
{
	setParent(parent);
	setWindowTitle(QString("Wavedit"));

	setDefaultLayout();
	setConnects();
}


void MyWindow::setDefaultLayout()
{
	mainLayout = new QGridLayout;

	createFileChooserBox();
	createPlotBox();

	mainLayout->addWidget(fileChooserBox);
	mainLayout->addWidget(plotBox);
	//mainLayout->addWidget(fileChooserBox, 0, 0, 1, 1); // fileChooserBox on the left side, taking up 1/3 of mainLayout
	//mainLayout->addWidget(plotBox, 0, 2, 1, 2, Qt::AlignLeft); // plotBox on the right side, taking up 2/3 of mainLayout
	setLayout(mainLayout);

	resize(1100,600);
}

// creates layout box on the left 1/3 of window to choose a file
void MyWindow::createFileChooserBox()
{
	fileChooserBox = new QGroupBox(QString("Choose a .wav file"));
	QVBoxLayout *layout = new QVBoxLayout;

	const int width = 0.3 * size().width(); // width of all elements in layout

	openFileDialogButton = new QPushButton(QString("Open file"), fileChooserBox);
	openFileDialogButton->setFixedWidth(width);

	// show mouse coordinates
	mouseCoords = new QLineEdit("Hz");
	mouseCoords->setFixedWidth(width);
	mouseCoords->setAlignment(Qt::AlignCenter);
	// double value with precision max 12 digits and optionally " Hz" at end
	QRegularExpression freqRegex("^(\\d)*\\.?(\\d){0,12}( Hz)?$");
	QRegularExpressionValidator *freqValid = new QRegularExpressionValidator(freqRegex, this);
	mouseCoords->setValidator(freqValid);

	// add button to layout
	layout->addWidget(openFileDialogButton);
	layout->addWidget(mouseCoords);

	fileChooserBox->setLayout(layout);
}

// creates layout box on the right 2/3 of window to plot Fourier Transforms
void MyWindow::createPlotBox()
{
	plotBox = new QGroupBox("Plot");
	QGridLayout *layout = new QGridLayout;

	plot = new PlotWidget(plotBox);
	plot->setMinimumSize(layout->maximumSize());
	//plot->setFixedWidth(0.6 * size().width());
	layout->addWidget(plot);

	plotBox->setLayout(layout);
}

// set all signal/slot connects
void MyWindow::setConnects()
{
	// connect button signal to slot that handles opening a file dialog on button click release
	connect(openFileDialogButton, &QPushButton::released, this, &MyWindow::handleFileDialog);
	// when handleOpenFileDialogButton read all samples in wav file and meta data (signal gotData emitted)
	// then plot the data/fourier transform on button click as well
	connect(wavfourier, &WavFourier::gotData, this, &MyWindow::plotFourierTransform);
	// show a error dialog when reading/opening data file failed
	connect(wavfourier, &WavFourier::failedToGetData, this, &MyWindow::showErrorDialogOpenFile);

	// show mouse x and y coordinates on hover like in matplotlib
	connect(plot, &QCustomPlot::mouseMove, this, &MyWindow::onMouseMove);
	// when a frequency is written into mouseCoords QLineEdit, move the cursor to that position
	// for more accurate clicking when filtering frequency
	connect(mouseCoords, &QLineEdit::editingFinished, this, &MyWindow::onFrequencyEdit);
	// when mouse clicked on frequency amplitude in diagram, filter/remove that frequency from data
	connect(plot, &QCustomPlot::mouseRelease, this, &MyWindow::onMouseClick);

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
	wavfourier->applyWindowFunction(data, WindowFunction::hamming); // reduce leakage effect
	std::cout << "getData() done\n";
	QVector<double> freq = wavfourier->Freq(nextPowOf2(data.size()), wavfourier->getSampleRate());
	std::cout << "Freq() done\n";
	QVector<complex> dft = wavfourier->FFT(data);
	std::cout << "DFT() done\n";
	QVector<double> abs_dft = wavfourier->abs(dft);
	std::cout << "abs() done\n";

	std::cout << "data.size(): "	<< data.size()	<< '\n'
		  << "freq.size(): "	<< freq.size()	<< '\n'
		  << "dft.size(): "	<< dft.size() 	<< '\n';

	plot->makePlot(freq, abs_dft, 0, true, PLOT, Qt::darkYellow);
	std::cout << "makePlot() done\n";
	plot->markNyquistFreq(freq[freq.size()/2], 1, Qt::darkMagenta);
	std::cout << "markNyquistFreq() done\n";

	// the x values in x = [0, getDataSize) are ok
	// because the y values are of importance are they oscillate, so
	// just use 1 as distance between every sample point
	// but in real life the distance between sample points is 1/sampleRate
	//std::iota(std::begin(x), std::end(x), 0);
#else
	CREATE_AND_FILL_QVEC(x, double, 10.24, 0.01);
	QVector<double> y(x.size());
	for (int i=0; i<y.size(); i++) {
		y[i] = std::sin(1.0*2.0*M_PI*x[i]) + 1.0/2.0*std::sin(17.0*2*M_PI*x[i]);
	}
	QVector<double> freq = wavfourier->Freq(y.size(), 100);
	std::cout << "freq.size(): " << freq.size() << '\n';
	QVector<double> dft = wavfourier->abs(wavfourier->FFT(y));
	std::cout << "dft.size(): " << dft.size() << '\n';

	// Frequencies: 10 Hz
	//std::for_each(y.begin(), y.end(), [](double &x1){ return sin(10*2*M_PI*x1); });

	std::cout << y.size() << " samples in QVector\n";
	//plot->makePlot(x, y, 0, true, SCATTER, Qt::white);
	plot->makePlot(freq, dft, 0, true, PLOT, Qt::yellow);
	plot->markNyquistFreq(freq[freq.size()/2], 1);

	/*std::cout << "=========\n";
	for (int i=0; i<freq.size(); i++) {
		std::cout << "( " << freq[i] << ", " << dft[i] << ")\n";
	}*/
#endif // PLOT_TEST
}


// triggered when mouse moves, then update mouse coords
void MyWindow::onMouseMove(QMouseEvent *ev)
{
	double x = plot->xAxis->pixelToCoord(ev->pos().x());
	mouseCoords->setText(QString("%1 Hz").arg(x));
}


// triggered when QLineEdit text changes, move cursor to that input value
void MyWindow::onFrequencyEdit()
{
	QVector<double> freq = wavfourier->Freq(nextPowOf2(wavfourier->getDataSize()),
						wavfourier->getSampleRate());
	QVector<complex> fft = wavfourier->FFT(wavfourier->getData());
	// get double value from mouseCoords field (without the _Hz suffix)
	double freqValue = mouseCoords->text().remove(" Hz").toDouble();
	if (freqValue < 0 || freqValue > freq.size())
		return;	// do nothing, freqValue is out of bounds

	// xcoord of freqValue
	double xCoordInPlot = plot->xAxis->coordToPixel(freqValue);

	// index in vector where (xcoord, ycoord) is bzw. where closes value to xcoord is
	auto const it = std::lower_bound(freq.begin(), freq.end(), freqValue);
	if (it == freq.end()) return;	// no closest value match, do nothing
	int index = *it;	// index of closest values for (freqValue, ycoord)

	// ycoord of freqValue
	double abs_val = std::abs(fft[index]);
	double yCoordInPlot = plot->yAxis->coordToPixel(abs_val);
	QPoint localcoords = QPoint(xCoordInPlot, yCoordInPlot);

	QPoint globalcoords = mapToGlobal(localcoords);

	//QPoint coordsGlobal = plotBox->mapToGlobal(QPoint(xcoordInPlot, yCoordInPlot));
	// TODO: get value of FFT in qcustomplot at freq/xcoord and plot

	QCursor::setPos(globalcoords);
}

// filters the clicked frequency from data
void MyWindow::onMouseClick(QMouseEvent *ev)
{
	// if not left button clicked do nothing
	if (ev->button() != Qt::LeftButton) {
		return;
	}
	// clicked on frequency freq
	double freq = plot->xAxis->pixelToCoord(ev->pos().x());
	/*
	 *
	*/
	std::cout << "mouse click on " << freq << " Hz\n";
}
