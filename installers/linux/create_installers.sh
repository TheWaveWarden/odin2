echo "================================================================="
echo "IMPORTANT: Build only on Ubuntu 18.4, else will give libc6 errors"
echo "================================================================="
bash create_deb_package.sh
bash create_bash_installer.sh
