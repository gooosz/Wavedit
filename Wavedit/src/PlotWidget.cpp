#include "PlotWidget.h"


// get minimum, maximum element in vector
#define MIN_ELEMENT_VEC(vec) (*std::min_element(vec.begin(), vec.end()))
#define MAX_ELEMENT_VEC(vec) (*std::max_element(vec.begin(), vec.end()))


PlotWidget::PlotWidget(QWidget *parent)
{
	setParent(parent);
	//layout->addWidget(plot);
	//layout->setSizeConstraint(QLayout::SetMaximumSize);
	//plot->setFixedSize(parent->size());

	QVector<double> x(100);
	std::iota(std::begin(x), std::end(x), double_iota(0.1/x.size()));
	// makePlot(x, [&](double x1){ return exp(-x1/x.size()*3)*cos(x1/x.size()*20); });
	// Frequencies: 10 Hz
	makePlot(x, [&](double x1){ return sin(10*2*M_PI*x1); }, 0, true);
}

/*
 * Plot f(x)
 * graphNr is which graph to draw (if it already exists as graph => overwrite)
 * rescale sets if the graph's axes are to be rescaled to dimensions of the now drawn graph
*/
void PlotWidget::makePlot(const QVector<double> &x, std::function<double(double)> f, int graphNr, bool rescale, PlotType plotType, QColor color)
{
	QVector<double> y(x.size());
	for (int i=0; i<x.size(); i++) {
		y[i] = f(x[i]);
	}
	makePlot(x, y, graphNr, rescale, plotType, color);
}

/*
 * Plot (x,y)
*/
void PlotWidget::makePlot(const QVector<double> &x, const QVector<double> &y, int graphNr, bool rescale, PlotType plotType, QColor color)
{
	if (graphCount() == 0) {
		addGraph();
	}
	graph(graphNr)->setData(x, y);

	graph(graphNr)->setPen(QPen(color));
	if (plotType == SCATTER) {
		graph(graphNr)->setLineStyle(QCPGraph::lsNone);
		graph(graphNr)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, color, color, 1));
	}
	setBackground(QBrush(Qt::NoBrush));
	xAxis->setLabel("Hz");
	yAxis->setLabel("y");

	if (x.size() > 50000) {
		QSharedPointer<QCPAxisTickerFixed> xticks(new QCPAxisTickerFixed);
		xticks->setTickStep(5000);	// display tick label every 5000 Hz
		xticks->setScaleStrategy(QCPAxisTickerFixed::ssNone);	// tick distance/step is fix
		xAxis->setTicker(xticks);
	}

	if (rescale) {
		graph(graphNr)->rescaleAxes();
	}
	replot();
}

// draws a vertical line where the Nyquist frequency is
void PlotWidget::markNyquistFreq(double nyquist, int graphNr, QColor color)
{
	if (graphCount() <= 1) {
		addGraph();
	}
	graph(graphNr)->setPen(QPen(color));
	double ymin = yAxis->range().lower;
	double ymax = yAxis->range().upper;
	QVector<double> x = {nyquist, nyquist};
	QVector<double> y = {yAxis->range().lower, yAxis->range().upper};
	graph(graphNr)->setData(x, y);
	setBackground(QBrush(Qt::NoBrush));
	replot();
}

