#/bin/bash

#bash script to make ; to f; if needed
#USE WITH GREAT CARE ONLY. ONLY IN THIS FOLDER!!

#even coeffs
sed -i /[0]/s/";"/"f;"/g *.h
#uneven coeffs
sed -i /[1]/s/";"/"f;"/g *.h
#remove ff if needed
sed -i s/ff/f/g *.h

