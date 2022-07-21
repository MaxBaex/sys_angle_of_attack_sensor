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

# install SD library
RUN arduino-cli lib install SD

# ugly patching of Seeeduino's FreeRTOS
# previous version had a byte alignment issue, causing problems with sprintf
RUN apt-get install -y unzip
ADD https://github.com/Seeed-Studio/Seeed_Arduino_FreeRTOS/archive/e4847b697c5062dd21cdda6235640897988ff663.zip FreeRTOS_SAMD21.zip
RUN rm -rf /root/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/libraries/Seeed_Arduino_FreeRTOS && unzip FreeRTOS_SAMD21.zip && mv Seeed_Arduino_FreeRTOS-e4847b697c5062dd21cdda6235640897988ff663 /root/.arduino15/packages/Seeeduino/hardware/samd/1.8.3/libraries/Seeed_Arduino_FreeRTOS

