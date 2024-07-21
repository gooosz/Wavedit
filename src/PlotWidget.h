#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QLayout>

#include <functional>
// #include <cmath>

#include "qcustomplot.h"

enum PlotType {
	PLOT,
	SCATTER
};

/* A widget containing QCustomPlot */
class PlotWidget : public QWidget {
private:
	QHBoxLayout *layout;
	QCustomPlot *plot;
public:
	PlotWidget(QWidget *parent = 0);

	void makePlot(const QVector<double> &x, std::function<double(double)> f, int graphNr=0, bool rescale=true, PlotType plotType=PLOT, QColor color=Qt::green);
	void makePlot(const QVector<double> &x, const QVector<double> &y, int graphNr=0, bool rescale=true, PlotType plotType=PLOT, QColor color=Qt::green);
};

#endif // PLOTWIDGET_H
