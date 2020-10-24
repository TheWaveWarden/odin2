#!/bin/bash
echo "=================================================================="
echo "IMPORTANT: Build only on Ubuntu 18.04, else will give libc6 errors"
echo "=================================================================="
echo ""
echo "removing old builds from directory..."
rm Odin*
echo ""
echo "running deb creator script"
#package version can be passed as first argument for CI, if not it must be input
if [ "$#" -eq 1 ]; then
    bash create_deb_package.sh $1
else 
    bash create_deb_package.sh
fi
echo ""
echo "running bash creator script"
bash create_bash_installer.sh
