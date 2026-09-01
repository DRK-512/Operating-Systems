FROM ubuntu:22.04

RUN apt update -y && \
    apt upgrade -y && \
    apt dist-upgrade -y 

RUN apt install \
cmake \
build-essential \
vim \
tmux \
libudev-dev \
libevdev-dev \
libevdev2 \
pkg-config \
-y

# Cleanup
RUN apt autoremove -y && \
    apt autoclean -y 

COPY ./export/ /root/export/

RUN mkdir /root/build && \
    cd /root/build && \
    cmake ../export/ && \
    make && \
    mv Keylogger /root/ && \
    rm -rf /root/build /root/export

CMD ["/bin/bash"]

