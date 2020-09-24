echo "================================================================="
echo "IMPORTANT: Build only on Ubuntu 18.4, else will give libc6 errors"
echo "================================================================="
echo ""
echo "running deb creator script"
bash create_deb_package.sh
echo ""
echo "running bash creator script"
bash create_bash_installer.sh
