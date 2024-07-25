#include "WavFourier.h"


/*WavFourier::WavFourier(QString wav_filename, QTime startTime, QTime endTime)
{
}*/

using complex = std::complex<double>;

// populates data QList from WAV file
/* returns true on success */
bool WavFourier::populateData(QString wav_filename)
{
	bool loadWAVSuccess = wavfile.load(wav_filename.toStdString());
	// only signal that the data is all read if it was successfull
	if (!loadWAVSuccess) {
		emit failedToGetData();	// show error dialog in MyWindow
		return false;
	}

	// wavfile contains all needed infos about WAV file, so get the data from file
	for (int i=0; i<wavfile.getNumSamplesPerChannel(); i++) {
		for (int channel=0; channel<wavfile.getNumChannels(); channel++) {
			data_uint16.push_back(wavfile.samples[channel][i]);
		}
	}
	// fill double QVector data as well
	for (int i=0; i<data_uint16.size(); i++) {
		double dlistel = static_cast<double>(data_uint16[i]);	// double list element (dlistel)
		data.push_back(dlistel);
	}
	std::reverse(data.begin(), data.end());


	std::cout << "ChunkID: " 	<< '-' << '\n'
		  << "ChunkSize: " 	<< '-' << '\n'
		  << "Format: " 	<< '-' << '\n'
		  << "SubChunk1ID: " 	<< '-' << '\n'
		  << "SubChunk1Size: "	<< '-' << '\n'
		  << "AudioFormat: "	<< '-' << '\n'
		  << "NumChannels: "	<< wavfile.getNumChannels() << '\n'
		  << "SampleRate: "	<< wavfile.getSampleRate() << '\n'
		  << "ByteRate: "	<< wavfile.getBitDepth()/8 << '\n'
		  << "BlockAlign: "	<< '-' << '\n'
		  << "BitsPerSample: "	<< '-' << '\n'
		  << "SubChunk2ID: "	<< '-' << '\n'
		  << "SubChunk2Size: "	<< '-' << '\n';
	std::cout << "Length in secs: " << wavfile.getLengthInSeconds() << '\n';
	std::cout << "Date size in bytes: " << getDataSize() << '\n';
	std::cout << "getNumSamplesPerChannel(): " << wavfile.getNumSamplesPerChannel() << '\n';

	emit gotData();	// success
	return true;
}

/* returns size of data in bytes */
qint64 WavFourier::getDataSize()
{
	return wavfile.getBitDepth()/8 * wavfile.getNumSamplesPerChannel() * wavfile.getNumChannels();
}

// returns data as QList from WAV file
/*
 * if startTime==0 && endTime==0 (default constructed)
 * then set endTime to endTime of file, so analyze the whole WAV file
*/
QList<quint16>& WavFourier::getDataList(QTime startTime, QTime endTime)
{
	return data_uint16;
}

QVector<double>& WavFourier::getData(QTime startTime, QTime endTime)
{
	return data;
}

// returns stuetzstelle x_k of data point x using (2*M_PI*k)/n
QVector<double> WavFourier::getStuetzstellen(int size)
{
	QVector<double> x_k(size);
	for (int k=0; k<x_k.size(); k++) {
		x_k[k] = 2.0 * M_PI * k / size;
	}
	return x_k;
}

// returns the DFT sample frequency bin centers
QVector<double> WavFourier::Freq(int size, double sample_rate)
{
	QVector<double> freq(size/2);
	if (size == 0)	return freq;	// empty size means no frequency bins
	// see https://numpy.org/doc/stable/reference/generated/numpy.fft.fftfreq.html+
	// or https://stackoverflow.com/questions/4364823/how-to-get-frequency-from-fft-result/4371627#4371627
	// only adds to < size/2 because of Nyquist-frequency (rest is symmetric) => small perfomance boost
	// Nyquist frequency at size/2
	for (int i=0; i<size/2; i++) {	// last index is size/2 - 1
		freq[i] = i * sample_rate / size;
	}
	return freq;
}

// calculates Discrete-Fourier-Transform of data
// @returns absolute values of fourier coefficients (beta_j)
QVector<double> WavFourier::DFT(const QVector<double>& vec)
{
	QVector<complex> beta(vec.size());
	for (int k=0; k<vec.size(); k++) {
		complex sum = 0.0;
		for (int j=0; j<vec.size(); j++) {
			sum += vec[j] * std::exp(-2.0 * M_PI * complex(0.0, 1.0) * (double) j * (double) k / (double)vec.size());
		}
		beta[k] = sum;
	}

	// absolute values of beta
	QVector<double> abs_beta(beta.size());
	for (int j=0; j<beta.size(); j++) {
		abs_beta[j] = std::abs(beta[j]);
	}
	//abs_beta.resize(abs_beta.size() / 2);	// maximum frequency <= 2 * sample rate, so only keep half the size (rest is symmetric)
	return abs_beta;
}

