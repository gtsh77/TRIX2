## TRIX2 based on SDL2 & OpenGL<br/>
### FEATURES
... (comming)
### BUILD
#### A. BSD AS FREEBSD11
1. PREPARE SHARED LIBS:
      1. COMPILE THEM FROM SOURCE USING PORTS:
            ```sh
            cd /usr/ports/devel/gmake && sudo make install clean BATCH=yes
            cd /usr/ports/lang/gcc && sudo make install clean BATCH=yes
            cd /usr/ports/devel/sdl20 && sudo make install clean BATCH=yes
            cd /usr/ports/graphics/glew && sudo make install clean BATCH=yes
            cd /usr/ports/math/gsl && sudo make install clean BATCH=yes
            cd /usr/ports/graphics/jpeg-turbo && sudo make install clean BATCH=yes
            ```
      2. OR FETCH PRE-COMPILED PACKAGES:
            ```sh
            sudo pkg install gcc gmake gsl sdl2 glew jpeg-turbo
            ```
2. PRODUCE  ELF64:
```sh
gmake
```

#### B. LINUX AS DEB11 (LIFE AFTER GCC 10 THAT RUINED (ANSI) C)
1. PREPARE SHARED LIBS:
      1. COMPILE GNU SDL LIB LOCALLY FROM SRC
            ```sh
                  apt update 
                  apt install -y make build-essential
                  tar xfz gsl-2.8.tar.gz
                  cd gsl-2.8
                  ./configure
                  make -jN [n - number of vcpu [cat /proc/cpuinfo |grep processor |wc -l]]
                  make install [from sudo/root [su -l root]]
                  ln /usr/lib/x86_64-linux-gnu/libgsl.so /usr/lib/x86_64-linux-gnu/libgsl.so.23
            ```
      2. PULL AND RUN DOCKER IMAGE BASED ON DEB10, GCC8 WITH PRE-COMPILED REQUIRED PACKAGES
            ```sh
                  docker run --rm -it -v .:/src docker.io/gtsh77workshop/deb10-gcc-8-trix2-lib:latest
                  make
                  exit

            ```
      3. OR BUILD AND RUN DOCKER IMAGE LOCALLY
            ```sh
                  docker build -f Dockerfile -t deb10-gcc-8-trix2-lib:latest .
                  docker run --rm -it -v .:/src deb10-gcc-8-trix2-lib:latest
                  make
                  exit
            ```
2. CHECK LINKS:
```sh
      ldd trix2 [should not be any 'not found' links] 
```

#### C. LINUX (OLD) AS CENTOS7/DEB10
1. PREPARE SHARED LIBS:
      1. INSTALL EPEL REPO
            ```sh
            yum install epel-release
            ```
      2. FETCH PRE-COMPILED PACKAGES (RHEL):
            ```sh
            yum install gcc gsl-devel.x86_64 glew-devel.x86_64 SDL2-devel.x86_64 libjpeg-turbo-devel.x86_64 
            ```
      2. FETCH PRE-COMPILED PACKAGES (DEB10):
            ```sh
            apt install libgsl-dev libglew-dev libsdl2-dev libjpeg-dev
            ```            
2. PRODUCE  ELF64:
```sh
make
```

#### D. WINDOWS
... (comming)

### SETUP ASSETS
1. Copy your assets to `assets` folder (only jpg supported yet, maxsize 2048\*2048)<br/>

### COMPILE MAP
`for map creation you can use any Q3-like map editor (eg. gtk radiant, moh radiant, etc.)`
>restrictions, no support yet for: prefabs, hollow, CSG substract, clipper, free rotation tools

1. Copy your .map to `maps` folder
2. Compile .map into `.cmap`
```sh
./trix2 -c maps/mapname.map
```

### RUN
#### usage
`trix2 [-m <mode> [opt] -f -l -b]`
#### load map w fullscreen example:
```sh
./trix2 -mf load mapname
```
#### FLAGS:
`-m mode` enable special mode
`-f` fullscreen
`-b` calc frame render time
`-l` nolights
  
### BUGS

#### IF RENDER CRASHED:
start a new issue here https://github.com/gtsh77/TRIX2/issues/new <br/>
drag-n-drop trix2.core file there (will be produced in the main folder after crash)<br/><br/>

### EXAMPLES

`MoH:AA dm/mohdm6 (build dec 2001, closed source, Q3 engine based on OpenGL 1.x)`
![github2_2](https://user-images.githubusercontent.com/8376353/35790824-5f7ae6e6-0a55-11e8-9067-7b69c03b2f17.jpg)<br/><br/>
`TRIX2 (opensource, based on OpenGL 3.2)`
![github1_1](https://user-images.githubusercontent.com/8376353/35790825-5f9954f0-0a55-11e8-8123-b4d4cd28db59.jpg)<br/><br/>
TRIX1: https://github.com/gtsh77/TRIX
