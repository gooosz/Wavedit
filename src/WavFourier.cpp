#include "WavFourier.h"

/*
 * Populate the wav structure in constructor
 * if startTime==0 && endTime==0 (default constructed)
 * then set endTime to endTime of file, so analyze the whole WAV file
 *
*/
WavFourier::WavFourier(QString wav_filename, QTime startTime, QTime endTime);
{

}


void WavFourier::handleOpenFileDialogButton()
{
	/*static int clickedNr = 0;
	clickedNr++;
	openFileDialogButton->setText(QString("Clicked %1 times").arg(clickedNr));*/
	wav_filename = QFileDialog::getOpenFileName(nullptr, "Open WAV File", "/home", "(*.wav)");
}
