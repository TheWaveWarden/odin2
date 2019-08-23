#define WT_NR 0
m_wavetable_names_1D[WT_NR] = "Sine";

m_fourier_coeffs[WT_NR][0][0] = 1.f; //signals sine only
m_fourier_coeffs[WT_NR][1][0] = 1.f; //normalization factor

m_fourier_coeffs[WT_NR][0][1] = 1.f; //sin(x)

#undef WT_NR