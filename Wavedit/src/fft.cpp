#include "fft.h"
 
// From http://rosettacode.org/wiki/Fast_Fourier_transform

// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
// Better optimized but less intuitive
// !!! Warning : in some cases this code make result different from not optimased version above (need to fix bug)
// The bug is now fixed @2017/05/30 
void fft::fft(QVector<complex> &x)
{
    // DFT
    unsigned int N = x.size(), k = N, n;
    double thetaT = M_PI / N;
    complex phiT = complex(cos(thetaT), -sin(thetaT));
    complex T;
    while (k > 1)
    {
        n = k;
        k >>= 1;
        phiT = phiT * phiT;
        T = 1.0L;
        for (unsigned int l = 0; l < k; l++)
        {
            for (unsigned int a = l; a < N; a += n)
            {
                unsigned int b = a + k;
                complex t = x[a] - x[b];
                x[a] += x[b];
                x[b] = t * T;
            }
            T *= phiT;
        }
    }
    // Decimate
    unsigned int m = (unsigned int)log2(N);
    for (unsigned int a = 0; a < N; a++)
    {
        unsigned int b = a;
        // Reverse bits
        b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
        b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
        b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
        b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
        b = ((b >> 16) | (b << 16)) >> (32 - m);
        if (b > a)
        {
            complex t = x[a];
            x[a] = x[b];
            x[b] = t;
        }
    }
    //// Normalize (This section make it not working correctly)
    //Complex f = 1.0 / sqrt(N);
    //for (unsigned int i = 0; i < N; i++)
    //    x[i] *= f;
}
 
// inverse fft (in-place)
void fft::ifft(QVector<complex>& x)
{
    // conjugate the complex numbers
    //x = x.apply(std::conj);
    for (auto& el : x) {
        el = std::conj(el);
    }
 
    // forward fft
    fft( x );
 
    // conjugate the complex numbers again
    //x = x.apply(std::conj);
    for (auto& el : x) {
        el = std::conj(el);
        el /= x.size();
    }

    // scale the numbers
    //x /= x.size();

}

QVector<complex> fft::mul_poly(QVector<complex> &x, QVector<complex> &y) {
    int n = 1;
    int deg = x.size() + y.size();
    while (n < deg) {
        n = n << 1;
    }
    QVector<complex> padX(n);
    QVector<complex> padY(n);
    QVector<complex> z(n);
    for (int i = 0; i < x.size(); i++) {
        padX[i] = x[i];
    }
    for (int i = 0; i < y.size(); i++) {
        padY[i] = y[i];
    }
    fft(padX);
    fft(padY);
    for (int i=0; i<z.size(); i++) {
        z[i] = padX[i] * padY[i];
    }
    ifft(z);
    return z;
}

QVector<complex> fft::sdp(QVector<complex> &x, QVector<complex> &y) {
    int n = x.size();
    int m = y.size();
    QVector<complex> revY(m);
    for (int i = 0; i < m; i++) {
        revY[i] = y[m-i-1];
    }
    QVector<complex> z = mul_poly(x, revY);

    QVector<complex> ret;
    for (int i=m-1; i<n-m+1; i++) {
        ret.push_back(z[i]);
    }
    // return std::valarray<complex>(z[std::slice(m - 1, n - m + 1, 1)])
    return ret;
}

std::vector<int> fft::int_sdp(std::vector<int> x, std::vector<int> y) {
    QVector<complex> cx(x.size());
    QVector<complex> cy(y.size());
    for (int i = 0; i < x.size(); i++) {
        cx[i] = complex(x[i], 0);
    }
    for (int i = 0; i < y.size(); i++) {
        cy[i] = complex(y[i], 0);
    }
    QVector<complex> cz = sdp(cx, cy);
    std::vector<int> z(cz.size());
    for (int i = 0; i < z.size(); i++) {
        z[i] = int(cz[i].real() + 0.5);
    }
    return z;
}
 
/*int main()
{
    const Complex a[] = { 3.0, 1.0, 1.0, 1.0, 4.0, 1.0 };
    const Complex b[] = { 1.0, 2.0, 3.0 };
    CArray x(a, 6);
    CArray y(b, 3);
    CArray z = sdp(x, y);
    for (int i = 0; i < z.size(); i++) {
        cout << z[i] << endl;
    }
    int ia[] = {3, 1, 1, 1, 4, 1};
    int ib[] = {1, 2, 3};
    vector<int> va(ia, ia + 6);
    vector<int> vb(ib, ib + 3);
    vector<int> vc = int_sdp(va, vb);
    for (int i = 0; i < vc.size(); i++) {
        cout << vc[i] << endl;
    }
    return 0;
}*/
