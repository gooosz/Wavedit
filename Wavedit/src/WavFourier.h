#ifndef WAVFOURIER_H
#define WAVFOURIER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QTime>

#include <complex>
#include <cmath>

#include "AudioFile.h"
#include "fft.h"

/* Class takes in a WAV File, analyzes it's Bytes
 * and then returns values of the Fourier Transform of the file (or between 2 Timestampts)
 *
 * Structure of WAV file (taken from http://soundfile.sapp.org/doc/WaveFormat/)
 * everything is little-endian unless otherwise specified:
 * Offset	Size	Name
 *  --------- RIFF Chunk -----------
 *   0		 4	ChunkID		// "RIFF" in big-endian
 *   4		 4	ChunkSize	// 36 + SubChunk2Size
 *   8		 4	Format		// "WAVE" in big-endian
 *  --------- FMT Chunk -----------
 *  12		 4	Subchunk1ID	// "fmt " in big-endian
 *  16		 4	Subchunk1Size	// 16 for PCM, size of rest of fmt chunk following this number
 *  20		 2	AudioFormat	// PCM = 1, else some form of compression
 *  22		 2	NumChannels	// Mono = 1, Stereo = 2
 *  24		 4	SampleRate	// 8000, 44100, etc.
 *  28		 4	ByteRate	// = sampleRate * numChannels * BitsPerSample/8
 *  32		 2	BlockAlign	// = NumChannels * BitsPerSample/8
 *  34		 2	BitsPerSample	// 8, 16
 *	evtl. extra parameters
 *  --------- Data Chunk -----------
 *  36		 4	SubChunk2ID	// "data" in big-endian
 *  40		 4	SubChunk2Size	// number of bytes in actual data
 *  44		 *	actual data
*/


using complex = std::complex<double>;
Q_DECLARE_METATYPE(complex);

// Some different window functions to reduce the leackage effect
struct WindowFunction {
	// M = window width = data.size()
	static double rect(double n, double M);
	static double vonhann(double n, double M);
	static double hamming(double n, double M);
	static double blackman(double n, double M);
	static double flattop(double n, double M);
	static double parzen(double n, double M);
};

class WavFourier : public QObject {
	Q_OBJECT

private:
	AudioFile<qint16> wavfile;

	/* Timerange startTime-endTime to analyze in WAV file */
	QTime startTime;
	QTime endTime;

	QList<qint16> data_int16;	// QList of samples (1 sample has size byteRate, so 2 Bytes)
	QVector<double> data;

	QVector<complex> dft;
	QVector<complex> fft;

public:
	// apply (multiply) window function to data to reduce leackage effect
	// hamming window should be overall the best one to use
	void applyWindowFunction(QVector<double> &vec, std::function<double(double,double)> window = WindowFunction::hamming);

	QVector<double> getStuetzstellen(int size);	// returns stuetzstelle x_k of data point x using (2*M_PI*k)/n
	QVector<double> Freq(int size, double sample_rate=1.0);		// returns the DFT sample frequency bin centers
	// calculate flag specifies if you want to calculate the DFT again
	QVector<complex>& DFT(QVector<double>& vec, bool calculate=false);		// Discrete-Fourier-Transform on data
	QVector<double> abs(const QVector<complex>& vec);		// absolute value of every element of vec
	QVector<complex> IDFT(const QVector<complex>& vec);		// Inverse Discrete-Fourier-Transform on DFT(data), returns complex numbers
	QVector<double> real(const QVector<complex>& vec);	// returns real values of vec,
								// use only if you know data was
								// real (not complex) to begin with
	// not const because applies a window function to vec
	// calculate flag specifies if you want to calculate the FFT again
	QVector<complex>& FFT(QVector<double>& vec, bool calculate=false);	// returns the FFT of sample

	QVector<complex> IFFT(const QVector<complex>& vec);	// Inverse Fast Fourier Transform

	void filter(QVector<complex>& fourier, QVector<int> idxOfPeak);

	// returns data as QList from WAV file
	bool populateData(QString wav_filename);

	void setWavFilename(QString filename);

public:
	WavFourier() = default;
	// WavFourier(QString wav_filename, QTime startTime=QTime(), QTime endTime=QTime());

	qint64 getDataSize();	// size of data in bytes
	QList<qint16>& getDataList(QTime startTime=QTime(), QTime endTime=QTime());
	QVector<double>& getData(QTime startTime=QTime(), QTime endTime=QTime());
	QList<double>& getFourierTransform();	// calculates fourier transform of given WAV file
	double getSampleRate();

	int getBinOfFreq(const QVector<double>& freq, double f);	// returns the index of the bin that frequency f belongs to, -1 if none
	QVector<int> getPeakNear(const QVector<double>& freqBins, int idx);	// returns indices of the peak next to frequency bin with index idx

signals:
	void gotData();	// data read from wav file
	void failedToGetData();	// error when trying to read data
public slots:
};

// returns next power of 2 >= n
int nextPowOf2(int n);

#endif // WAVFOURIER_H
