#ifndef FFT_H
#define FFT_H

#include <complex>
#include <vector>
#include <valarray>
#include <QVector>


using complex = std::complex<double>;
typedef std::valarray<complex> CArray;

namespace fft {

void fft(QVector<complex> &x); // Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
void ifft(QVector<complex>& x); // inverse fft (in-place)

QVector<complex> mul_poly(QVector<complex> &x, QVector<complex> &y);
QVector<complex> sdp(QVector<complex> &x, QVector<complex> &y);
std::vector<int> int_sdp(std::vector<int> x, std::vector<int> y);

}


#endif // FFT_H
