#include "PlotWidget.h"

PlotWidget::PlotWidget(QWidget *parent)
	: layout(new QHBoxLayout), plot(new QCustomPlot(this))
{
	setParent(parent);
	layout->addWidget(plot);
	layout->setSizeConstraint(QLayout::SetMaximumSize);
	plot->setMinimumSize(size());

	makePlot();
}

void PlotWidget::makePlot()
{
	QVector<double> x(101), y(101);
	for (int i=0; i<x.size(); i++) {
		x[i] = i/(double)x.size()*8;
		y[i] = qSin(x[i]);
	}
	plot->addGraph();
	plot->graph(0)->setData(x,y);

	plot->setBackground(QBrush(Qt::NoBrush));
	plot->xAxis->setLabel("x");
	// plot->xAxis->setRange(-1, 1);
	plot->yAxis->setLabel("y");
	// plot->yAxis->setRange(-1, 1);
	plot->graph(0)->rescaleAxes();
	plot->replot();
}
