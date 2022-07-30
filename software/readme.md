# 5 Hole Probe Software

Here you will find the software to operate the sensors within the 5 hole probe.

## Usage



## Build (Arduino IDE)

TODO

## Build (Docker)

Start with building the docker image from the `Dockerfile` given within this folder.

```
docker build -t arduino_build .
```

Then start a docker container using the image build before.

```
docker run --name arduino_builder -t -d -v $(pwd):/home/Read3Sensors --restart=always arduino_build
```

Enter the container and build our sketch

```
docker exec -it arduino_builder bash
> cd /home/Read3Sensors
root@container:/home/Read3Sensors> arduino-cli -b Seeeduino:samd:seeed_XIAO_m0 compile
```

To upload our sketch run your container with the `--device=/dev/ttyXXX` option and use

```
root@container:/home/Read3Sensors> arduino-cli -b Seeeduino:samd:seeed_XIAO_m0 compile
root@container:/home/Read3Sensors> arduino-cli -b Seeeduino:samd:seeed_XIAO_m0 upload --port=/dev/ttyXXX
```
