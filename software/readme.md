# 5 Hole Probe Software

Here you will find the software to operate the sensors within the 5 hole probe.
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
cd /home/Read3Sensors
arduino-cli compile
```
