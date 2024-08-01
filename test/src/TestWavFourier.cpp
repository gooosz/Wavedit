#include <QTest>

#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../../Wavedit/src/WavFourier.h"

class TestWavFourier : public QObject {
	Q_OBJECT

	QVector<double> *data;	// gets filled before every test
	WavFourier wavfourier;

private slots:
	void init();	// fill data vector
	void helloWorld();

	void testGetStuetzstellen_data();
	void testGetStuetzstellen();

	void testFreq_data();
	void testFreq();

	void testDFT_data();
	void testDFT();

	void test_abs_DFT_data();
	void test_abs_DFT();

	void testIDFT_data();
	void testIDFT();

	void testIDFT_real_data();
	void testIDFT_real();

	void testFFT_data();
	void testFFT();

	void testNextPowOf2_data();
	void testNextPowOf2();

	void testGetBinOfFreq_data();
	void testGetBinOfFreq();
};

void TestWavFourier::helloWorld()
{
	QString str = "Hello";
	QVERIFY(QString::compare(str, "Test", Qt::CaseSensitive));
	QCOMPARE(str, "Hello");
}

void TestWavFourier::init()
{
	data = new QVector<double>();
	for (int i=0; i<4; i++) {
		data->push_back(i);
	}
}

void TestWavFourier::testGetStuetzstellen_data()
{
	QTest::addColumn<int>("size");
	QTest::addColumn<QVector<double>>("stuetzstellen");

	// data.size, correct result are stored in table
	QTest::newRow("empty data") << 0 << QVector<double>({});
	QTest::newRow("data.size() = 4") << 4 << QVector<double>({0, M_PI/2.0, M_PI, 3.0*M_PI/2.0});
	QTest::newRow("data.size() = 7") << 7 << QVector<double>(
		{0, 2.0*M_PI/7.0, 4.0*M_PI/7.0, 6.0*M_PI/7.0, 8.0*M_PI/7.0, 10.0*M_PI/7.0, 12.0*M_PI/7.0}
	);
}

void TestWavFourier::testGetStuetzstellen()
{
	QFETCH(int, size);
	QFETCH(QVector<double>, stuetzstellen);

	QCOMPARE(wavfourier.getStuetzstellen(size), stuetzstellen);
}


void TestWavFourier::testFreq_data()
{
	QTest::addColumn<int>("data_size");
	QTest::addColumn<double>("sample_rate");
	QTest::addColumn<QVector<double>>("frequency_bins");	// k * sample_rate / size

	QTest::newRow("empty data, default sample_rate") << 0 << 1.0 << QVector<double>({});
	QTest::newRow("empty data, sample_rate 10 Hz") << 0 << 10.0 << QVector<double>({});

	QTest::newRow("data.size() = 4, default sample_rate") << 4 << 1.0 << QVector<double>(
		{0, 1.0/4.0, 2.0/4.0, 3.0/4.0}
	);

	QTest::newRow("data.size() = 4, , sample_rate 10 Hz") << 4 << 10.0 << QVector<double>(
		{0, 10.0/4.0, 20.0/4.0, 30.0/4.0}
	);

	QTest::newRow("data.size() = 7, default sample_rate") << 7 << 1.0 << QVector<double>(
		{0, 1.0/7.0, 2.0/7.0, 3.0/7.0, 4.0/7.0, 5.0/7.0, 6.0/7.0}
	);
	QTest::newRow("data.size() = 7, sample_rate 10 Hz") << 7 << 10.0 << QVector<double>(
		{0, 10.0/7.0, 20.0/7.0, 30.0/7.0, 40.0/7.0, 50.0/7.0, 60.0/7.0}
	);
}

void TestWavFourier::testFreq()
{
	QFETCH(int, data_size);
	QFETCH(double, sample_rate);
	QFETCH(QVector<double>, frequency_bins);

	QCOMPARE(wavfourier.Freq(data_size, sample_rate), frequency_bins);
}


void TestWavFourier::testDFT_data()
{
	QTest::addColumn<QVector<double>>("data");
	QTest::addColumn<QVector<complex>>("dft");

	QTest::newRow("empty data") << QVector<double>({}) << QVector<complex>({});

	QTest::newRow("data.size() = 4") << QVector<double>({0.0, 1.0, 2.0, 3.0}) << QVector<complex>(
		{complex(6.0,0.0), complex(-2.0,2.0), complex(-2.0,0.0), complex(-2.0,-2.0)}
	);

	QTest::newRow("data.size() = 5") << QVector<double>({2.0, 5.0, 1.0, 8.0, 1.0}) << QVector<complex>(
		{complex(17.0, 0.0), complex(-3.4270509831248424,0.3102707008666963),
		 complex(-0.07294901687515476,-9.008536623235965), complex(-0.07294901687516075,9.008536623235967),
		 complex(-3.4270509831248375,-0.31027070086670205)}
	);
}

#include <iomanip>

void TestWavFourier::testDFT()
{
	QFETCH(QVector<double>, data);
	QFETCH(QVector<complex>, dft);

	/*std::cout << "fourier.size(): " << fourier.size() << '\n';
	std::for_each(fourier.begin(), fourier.end(), [](complex d){ std::cout << std::setprecision(20) << d << '\n'; });
	std::cout << "-----" << '\n';
	std::cout << "dft.size(): " << dft.size() << '\n';
	std::for_each(dft.begin(), dft.end(), [](complex d){ std::cout << std::setprecision(20) << d << '\n'; });
	for (int i=0, j=0; i<fourier.size() || j<dft.size(); ) {
		if (fourier[i] != dft[j]) {
			std::cout << i << "," << j << ": " << std::setprecision(20) << fourier[i] << " != "<< dft[j] << '\t'
				<< "|" << fourier[i] - dft[j] << "| = "
				<< std::abs(fourier[i] - dft[j]) << " < "
				<< qMin(qAbs(fourier[i].imag()), qAbs(dft[j].imag())) << " ? "
				<< (std::abs(fourier[i] - dft[j]) < qMin(qAbs(fourier[i].imag()), qAbs(dft[j].imag())))
				<< '\n';
		} else {
			std::cout << i << "," << j << ": " << fourier[i] << " == " << dft[j] << '\n';
		}
		i++; j++;
	}*/

	QVector<complex> fourier = wavfourier.DFT(data);
	QCOMPARE(fourier.size(), dft.size());
	for (int i=0, j=0; i<fourier.size() && j<dft.size(); ) {
		// qFuzzyCompare doesn't work for complex numbers, so compare real and imaginary part
		/*
		 * qFuzzyCompare fails when one of number is 0.0
		 * https://doc.qt.io/qt-5/qtglobal.html#qFuzzyCompare recommends
		 * if one number is likely ot be 0.0, then add 1 to both numbers
		 * which doesn't effect equality but solves the issue of the way
		 * qFuzzyCompare is implemented
		*/
		QVERIFY(qFuzzyCompare(fourier[i].real()+1, dft[j].real()+1));
		QVERIFY(qFuzzyCompare(fourier[i].imag()+1, dft[j].imag()+1));
		i++; j++;
	}
}


void TestWavFourier::test_abs_DFT_data()
{
	QTest::addColumn<QVector<double>>("data");
	QTest::addColumn<QVector<double>>("abs_dft");

	QTest::newRow("empty data") << QVector<double>({}) << QVector<double>({});

	QTest::newRow("data.size() = 4") << QVector<double>({0.0, 1.0, 2.0, 3.0}) << QVector<double>(
		{6.0, sqrt(8), 2.0, sqrt(8)}
	);

	QTest::newRow("data.size() = 5") << QVector<double>({2.0, 5.0, 1.0, 8.0, 1.0}) << QVector<double>(
		{17.0, 3.441067617579355, 9.008831980409376, 9.008831980409376, 3.441067617579355}
	);
}

void TestWavFourier::test_abs_DFT()
{
	QFETCH(QVector<double>, data);
	QFETCH(QVector<double>, abs_dft);

	QVector<complex> fourier = wavfourier.DFT(data);
	QVector<double> abs_fourier = wavfourier.abs(fourier);

	QCOMPARE(abs_fourier.size(), abs_dft.size());
	for (int i=0, j=0; i<abs_fourier.size() && j<abs_dft.size(); ) {
		QVERIFY(qFuzzyCompare(abs_fourier[i], abs_dft[j]));
		i++; j++;
	}
}


void TestWavFourier::testIDFT_data()
{
	QTest::addColumn<QVector<complex>>("dft");
	QTest::addColumn<QVector<complex>>("data");

	QTest::newRow("empty data") << QVector<complex>({}) << QVector<complex>({});

	QTest::newRow("data.size() = 4") << QVector<complex>({	complex(6.0,0.0),
								complex(-2.0,2.0),
								complex(-2.0,0.0),
								complex(-2.0,-2.0)})
		<< QVector<complex>({	complex(0.0, 0.0),
					complex(1.0, 0.0),
					complex(2.0, 0.0),
					complex(3.0, 0.0)}
	);

	QTest::newRow("data.size() = 5") << QVector<complex>(
		{complex(17.0, 0.0),
		 complex(-3.4270509831248424,0.3102707008666963),
		 complex(-0.07294901687515476,-9.008536623235965),
		 complex(-0.07294901687516075,9.008536623235967),
		 complex(-3.4270509831248375,-0.31027070086670205)})
		<< QVector<complex>(
			{complex(2.0, 0.0),
			 complex(5.0, 0.0),
			 complex(1.0, 0.0),
			 complex(8.0, 0.0),
			 complex(1.0, 0.0)}
	);
}

void TestWavFourier::testIDFT()
{
	/*std::cout << "fourier.size(): " << fourier.size() << '\n';
	std::for_each(fourier.begin(), fourier.end(), [](double d){ std::cout << d << '\n'; });
	std::cout << "-----" << '\n';
	std::cout << "dft.size(): " << dft.size() << '\n';
	std::for_each(dft.begin(), dft.end(), [](double d){ std::cout << d << '\n'; });*/
	/*
	for (int i=0, j=0; i<fourier.size() || j<dft.size(); ) {
		if (fourier[i] != dft[j]) {
			std::cout << i << "," << j << ": " << std::setprecision(15) << fourier[i] << " != "
				<< std::setprecision(15) << dft[j]
				<< '\t' << std::setprecision(15)
				<< "|" << fourier[i] - dft[j] << "| < " << std::numeric_limits<double>::epsilon() << " ? "
				<< (std::abs(fourier[i] - dft[j]) < std::numeric_limits<double>::epsilon())
				<< '\n';
		} else {
			std::cout << i << "," << j << ": " << fourier[i] << " == " << dft[j] << '\n';
		}
		i++; j++;
	}*/
	QFETCH(QVector<complex>, dft);
	QFETCH(QVector<complex>, data);

	QVector<complex> idft = wavfourier.IDFT(dft);
	QCOMPARE(idft.size(), data.size());
	for (int i=0, j=0; i<idft.size() && j<data.size(); ) {
		// qFuzzyCompare doesn't work for complex numbers, so compare real and imaginary part
		/*
		 * qFuzzyCompare fails when one of number is 0.0
		 * https://doc.qt.io/qt-5/qtglobal.html#qFuzzyCompare recommends
		 * if one number is likely ot be 0.0, then add 1 to both numbers
		 * which doesn't effect equality but solves the issue of the way
		 * qFuzzyCompare is implemented
		*/
		QVERIFY(qFuzzyCompare(idft[i].real()+1, data[j].real()+1));
		QVERIFY(qFuzzyCompare(idft[i].imag()+1, data[j].imag()+1));
		i++; j++;
	}
}


void TestWavFourier::testIDFT_real_data()
{
	QTest::addColumn<QVector<complex>>("dft");
	QTest::addColumn<QVector<double>>("data");

	QTest::newRow("empty data") << QVector<complex>({}) << QVector<double>({});

	QTest::newRow("data.size() = 4") << QVector<complex>({	complex(6.0,0.0),
								complex(-2.0,2.0),
								complex(-2.0,0.0),
								complex(-2.0,-2.0)})
		<< QVector<double>({0.0, 1.0, 2.0, 3.0}
	);

	QTest::newRow("data.size() = 5") << QVector<complex>(
		{complex(17.0, 0.0),
		 complex(-3.4270509831248424,0.3102707008666963),
		 complex(-0.07294901687515476,-9.008536623235965),
		 complex(-0.07294901687516075,9.008536623235967),
		 complex(-3.4270509831248375,-0.31027070086670205)})
		<< QVector<double>({2.0, 5.0, 1.0, 8.0, 1.0}
	);
}

void TestWavFourier::testIDFT_real()
{
	QFETCH(QVector<complex>, dft);
	QFETCH(QVector<double>, data);

	QVector<double> idft = wavfourier.IDFT_real(dft);

	QCOMPARE(idft.size(), data.size());
	for (int i=0, j=0; i<idft.size() && j<data.size(); ) {
		QVERIFY(qFuzzyCompare(idft[i], data[j]));
		i++; j++;
	}
}


void TestWavFourier::testFFT_data()
{
	QTest::addColumn<QVector<double>>("data");
	QTest::addColumn<QVector<complex>>("fft");

	QTest::newRow("empty data") << QVector<double>({}) << QVector<complex>({});

	// TODO: DFT, FFT fails because of window function, calculate the correct result here again

	QTest::newRow("data.size() = 4") << QVector<double>({0.0, 1.0, 2.0, 3.0}) << QVector<complex>(
		{complex(6.0,0.0), complex(-2.0,2.0), complex(-2.0,0.0), complex(-2.0,-2.0)}
	);

	QTest::newRow("data.size() = 5") << QVector<double>({2.0, 5.0, 1.0, 8.0, 1.0}) <<
	/*QVector<complex>(
		{complex(17.0, 0.0),
		 complex(-3.4270509831248424,0.3102707008666963),
		 complex(-0.07294901687515476,-9.008536623235965), complex(-0.07294901687516075,9.008536623235967),
		 complex(-3.4270509831248375,-0.31027070086670205)}
	);*/
	QVector<complex>(
		{complex(17.000000,0.000000),
		 complex(-1.121320,-10.192388),
		 complex(2.0000000,3.0000000),
		 complex(3.1213200,-8.192388),
		 complex(-9.000000,0.0000000),
		 complex(3.1213200,8.1923880),
		 complex(2.0000000,-3.000000),
		 complex(-1.121320,10.192388)}
	);
}

void TestWavFourier::testFFT()
{
	QFETCH(QVector<double>, data);
	QFETCH(QVector<complex>, fft);

	/*QVector<complex> fourier = wavfourier.FFT(data);
	std::cout << "fourier.size(): " << fourier.size() << '\n';
	std::for_each(fourier.begin(), fourier.end(), [](complex d){
		std::cout << std::setprecision(20) << d << '\n';

	});
	std::cout << "-----" << '\n';
	std::cout << "dft.size(): " << fft.size() << '\n';
	std::for_each(fft.begin(), fft.end(), [](complex d){ std::cout << std::setprecision(20) << d << '\n'; });
	for (int i=0, j=0; i<fourier.size() || j<fft.size(); ) {
		if (fourier[i] != fft[j]) {
			std::cout << i << "," << j << ": " << std::setprecision(20) << fourier[i] << " != "<< fft[j] << '\t'
				<< "|" << fourier[i] - fft[j] << "| = "
				<< std::abs(fourier[i] - fft[j]) << " < "
				<< qMin(qAbs(fourier[i].imag()), qAbs(fft[j].imag())) << " ? "
				<< (std::abs(fourier[i] - fft[j]) < qMin(qAbs(fourier[i].imag()), qAbs(fft[j].imag())))
				<< '\n';
		} else {
			std::cout << i << "," << j << ": " << fourier[i] << " == " << fft[j] << '\n';
		}
		i++; j++;
	}
	for (int i=0,j=0; i<fourier.size() && j<fft.size(); ) {
		if (!qFuzzyCompare((float)fourier[i].real()+1, (float)fft[j].real()+1))
			std::cout << "qFuzzyCompare failed for real for (" << i << ", " << j << ")\n";
		if (!qFuzzyCompare((float)fourier[i].imag()+1, (float)fft[j].imag()+1))
			std::cout << "qFuzzyCompare failed for imag for (" << i << ", " << j << ")\n";
		i++; j++;
	}*/

	QVector<complex> fourier = wavfourier.FFT(data);
	QCOMPARE(fourier.size(), fft.size());
	for (int i=0, j=0; i<fourier.size() && j<fft.size(); ) {
		// qFuzzyCompare doesn't work for complex numbers, so compare real and imaginary part
		/*
		 * qFuzzyCompare fails when one of number is 0.0
		 * https://doc.qt.io/qt-5/qtglobal.html#qFuzzyCompare recommends
		 * if one number is likely ot be 0.0, then add 1 to both numbers
		 * which doesn't effect equality but solves the issue of the way
		 * qFuzzyCompare is implemented
		 *
		 * https://scistatcalc.blogspot.com/2013/12/fft-calculator.html calculated the
		 * correct result of FFT, however they only returned floats, so cast my result to float
		 * or else the different precisions make qFuzzyCompare fail
		*/
		QVERIFY(qFuzzyCompare((float)fourier[i].real()+1, (float)fft[j].real()+1));
		QVERIFY(qFuzzyCompare((float)fourier[i].imag()+1, (float)fft[j].imag()+1));
		i++; j++;
	}
}


void TestWavFourier::testNextPowOf2_data()
{
	QTest::addColumn<int>("n");
	QTest::addColumn<int>("nextPow2");

	QTest::addRow("n = 0") << 0 << 0;	// special case 0
	QTest::addRow("n not power of 2") << 11 << 16;
	QTest::addRow("n already power of 2") << 8 << 8;
}

void TestWavFourier::testNextPowOf2()
{
	QFETCH(int, n);
	QFETCH(int, nextPow2);

	QCOMPARE(nextPowOf2(n), nextPow2);
}


void TestWavFourier::testGetBinOfFreq_data()
{
	QTest::addColumn<QVector<double>>("freqBins");
	QTest::addColumn<double>("frequency");
	QTest::addColumn<int>("index_of_bin");

	QTest::addRow("0 frequency bins")
		<< QVector<double>({})
		<< 2.3
		<< -1;

	QTest::addRow("6 frequency bins, frequency in vec")
		<< QVector<double>({0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0})
		<< 2.3
		<< 5;

	QTest::addRow("6 frequency bins, frequency before vec")
		<< QVector<double>({0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0})
		<< -1.8
		<< 0;

	QTest::addRow("6 frequency bins, frequency after vec")
		<< QVector<double>({0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0})
		<< 6.4
		<< -1;
}

void TestWavFourier::testGetBinOfFreq()
{
	QFETCH(QVector<double>, freqBins);
	QFETCH(double, frequency);
	QFETCH(int, index_of_bin);

	QCOMPARE(wavfourier.getBinOfFreq(freqBins, frequency), index_of_bin);
}






QTEST_MAIN(TestWavFourier)
#include "TestWavFourier.moc"
