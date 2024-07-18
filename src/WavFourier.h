#ifndef WAVFOURIER_H
#define WAVFOURIER_H

#include <QObject>
#include <QList>
#include <QFileDialog>
#include <QString>
#include <QTime>

/* Class takes in a WAV File, analyzes it's Bytes
 * and then returns values of the Fourier Transform of the file (or between 2 Timestampts)
 *
 * Structure of WAV file:
 *
*/

struct WAV {

};


class WavFourier : public QObject {
	Q_OBJECT

private:
	QString wav_filename;
	struct WAV wav;

	/* Timerange startTime-endTime to analyze in WAV file */
	QTime startTime;
	QTime endTime;


public:
	WavFourier(QString wav_filename, QTime startTime=QTime(), QTime endTime=QTime());

	QList<double> getFourierTransform();	// calculates fourier transform of given WAV file


signals:
public slots:
	void handleOpenFileDialogButton();
};


#endif // WAVFOURIER_H
