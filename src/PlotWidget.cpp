#include "PlotWidget.h"

PlotWidget::PlotWidget(QWidget *parent)
	: layout(new QHBoxLayout), plot(new QCustomPlot(this))
{
	setParent(parent);
	layout->addWidget(plot);
	layout->setSizeConstraint(QLayout::SetMaximumSize);
	plot->setMinimumSize(size());

	QVector<double> x(1001);
	std::iota(std::begin(x), std::end(x), 0);
	makePlot(x, [&](double x1){ return exp(-x1/x.size()*3)*cos(x1/x.size()*20); });
}

/*
 * Plot f(x)
 * graphNr is which graph to draw (if it already exists as graph => overwrite)
 * rescale sets if the graph's axes are to be rescaled to dimensions of the now drawn graph
*/
void PlotWidget::makePlot(QVector<double> x, std::function<double(double)> f, int graphNr, bool rescale)
{
	QVector<double> y(x.size());
	for (int i=0; i<x.size(); i++) {
		y[i] = f(x[i]);
	}
	makePlot(x, y, graphNr, rescale);
}

/*
 * Plot (x,y)
*/
void PlotWidget::makePlot(QVector<double> x, QVector<double> y, int graphNr, bool rescale)
{
	plot->addGraph();
	plot->graph(graphNr)->setData(x,y);

	plot->graph(graphNr)->setPen(QPen(Qt::green));
	plot->setBackground(QBrush(Qt::NoBrush));
	plot->xAxis->setLabel("x");
	plot->yAxis->setLabel("y");
	if (rescale) plot->graph(graphNr)->rescaleAxes();
	plot->replot();
}
