#include "PlotWidget.h"

PlotWidget::PlotWidget(QWidget *parent)
	: layout(new QHBoxLayout), plot(new QCustomPlot(this))
{
	setParent(parent);
	layout->addWidget(plot);
	layout->setSizeConstraint(QLayout::SetMaximumSize);
	plot->setMinimumSize(size());

	QVector<double> x(10e6);
	std::iota(std::begin(x), std::end(x), 0);
	makePlot(x, [&](double x1){ return exp(-x1/x.size()*3)*cos(x1/x.size()*20); });
}

/*
 * Plot f(x)
 * graphNr is which graph to draw (if it already exists as graph => overwrite)
 * rescale sets if the graph's axes are to be rescaled to dimensions of the now drawn graph
*/
void PlotWidget::makePlot(const QVector<double> &x, std::function<double(double)> f, int graphNr, bool rescale, QColor color)
{
	QVector<double> y(x.size());
	for (int i=0; i<x.size(); i++) {
		y[i] = f(x[i]);
	}
	makePlot(x, y, graphNr, rescale, color);
}

/*
 * Plot (x,y)
*/
void PlotWidget::makePlot(const QVector<double> &x, const QVector<double> &y, int graphNr, bool rescale, QColor color)
{
	plot->addGraph();
	plot->graph(graphNr)->setData(x, y);

	plot->graph(graphNr)->setPen(QPen(color));
	plot->graph(graphNr)->setLineStyle(QCPGraph::lsNone);
	plot->graph(graphNr)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, color, color, 1));
	plot->setBackground(QBrush(Qt::NoBrush));
	plot->xAxis->setLabel("x");
	plot->yAxis->setLabel("y");
	if (rescale) plot->graph(graphNr)->rescaleAxes();
	/*plot->xAxis->setRange(2000,4000);
	plot->yAxis->setRange(0,66000);*/
	plot->replot();
}
