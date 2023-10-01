FROM ubuntu:latest

RUN apt-get -y update && apt-get install -y

RUN apt-get install sudo

RUN apt-get install -y build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev 

RUN apt-get install -y git
# RUN apt-get install -y              \
#     build-essential                 \
#     libssl-dev                      \
#     gcc                             \
#     make                            \
#     cmake                           \
#     cmake-gui                       \
#     cmake-curses-gui                \
#     automake                        \
#     autoconf                        \
#     libtool                         \
#     doxygen                         \
#     graphviz                        \
#     git                             \
#     gcc-arm-linux-gnueabihf         \
#     g++-arm-linux-gnueabihf

WORKDIR /usr/src/pub


RUN git clone https://github.com/eclipse/paho.mqtt.c.git
WORKDIR /usr/src/pub/paho.mqtt.c
RUN git checkout v1.3.8

RUN cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
    -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
RUN sudo cmake --build build/ --target install
RUN sudo ldconfig


RUN cd .. && git clone https://github.com/eclipse/paho.mqtt.cpp
WORKDIR /usr/src/pub/paho.mqtt.cpp

RUN cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
    -DPAHO_BUILD_DOCUMENTATION=FALSE -DPAHO_BUILD_SAMPLES=TRUE
RUN sudo cmake --build build/ --target install
RUN sudo ldconfig

# RUN cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_DEB_PACKAGE=ON
# RUN cmake --build build
# RUN (cd build && cpack)

#COPY install.sh .
#RUN ./install.sh

WORKDIR /usr/src/pub

COPY pub.cpp .
RUN g++ -o Publisher pub.cpp -lpaho-mqttpp3 -lpaho-mqtt3as

CMD [ "./Publisher" ]