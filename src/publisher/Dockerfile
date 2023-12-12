#FROM ubuntu:latest

FROM gcc:latest

RUN apt-get -y update

RUN apt-get install cmake -y

WORKDIR /usr/src/pub

RUN git clone https://github.com/eclipse/paho.mqtt.c.git
WORKDIR /usr/src/pub/paho.mqtt.c
RUN git checkout v1.3.8

RUN cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
    -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
RUN cmake --build build/ --target install
RUN ldconfig


RUN cd .. && git clone https://github.com/eclipse/paho.mqtt.cpp
WORKDIR /usr/src/pub/paho.mqtt.cpp

RUN cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
    -DPAHO_BUILD_DOCUMENTATION=FALSE -DPAHO_BUILD_SAMPLES=TRUE
RUN cmake --build build/ --target install
RUN ldconfig

# Install RapidJSON
WORKDIR /usr/src
RUN git clone https://github.com/Tencent/rapidjson.git
WORKDIR /usr/src/rapidjson
RUN cmake -Bbuild -H.
RUN cmake --build build --target install
RUN ldconfig

# RUN cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_DEB_PACKAGE=ON
# RUN cmake --build build
# RUN (cd build && cpack)

#COPY install.sh .
#RUN ./install.sh

WORKDIR /usr/src/pub
COPY bottles.json .
COPY pub.cpp .
RUN g++ -o Publisher pub.cpp -lpaho-mqttpp3 -lpaho-mqtt3as

CMD [ "./Publisher" ]