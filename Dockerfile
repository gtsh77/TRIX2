FROM debian:10 as builder
WORKDIR /src

RUN apt update

RUN apt install -y gcc-8
RUN apt install -y make bash vim 
RUN apt install -y libgsl-dev libglew-dev libsdl2-dev libjpeg-dev

RUN ln -s /usr/bin/gcc-8 /usr/bin/gcc

ENTRYPOINT ["/bin/bash"]