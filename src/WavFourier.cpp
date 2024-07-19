#include "WavFourier.h"


/*WavFourier::WavFourier(QString wav_filename, QTime startTime, QTime endTime)
{
}*/


// populates data QList from WAV file
void WavFourier::populateData(QString wav_filename)
{
	bool loadWAVSuccess = wavfile.load(wav_filename.toStdString());
	if (!loadWAVSuccess) {
		QMessageBox errorMsg;
		errorMsg.critical(0, "Error", "Could not open file");
		errorMsg.setFixedSize(500,200);
		return;
	}

	// wavfile contains all needed infos about WAV file, so get the data from file
	for (int i=0; i<wavfile.getNumSamplesPerChannel(); i++) {
		for (int channel=0; channel<wavfile.getNumChannels(); channel++) {
			data.push_back(wavfile.samples[channel][i]);
		}
	}

	std::cout << "ChunkID: " 	<< '-' << '\n'
		  << "ChunkSize: " 	<< '-' << '\n'
		  << "Format: " 	<< '-' << '\n'
		  << "SubChunk1ID: " 	<< '-' << '\n'
		  << "SubChunk1Size: "	<< '-' << '\n'
		  << "AudioFormat: "	<< '-' << '\n'
		  << "NumChannels: "	<< wavfile.getNumChannels() << '\n'
		  << "SampleRate: "	<< wavfile.getSampleRate() << '\n'
		  << "ByteRate: "	<< '-' << '\n'
		  << "BlockAlign: "	<< '-' << '\n'
		  << "BitsPerSample: "	<< '-' << '\n'
		  << "SubChunk2ID: "	<< '-' << '\n'
		  << "SubChunk2Size: "	<< '-' << '\n';
	std::cout << "Length in secs: " << wavfile.getLengthInSeconds() << '\n';
}

// returns data as QList from WAV file
/*
 * Populate the wav structure
 * if startTime==0 && endTime==0 (default constructed)
 * then set endTime to endTime of file, so analyze the whole WAV file
 *
*/
QList<double> WavFourier::getData(QString wav_filename, QTime startTime, QTime endTime)
{
	return QList<double>();
}

void WavFourier::handleOpenFileDialogButton()
{
	wav_filename = QFileDialog::getOpenFileName(nullptr, "Open WAV File", "/home", "(*.wav)");
	populateData(wav_filename);
	QList<double> data = getData(wav_filename);
}
