#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QLayout>

#include <functional>
#include <algorithm>
// #include <cmath>

#include "qcustomplot.h"

/* used to specify step size and min value used for std::iota()
 * see https://stackoverflow.com/questions/39162938/easiest-way-to-fill-stdvectordouble-with-equidistant-values
*/
struct double_iota {
	// TODO: change inc to how many points should be in interval
	double_iota(double inc, double init_value=0.0) : _value(init_value), _inc(inc) {}
	operator double() const { return _value; }
	double_iota& operator++() { _value += _inc; return *this; }
	double _value;
	double _inc;
};

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
