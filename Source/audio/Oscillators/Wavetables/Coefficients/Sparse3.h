#define WT_NR 104
	
m_wavetable_names_1D[WT_NR] = "Sparse3";
m_fourier_coeffs[WT_NR][0][0] = 1.f; //signals sine only
m_fourier_coeffs[WT_NR][1][0] = 0.539144; //Saw scalar (negative to invert for LFO)

m_fourier_coeffs[WT_NR][0][1] = 1.0;
m_fourier_coeffs[WT_NR][0][6] = -0.166667;
m_fourier_coeffs[WT_NR][0][11] = 0.0909091;
m_fourier_coeffs[WT_NR][0][16] = -0.0625;
m_fourier_coeffs[WT_NR][0][21] = 0.047619;
m_fourier_coeffs[WT_NR][0][26] = -0.0384615;
m_fourier_coeffs[WT_NR][0][31] = 0.0322581;
m_fourier_coeffs[WT_NR][0][36] = -0.0277778;
m_fourier_coeffs[WT_NR][0][41] = 0.0243902;
m_fourier_coeffs[WT_NR][0][46] = -0.0217391;
m_fourier_coeffs[WT_NR][0][48] = -0.0208333;
m_fourier_coeffs[WT_NR][0][49] = 0.0204082;
#undef WT_NR
