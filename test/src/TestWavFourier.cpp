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

	void testIDFT();
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
	QTest::addColumn<QVector<double>>("frequency_bins");

	QTest::newRow("empty data, default sample_rate") << 0 << 1.0 << QVector<double>({});
	QTest::newRow("empty data, sample_rate 10 Hz") << 0 << 10.0 << QVector<double>({});

	QTest::newRow("data.size() = 4, default sample_rate") << 4 << 1.0 << QVector<double>(
		{0, 1.0/4.0}
	);

	QTest::newRow("data.size() = 4, , sample_rate 10 Hz") << 4 << 10.0 << QVector<double>(
		{0, 5.0/2.0}
	);

	QTest::newRow("data.size() = 7, default sample_rate") << 7 << 1.0 << QVector<double>(
		{0, 1.0/7.0, 2.0/7.0}
	);
	QTest::newRow("data.size() = 7, sample_rate 10 Hz") << 7 << 10.0 << QVector<double>(
		{0, 10.0/7.0, 20.0/7.0}
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
	QTest::addColumn<QVector<double>>("dft");

	QTest::newRow("empty data") << QVector<double>({}) << QVector<double>({});

	QTest::newRow("data.size() = 4") << QVector<double>({0.0, 1.0, 2.0, 3.0}) << QVector<double>(
		{6.0, sqrt(8), 2.0, sqrt(8)}
	);

	QTest::newRow("data.size() = 5") << QVector<double>({2.0, 5.0, 1.0, 8.0, 1.0}) << QVector<double>(
		{17.0, 3.441067617579355, 9.008831980409376, 9.008831980409376, 3.441067617579355}
	);
}

void TestWavFourier::testDFT()
{
	QFETCH(QVector<double>, data);
	QFETCH(QVector<double>, dft);

	QVector<double> fourier = wavfourier.DFT(data);
	QCOMPARE(fourier.size(), dft.size());
	for (int i=0, j=0; i<fourier.size() && j<dft.size(); ) {
		QVERIFY(qFuzzyCompare(fourier[i], dft[j]));
		i++; j++;
	}
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
}


QTEST_MAIN(TestWavFourier)
#include "TestWavFourier.moc"
