#define WT_NR 89
	
m_wavetable_names_1D[WT_NR] = "Sparse2";
m_fourier_coeffs[WT_NR][0][0] = 1.f; //signals sine only
m_fourier_coeffs[WT_NR][1][0] = 0.539144; //Saw scalar (negative to invert for LFO)

m_fourier_coeffs[WT_NR][0][1] = 1.0;
m_fourier_coeffs[WT_NR][0][5] = 0.2;
m_fourier_coeffs[WT_NR][0][9] = 0.111111;
m_fourier_coeffs[WT_NR][0][13] = 0.0769231;
m_fourier_coeffs[WT_NR][0][17] = 0.0588235;
m_fourier_coeffs[WT_NR][0][21] = 0.047619;
m_fourier_coeffs[WT_NR][0][25] = 0.04;
m_fourier_coeffs[WT_NR][0][29] = 0.0344828;
m_fourier_coeffs[WT_NR][0][33] = 0.030303;
m_fourier_coeffs[WT_NR][0][37] = 0.027027;
m_fourier_coeffs[WT_NR][0][41] = 0.0243902;
m_fourier_coeffs[WT_NR][0][45] = 0.0222222;
m_fourier_coeffs[WT_NR][0][49] = 0.0204082;
#undef WT_NR
