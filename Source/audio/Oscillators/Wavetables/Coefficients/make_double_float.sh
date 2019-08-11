#/bin/bash

#bash script to make ; to f; if needed
#USE WITH GREAT CARE ONLY. ONLY IN THIS FOLDER!!

#replace for all coeffs
sed -i /m_fourier_coeffs/s/";"/"f;"/g *.h
#remove ff if needed
sed -i s/ff/f/g *.h
#remove 1f if needed
sed -i s/1f/1/g *.h
#rebuild coeffs if needed
sed -i s/coefs/coeffs/g *.h

