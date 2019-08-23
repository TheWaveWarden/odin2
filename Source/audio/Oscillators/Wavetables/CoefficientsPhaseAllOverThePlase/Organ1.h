#define WT_NR 40

m_wavetable_names_1D[WT_NR] = "Organ1";

m_fourier_coeffs[WT_NR][1][0] = 0.194926f;//TODO scalar

m_fourier_coeffs[WT_NR][0][1] = 1.000000f;
m_fourier_coeffs[WT_NR][0][2] = 0.8000f;
m_fourier_coeffs[WT_NR][0][4] = 0.6f;
m_fourier_coeffs[WT_NR][0][8] = 0.400000f;
m_fourier_coeffs[WT_NR][0][16] = 0.200000f;
m_fourier_coeffs[WT_NR][0][32] = 0.100000f;
m_fourier_coeffs[WT_NR][0][64] = 0.0500000f;

#undef WT_NR
