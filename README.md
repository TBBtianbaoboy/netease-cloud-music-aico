# netease-cloud-music-aico
# Installation
> before install neteasr-cloud-music-aico,please preinstall some necessary libs 
## Centos 7/8
```sh
# install ncurses-devel
wget http://mirror.centos.org/centos/7/os/x86_64/Packages/ncurses-devel-5.9-14.20130511.el7_4.x86_64.rpm
rpm -i ncurses-devel-5.9-14.20130511.el7_4.x86_64.rpm
# install libmpg123-devel
yum install -y mpg123-devel
# install libao-devel
yum install -y libao-devel 
# install libcurl-devel
yum install -y libcurl-devel
```
## Fedora
```sh
dnf install -y \
libao-devel \
libcurl-devel \
ncurses-devel \
libmpg123-devel
```
## Other Linux 
```sh
#Use Google To Resolve The PreInstallation
```
> next,clone this repo to your local directory and complie it by using make with g++
```sh
git clone https://github.com/TBBtianbaoboy/netease-cloud-music-aico.git
cd netease-cloud-music-aico
chmod +x ./configure
./configure
make
```
**Annotation:** The ELF named `aico` in `$HOME/bin`,if this path is not in your `$PATH`,please add it to.
