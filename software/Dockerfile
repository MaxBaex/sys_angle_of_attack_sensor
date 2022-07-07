FROM ubuntu:22.04
MAINTAINER Akaflieg_Darmstadt
LABEL name="arduino-cli"

# update and upgrade
RUN apt-get update && apt-get upgrade -y && apt-get clean

# install arduino cli
RUN apt-get install -y curl screen
ADD https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh install.sh
RUN chmod a+x install.sh && ./install.sh

# install seeduino xiao (samd) support
RUN arduino-cli core install --additional-urls https://files.seeedstudio.com/arduino/package_seeeduino_boards_index.json Seeeduino:samd

# install FreeRTOS for SAMD21
RUN arduino-cli lib install FreeRTOS_SAMD21
