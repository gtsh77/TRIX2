## TRIX2 based on SDL2 & OpenGL<br/>
### BUILD
#### BSD
1. PREPARE SHARED LIBS:
      1. COMPILE THEM FROM SOURCE:
      ```sh
      cd /usr/ports/devel/gmake && sudo make install clean BATCH=yes
      cd /usr/ports/lang/gcc && sudo make install clean BATCH=yes
      cd /usr/ports/devel/sdl20 && sudo make install clean BATCH=yes
      cd /usr/ports/graphics/glew && sudo make install clean BATCH=yes
      cd /usr/ports/math/gsl && sudo make install clean BATCH=yes
      cd /usr/ports/graphics/jpeg-turbo && sudo make install clean BATCH=yes
      ```
      2. OR CPY PRE-COMPILED:
      ```sh
      sudo pkg install gcc gmake gsl sdl2 glew jpeg-turbo
      ```
2. CLONE LATEST VER:
```sh
git clone git@github.com:gtsh77/TRIX2.git lovely_folder && cd lovely_folder
```
3. PRODUCE  ELF64:
```sh
gmake
```

### SETUP ASSETS
1. Copy your assets to `assets` folder (only jpg supported yet, maxsize 2048\*2048)<br/>
2. Edit src/t2assets.c <br/>
Use only power of two images, at the example below 128x256 used for walls and 64x64 used for the crate<br/><br/>
BUILD & RUN:
```sh
git clone git@github.com:gtsh77/TRIX2.git lovely_folder
cd lovely_folder && gmake
./trix2
```

### COMPILE MAP
`for map creation you can use any Q3-like map editor (eg. gtk radiant, moh radiant, etc.)`
>restrictions, no support yet for: prefabs, hollow, CSG substract, clipper, free rotation tools

1. Copy your .map to `maps` folder
2. Compile .map into `.cmap`
```sh
./trix2 -c maps/mapname.map
```

### RUN
>sage: trix2 [-m <mode> [opt] -f -]
  
  
IF RENDER CRASHED:
start a new issue here https://github.com/gtsh77/TRIX2/issues/new <br/>
drag-n-drop trix2.core file there (will be produced in the main folder after crash)<br/><br/>

`MoH:AA dm/mohdm6 (build dec 2001, closed source, Q3 engine based on OpenGL 1.x)`
![github2_2](https://user-images.githubusercontent.com/8376353/35790824-5f7ae6e6-0a55-11e8-9067-7b69c03b2f17.jpg)<br/><br/>
`TRIX2 (opensource, based on OpenGL 3.2)`
![github1_1](https://user-images.githubusercontent.com/8376353/35790825-5f9954f0-0a55-11e8-8123-b4d4cd28db59.jpg)<br/><br/>
TRIX1: https://github.com/gtsh77/TRIX
