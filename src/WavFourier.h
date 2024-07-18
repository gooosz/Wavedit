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
 * Structure of WAV file (taken from http://soundfile.sapp.org/doc/WaveFormat/):
 * Offset	Size	Name
 *   0		 4	ChunkID
 *   4		 4	ChunkSize
 *   8		 4	Format
 *
 *  12		 4	Subchunk1ID
 *  16		 4	Subchunk1Size
 *  20		 2	AudioFormat
 *  22		 2	NumChannels
 *  24		 4	SampleRate
 *  28		 4	ByteRate
 *  32		 2	BlockAlign
 *  34		 2	BitsPerSample
 *	evtl. extra parameters
 *  36		 4	SubChunk2ID
 *  40		 4	SubChunk2Size
 *  44		 *	actual data
*/

struct WAV_Header {
	/* -- RIFF Chunk -- */
	qint32 chunkID;
	qint32 chunkSize;
	qint32 format;
	/* -- FMT Subchunk -- */
	qint32 subchunk1ID;
	qint32 subchunk1Size;
	qint16 audioFormat;	// PCM = 1, else some form of compression
	qint16 numChannels;	// Mono = 1, Stereo = 2
	qint32 sampleRate;
	qint32 byteRate;	// = sampleRate * numChannels * BitsPerSample/8
	qint16 blockAlign;
	/* -- Data Subchunk -- */
	qint32 subchunk2ID;
	qint32 subchunk2Size;
	qint16 *data;		// array of samples (1 sample has size byteRate, so 2 Bytes)
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
