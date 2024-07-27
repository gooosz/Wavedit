#include "WavFourier.h"


/*WavFourier::WavFourier(QString wav_filename, QTime startTime, QTime endTime)
{
}*/

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
	// TODO: but DFT, IDFT uses full spectrum (I want it like that to visualize symmetry)
	//	 so return the full freq spectrum until size as well, update Test
	for (int i=0; i<size/2; i++) {	// last index is size/2 - 1
		freq[i] = i * sample_rate / size;
	}
	return freq;
}

// calculates Discrete-Fourier-Transform of data
// @returns fourier coefficients (beta_j)
QVector<complex> WavFourier::DFT(const QVector<double>& vec)
{
	QVector<complex> beta(vec.size());
	for (int k=0; k<vec.size(); k++) {
		complex sum = 0.0;
		for (int j=0; j<vec.size(); j++) {
			sum += vec[j] * std::exp(-2.0 * M_PI * complex(0.0, 1.0) * (double) j * (double) k / (double)vec.size());
		}
		beta[k] = sum;
	}

	//beta.resizebeta.size() / 2);	// maximum frequency <= 2 * sample rate, so only keep half the size (rest is symmetric)
	// keep all the values because else IDFT would need to create half of the values out of
	// fourier coefficients, which is not a problem but I don't like
	// => return the all frequency bins as well
	// TODO: the diagramm shows symmetry => in Präsentation erwähnen warum DFT symmetrisch dlistel
	//	 weil die Werte i > Nyquist-Frequest nur die komplex konjugierten sind
	return beta;
}

// absolute value of every element of vec
QVector<double> WavFourier::abs(const QVector<complex>& vec)
{
	QVector<double> abs_beta(vec.size());
	for (int j=0; j<vec.size(); j++) {
		abs_beta[j] = std::abs(vec[j]);
	}
	return abs_beta;
}


// Inverse Discrete-Fourier-Transform on DFT(data), vec specifies the whole
QVector<complex> WavFourier::IDFT(const QVector<complex>& vec)
{
	QVector<complex> beta_inv(vec.size());
	for (int k=0; k<vec.size(); k++) {
		complex sum = 0.0;
		for (int j=0; j<vec.size(); j++) {
			// DFT was e^(-2 * pi * i * j * k / vec.size())
			// 	rotates on complex plane unit circle counterclockwise
			// IDFT is e^(-2 * pi * i * j * k / vec.size())
			//	rotates on complex plane unit circle clockwise
			// which
			sum += vec[j] * std::exp(2.0 * M_PI * complex(0.0, 1.0) * (double) j * (double) k / (double)vec.size());
		}
		// normalization factor 1/n because as matrices: DFT*IDFT soll Einheitsmatrix E_n
		// but DFT*IDFT = n*E_n => hence we do DFT * 1/n*IDFT = E_n
		beta_inv[k] = sum / (double)vec.size();
	}
	return beta_inv;
}

/* returns real values of IDFT,
 * use only if you know data was
 * real (not complex) to begin with */
QVector<double> WavFourier::IDFT_real(const QVector<complex>& vec)
{
	QVector<complex> idft = IDFT(vec);
	QVector<double> idft_real(idft.size());
	for (int i=0; i<idft.size(); i++) {
		idft_real[i] = idft[i].real();
	}
	return idft_real;
}














