# Wavedit

is a programm where you open a .wav file
and it shows the Fourier Transform of the WAV file. <br/>

You can then click on a specific frequency in the fourier transform diagram and it will remove those frequency values (via IFFT) from the \<name\>.wav file <br/>
and stores the filtered audio in a file called \<name\>_filtered\<frequency filtered\>Hz.wav

> **_Note_** that the filtered data has a smaller amplitude than the original data because a window function (von-Hann window) is applied to the data before taking the Fourier Transform
which reduces [Spectral Leakage](https://en.wikipedia.org/wiki/Spectral_leakage) but also dampens the amplitude of all spectral components a bit.


It uses QT, qcustomplot for plotting. <br/>
For calculating the Fourier Transform the functions fft (and inverse ifft) in fft.h are used. <br/>
To read the WAV files I use the library [AudioFile](https://github.com/adamstark/AudioFile/tree/master)

## Tests

I tried to add unit tests for some but not all methods of class _WavFourier_. <br/>
_WavFourier_ is like the Backend as it provides all the methods for calculating etc., <br/>
the GUI is handled by ```MyWindow.h``` explicitly. ```PlotWidget.h``` contains methods that use qcustomplot to plot the diagrams.
