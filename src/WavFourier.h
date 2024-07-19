#ifndef WAVFOURIER_H
#define WAVFOURIER_H

#include <QObject>
#include <QList>
#include <QFileDialog>
#include <QString>
#include <QTime>
#include <QMessageBox>

#include "AudioFile.h"

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


class WavFourier : public QObject {
	Q_OBJECT

private:
	QString wav_filename;
	AudioFile<qint16> wavfile;

	/* Timerange startTime-endTime to analyze in WAV file */
	QTime startTime;
	QTime endTime;

	QList<quint16> data;	// QList of samples (1 sample has size byteRate, so 2 Bytes)

	// returns data as QList from WAV file
	void populateData(QString wav_filename);

public:
	WavFourier() = default;
	// WavFourier(QString wav_filename, QTime startTime=QTime(), QTime endTime=QTime());

	QList<double> getData(QString wav_filename, QTime startTime=QTime(), QTime endTime=QTime());
	QList<double> getFourierTransform();	// calculates fourier transform of given WAV file


signals:
public slots:
	void handleOpenFileDialogButton();
};


#endif // WAVFOURIER_H
