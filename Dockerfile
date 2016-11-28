FROM phusion/baseimage

CMD ["/sbin/my_init"]

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install curl build-essentials bison git cmake zlib1g-dev pkg-config -y && \
    apt-get autoclean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN git clone https://github.com/cmusphinx/sphinxbase
RUN cd /sphinxbase && \
    ./autogen.sh && \
    ./configure --enable-fixed --without-lapack && \
    make && \
    make check && \
    make install

RUN git clone https://github.com/cmusphinx/pocketsphinx
RUN cd /pocketsphinx && \
    ./autogen.sh && \
    ./configure && \
    make clean all && \
    make check && \
    make install

RUN git clone https://github.com/lpereira/lwan
RUN cd /lwan && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make && \
    make install


RUN git clone https://github.com/ChrisRx/stt_server
RUN cd /stt_server && \
    make

CMD ["/stt_server/bin/stt_server"]
