# TRIX2 based on SDL2 & OpenGL<br/>
### BUILD DEPS FROM SOURCE (FREEBSD, GCC-8):
```sh
cd /usr/ports/devel/gmake && sudo make install clean BATCH=yes
cd /usr/ports/lang/gcc && sudo make install clean BATCH=yes
cd /usr/ports/devel/sdl20 && sudo make install clean BATCH=yes
cd /usr/ports/graphics/glew && sudo make install clean BATCH=yes
cd /usr/ports/math/gsl && sudo make install clean BATCH=yes
cd /usr/ports/graphics/jpeg-turbo && sudo make install clean BATCH=yes
```

#### OR JUST CPY PRE-COMPILED:
```sh
sudo pkg install gcc gmake gsl sdl2 glew jpeg-turbo
```

### BUILD DEPS FROM SOURCE (DEBIAN 11):
```
apt update 
apt install -y make build-essential
tar xfz gsl-2.8.tar.gz
cd gsl-2.8
./configure
make -jN [n - number of vcpu [cat /proc/cpuinfo |grep processor |wc -l]]
make install [from sudo/root [su -l root]]
ln /usr/lib/x86_64-linux-gnu/libgsl.so /usr/lib/x86_64-linux-gnu/libgsl.so.23
cd ..

docker run --rm -it -v .:/src docker.io/gtsh77workshop/deb10-gcc-8-trix2-lib:latest
make
exit
ldd trix2 [should not be any 'not found' links] 
./trix2
```

#### or build docker image with required gcc and libs from Dockerfile localy
```
docker build -f Dockerfile -t deb10-gcc-8-trix2-lib:latest .
docker run --rm -it -v .:/src deb10-gcc-8-trix2-lib:latest
make 
exit
ldd trix2 [should not be any 'not found' links] 
./trix2
```

### BUILD & RUN RENDER:
```sh
make
./trix2
```
### IF RENDER CRASHED:
start a new issue here https://github.com/gtsh77/TRIX2/issues/new <br/>
drag-n-drop trix2.core file there (will be produced in the main folder after crash)<br/><br/>
TRIX1: https://github.com/gtsh77/TRIX