# TRIX2 based on SDL2 & OpenGL<br/>
BUILD DEPS FROM SOURCE:
```sh
cd /usr/ports/devel/gmake && sudo make install clean BATCH=yes
cd /usr/ports/lang/gcc && sudo make install clean BATCH=yes
cd /usr/ports/devel/sdl20 && sudo make install clean BATCH=yes
cd /usr/ports/graphics/glew && sudo make install clean BATCH=yes
cd /usr/ports/math/gsl && sudo make install clean BATCH=yes
cd /usr/ports/graphics/jpeg-turbo && sudo make install clean BATCH=yes
```
OR JUST CPY PRE-COMPILED:
```sh
sudo pkg install gcc gmake gsl sdl2 glew jpeg-turbo
```
SETUP ASSETS LIST:<br/>
1. Copy your asset to assets folder (only jpg supported yet)<br/>
2. Edit src/t2assets.c <br/>
Use only power of two images, at the example below 128x256 used for walls and 64x64 used for the crate<br/><br/>
BUILD & RUN:
```sh
git clone git@github.com:gtsh77/TRIX2.git lovely_folder
cd lovely_folder && gmake
./trix2
```
IF RENDER CRASHED:
start a new issue here https://github.com/gtsh77/TRIX2/issues/new <br/>
drag-n-drop trix2.core file there (will be produced in the main folder after crash)<br/><br/>

3d CURRENT<br/>
![screenshot_2017-12-09_18-19-37](https://user-images.githubusercontent.com/8376353/33796860-0f0da370-dd0e-11e7-9c1a-bdc7da39998e.png)<br />

TRIX1: https://github.com/gtsh77/TRIX