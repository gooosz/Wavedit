#include "PlotWidget.h"

PlotWidget::PlotWidget(QWidget *parent)
	: layout(new QHBoxLayout), plot(new QCustomPlot(this))
{
	setParent(parent);
	layout->addWidget(plot);
	layout->setSizeConstraint(QLayout::SetMaximumSize);
	plot->setMinimumSize(size());

	QVector<double> x(101);
	std::iota(std::begin(x), std::end(x), 0);
	makePlot(x, sin);
}

/* Plot f(x) */
void PlotWidget::makePlot(QVector<double> x, std::function<double(double)> f)
{
	QVector<double> y(x.size());
	for (int i=0; i<x.size(); i++) {
		y[i] = f(x[i]);
	}
	plot->addGraph();
	plot->graph(0)->setData(x,y);

	plot->graph(0)->setPen(QPen(Qt::green));
	plot->setBackground(QBrush(Qt::NoBrush));
	plot->xAxis->setLabel("x");
	// plot->xAxis->setRange(-1, 1);
	plot->yAxis->setLabel("y");
	// plot->yAxis->setRange(-1, 1);
	plot->graph(0)->rescaleAxes();
	plot->replot();
}
