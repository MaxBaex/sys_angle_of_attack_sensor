import board
import digitalio
import time
import busio
import math
import adafruit_pcf8523
import os
import adafruit_sdcard
import storage
import microcontroller
import time
import neopixel

#I2C Schnittstelle
i2c = board.I2C()

#UART Schnittstelle
uart = busio.UART(board.TX, board.RX, baudrate=115200)

#RealTimeClock vorbereiten
rtc = adafruit_pcf8523.PCF8523(i2c)
days = ("Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday")
t = rtc.datetime

#blaue LED vorbereiten
led = digitalio.DigitalInOut(board.BLUE_LED)
led.direction = digitalio.Direction.OUTPUT
#Grüne LED
led1 =digitalio.DigitalInOut(board.A0)
led1.direction = digitalio.Direction.OUTPUT
#gelbe LED
led2 =digitalio.DigitalInOut(board.A1)
led2.direction = digitalio.Direction.OUTPUT
#rote LED
led3 =digitalio.DigitalInOut(board.A2)
led3.direction = digitalio.Direction.OUTPUT
#Schalter
button = digitalio.DigitalInOut(board.A3)
button.switch_to_input(pull=digitalio.Pull.UP)

#Zeitmessungen
time0=time.monotonic_ns()
time1=time.monotonic_ns()
time2=time.monotonic_ns()
time3=time.monotonic_ns()
time4=time.monotonic_ns()
time5=time.monotonic_ns()

#Variablen
button_state=False
tastdauer=0
selector=0
Pressed=False
n_messung=1


def lightshow():
    led1.value = True
    time.sleep(0.2)
    led2.value = True
    time.sleep(0.2)
    led3.value = True
    time.sleep(0.2)
    led1.value = False
    time.sleep(0.2)
    led2.value = False
    time.sleep(0.2)
    led3.value = False

def blink1():
    led1.value = True
    time.sleep(0.2)
    led1.value = False
    time.sleep(0.2)

def blink2():
    led2.value = True
    time.sleep(0.2)
    led2.value = False
    time.sleep(0.2)

def blink3():
    led3.value = True
    time.sleep(0.2)
    led3.value = False
    time.sleep(0.2)

#Initialisierung der SD Karte
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)
#use a digitalio pin 10 for breakout wiring:
cs = digitalio.DigitalInOut(board.D10)
sdcard = adafruit_sdcard.SDCard(spi, cs)
vfs = storage.VfsFat(sdcard)
storage.mount(vfs, "/sd")


with open("/sd/datalogging.csv", "a") as f:
    led.value = True  # turn on LED to indicate we're writing to the file
    f.write("date %s %d/%d/%d\n time %d:%02d:%02d \n " %(days[t.tm_wday], t.tm_mday, t.tm_mon, t.tm_year,t.tm_hour, t.tm_min, t.tm_sec))
    print("date %s %d/%d/%d\n time %d:%02d:%02d \n"  %(days[t.tm_wday], t.tm_mday, t.tm_mon, t.tm_year,t.tm_hour, t.tm_min, t.tm_sec))
    led.value = False  # turn off LED to indicate we're done
    # file is saved



while True:
    led2.value = True
    if button.value==True:
        time4=time.monotonic_ns()
        led1.value = True
        time0=time.monotonic_ns()
        while button.value==True:
            Pressed=True
            time5=time.monotonic_ns()
            time1=time.monotonic_ns()
            d1_time=time1-time0
            d2_time=time5-time4
            led2.value=False
            if d1_time>1000:
                selector=1
            if d1_time>1000000000:
                led2.value = True
                time.sleep(0.1)
                led2.value = False
                time0=time.monotonic_ns()
                time1=time.monotonic_ns()
                tastdauer=tastdauer+1
                print(tastdauer)
            if d2_time>3000000000:
                selector=2
                time0=time.monotonic_ns()
                time1=time.monotonic_ns()
                for i in range(3):
                    led2.value = True
                    time.sleep(0.05)
                    led2.value = False
                    time.sleep(0.05)

        print("taster")
        tastdauer=0
        led1.value = False
        time2=time.monotonic_ns()
        while time3-time2<1000000000:
            dataset=[(),(),(),(),()]
            time3=time.monotonic_ns()
            if selector==1:
                led1.value = True
                with open("/sd/datalogging.csv", "a") as f:
                    f.write("Messung %02d Start\r\n" % n_messung)
                while time3-time2<10000000000:
                    t = rtc.datetime
                    time3=time.monotonic_ns()
                    ser_bytes = str(uart.readline())
                    ser_bytes_a1=ser_bytes.split(',')
                    ser_bytes_a2=ser_bytes_a1[0].split('\'')
                    ser_bytes_a=ser_bytes_a2[1:]+ser_bytes_a1[1:]
                    try:
                        data="{},{},{},{},{}\n".format(ser_bytes_a1[0],ser_bytes_a1[1],ser_bytes_a1[2],ser_bytes_a1[3],ser_bytes_a1[4])
                        if math.fabs(float(ser_bytes_a[0])) > 80:
                            print("sensor error 1")
                            led3.value = True
                        if math.fabs(float(ser_bytes_a[1])) > 30:
                            print("sensor error 2")
                            led3.value = True
                        if math.fabs(float(ser_bytes_a[2])) > 30:
                            print("sensor error 3")
                            led3.value = True
                        with open("/sd/datalogging.csv", "a") as f:
                            led.value = True  # turn on LED to indicate we're writing to the file
                            f.write("time %d:%02d:%02d" % (t.tm_hour, t.tm_min, t.tm_sec))
                            print("time %d:%02d:%02d" %(t.tm_hour, t.tm_min, t.tm_sec))
                            f.write(data)
                            print(data)
                            led.value = False  # turn off LED to indicate we're done
                    except:
                        for i in range(5):
                            blink3()
                        led3.value = False
                    print(ser_bytes_a)
                led1.value = False
                n_messung=n_messung+1
                with open("/sd/datalogging.csv", "a") as f:
                    f.write("Messung %02d Ende\r\n" % n_messung)
                print("Messung", n_messung)
                lightshow()
                print(selector)
            if selector==2:
                led1.value = True
                led2.value = True
                led3.value = True
                time.sleep(1)
                led1.value = False
                led2.value = False
                led3.value = False
                time.sleep(2)
                print("Letzte Messungs Nummer", n_messung)
                n_messung_str= str(n_messung)
                if n_messung<10:
                    for i in range(int(n_messung_str[0])):
                        blink3()
                    time.sleep(0.5)
                elif n_messung<100:
                    for i in range(int(n_messung_str[0])):
                        blink2()
                    time.sleep(0.5)
                    for i in range(int(n_messung_str[1])):
                        blink3()
                    time.sleep(0.5)
                elif n_messung<1000:
                    for i in range(int(n_messung_str[0])):
                        blink1()
                    time.sleep(0.5)
                    for i in range(int(n_messung_str[1])):
                        blink2()
                    time.sleep(0.5)
                    for i in range(int(n_messung_str[2])):
                        blink3()
                    time.sleep(0.5)
                else:
                    led3.value = True
                    time.sleep(2)
                    led3.value = False
                    time.sleep(0.05)
                time.sleep(1)
                lightshow()

            selector=0


    time.sleep(0.001)
