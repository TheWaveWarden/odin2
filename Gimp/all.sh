bash crop.sh
bash knob.sh
echo "apply postprocessing"
bash apply.sh
echo "copy files"
bash copy.sh
echo "apply brightness-contrast"
bash brightness_contrast.sh