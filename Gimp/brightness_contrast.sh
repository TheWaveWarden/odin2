for d in $(find /home/frot/odinvst/Gimp/ -maxdepth 20 -type d)
do
  pushd $d
  for f in *.png; do
    #convert ./"$f" ./"${f%.jpg}.pdf"
    echo convert ./"$f" -brightness-contrast 4x2  ./"$f"
    convert ./"$f" -brightness-contrast 4x2 ./"$f"
  done
  popd
done