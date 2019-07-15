
echo 'cropping knob frames'

#zero padded loop 1 to 256
for i in $(seq -f "%04g" 1 256)
do
    echo $i
    convert knobs/raw/frame$i.png -crop 33x35+32+110 cropped/knobs/metal1/frame$i.png
    convert knobs/raw/frame$i.png -crop 39x42+129+58 cropped/knobs/metal2/frame$i.png
    convert knobs/raw/frame$i.png -crop 45x48+75+55 cropped/knobs/metal3/frame$i.png
    convert knobs/raw/frame$i.png -crop 21x24+89+114 cropped/knobs/black1/frame$i.png
    convert knobs/raw/frame$i.png -crop 27x30+36+62 cropped/knobs/black2/frame$i.png
    convert knobs/raw/frame$i.png -crop 29x32+85+11 cropped/knobs/black3/frame$i.png
    convert knobs/raw/frame$i.png -crop 32x35+33+10 cropped/knobs/black4/frame$i.png
    convert knobs/raw/frame$i.png -crop 36x39+131+8 cropped/knobs/round/frame$i.png
    convert knobs/raw/frame$i.png -crop 17x78+180+36 cropped/knobs/modwheel/frame$i.png
done  

echo 'make film strips'

convert -append cropped/knobs/metal1/frame0001.png cropped/knobs/metal1/frame0002.png cropped/knobs/metal1/metal_knob_small.png 
convert -append cropped/knobs/metal2/frame0001.png cropped/knobs/metal2/frame0002.png cropped/knobs/metal2/metal_knob_mid.png 
convert -append cropped/knobs/metal3/frame0001.png cropped/knobs/metal3/frame0002.png cropped/knobs/metal3/metal_knob_big.png 
convert -append cropped/knobs/black1/frame0001.png cropped/knobs/black1/frame0002.png cropped/knobs/black1/black_knob_very_small.png 
convert -append cropped/knobs/black2/frame0001.png cropped/knobs/black2/frame0002.png cropped/knobs/black2/black_knob_small.png 
convert -append cropped/knobs/black3/frame0001.png cropped/knobs/black3/frame0002.png cropped/knobs/black3/black_knob_mid.png 
convert -append cropped/knobs/black4/frame0001.png cropped/knobs/black4/frame0002.png cropped/knobs/black4/black_knob_big.png 
convert -append cropped/knobs/round/frame0001.png cropped/knobs/round/frame0002.png cropped/knobs/round/round_knob.png 
convert +append cropped/knobs/modwheel/frame0001.png cropped/knobs/modwheel/frame0002.png cropped/knobs/modwheel/modwheel.png 


for i in $(seq -f "%04g" 3 256)
do
     echo $i    
     convert -append cropped/knobs/metal1/metal_knob_small.png cropped/knobs/metal1/frame$i.png cropped/knobs/metal1/metal_knob_small.png 
     convert -append cropped/knobs/metal2/metal_knob_mid.png cropped/knobs/metal2/frame$i.png cropped/knobs/metal2/metal_knob_mid.png 
     convert -append cropped/knobs/metal3/metal_knob_big.png cropped/knobs/metal3/frame$i.png cropped/knobs/metal3/metal_knob_big.png
     convert -append cropped/knobs/black1/black_knob_very_small.png cropped/knobs/black1/frame$i.png cropped/knobs/black1/black_knob_very_small.png 
     convert -append cropped/knobs/black2/black_knob_small.png cropped/knobs/black2/frame$i.png cropped/knobs/black2/black_knob_small.png 
     convert -append cropped/knobs/black3/black_knob_mid.png cropped/knobs/black3/frame$i.png cropped/knobs/black3/black_knob_mid.png 
     convert -append cropped/knobs/black4/black_knob_big.png cropped/knobs/black4/frame$i.png cropped/knobs/black4/black_knob_big.png 
     convert -append cropped/knobs/round/round_knob.png cropped/knobs/round/frame$i.png cropped/knobs/round/round_knob.png 
     convert +append cropped/knobs/modwheel/modwheel.png cropped/knobs/modwheel/frame$i.png cropped/knobs/modwheel/modwheel.png 
#    convert knobs/raw/frame_$i.png -crop 33x35+32+110 cropped/knobs/metal1/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 39x42+129+58 cropped/knobs/metal2/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 45x48+75+55 cropped/knobs/metal3/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 21x24+89+114 cropped/knobs/black1/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 27x30+36+62 cropped/knobs/black2/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 29x32+85+11 cropped/knobs/black3/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 32x35+33+10 cropped/knobs/black4/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 36x39+131+8 cropped/knobs/round/frame_$i.png
#    convert knobs/raw/frame_$i.png -crop 23x94+175+28 cropped/knobs/modwheel/frame_$i.png
done  


rm cropped/knobs/metal1/frame*
rm cropped/knobs/metal2/frame*
rm cropped/knobs/metal3/frame*
rm cropped/knobs/black1/frame*
rm cropped/knobs/black2/frame*
rm cropped/knobs/black3/frame*
rm cropped/knobs/black4/frame*
rm cropped/knobs/round/frame*
rm cropped/knobs/modwheel/frame*