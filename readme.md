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
BUILD & RUN RENDER:
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