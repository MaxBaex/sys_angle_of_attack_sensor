
import serial as ser
import time
import csv
import matplotlib.pyplot as plt
import numpy as np
import serial


counter=0
plot_window = 1000
x=np.linspace(counter,counter+plot_window,num=plot_window)

y1_var = np.array(np.zeros([plot_window]))
y2_var = np.array(np.zeros([plot_window]))
y3_var = np.array(np.zeros([plot_window]))
y4_var = np.array(np.zeros([plot_window]))
y5_var = np.array(np.zeros([plot_window]))

plt.ion()
fig, ax = plt.subplots(2,1)
#line, = ax.plot(y1_var)

data=np.array([[""],[""],[""],[""],[""]]).reshape((1,5))

ser = ser.Serial('COM6',115200, timeout=0.1)
ser.flushInput()

while True:
    try:
        x=np.linspace(counter,counter+plot_window,num=plot_window)
        correction=np.array([-0.015,0.021,-0.01,0,0.2])
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes.decode("utf-8")
        decoded_bytes=decoded_bytes.strip()
        decoded_bytes=decoded_bytes.replace('<', '')
        decoded_bytes=decoded_bytes.replace('>', '')
        decoded_bytes=[float(i) for i in decoded_bytes.split(',')]
        decoded_bytes=np.array(decoded_bytes).reshape((1,5))
        decoded_bytes=decoded_bytes+correction
        data=np.append(data,decoded_bytes,axis=0)
        
        """with open("test_data.csv","a",newline='') as f:
            writer = csv.writer(f,delimiter=",")
            writer.writerow(decoded_bytes)"""
        x_var=x
        y1_var = np.append(y1_var,decoded_bytes[0,0])
        y1_var = y1_var[1:plot_window+1]
        y2_var = np.append(y2_var,decoded_bytes[0,1])
        y2_var = y2_var[1:plot_window+1]
        y3_var = np.append(y3_var,decoded_bytes[0,2])
        y3_var = y3_var[1:plot_window+1]
        y4_var = np.append(y4_var,decoded_bytes[0,3])
        y4_var = y4_var[1:plot_window+1]
        y5_var = np.append(y5_var,decoded_bytes[0,4])
        y5_var = y5_var[1:plot_window+1]
        
        
        ax[0].cla()     
        ax[0].plot(x_var,y1_var)     
        ax[0].plot(x_var,y2_var)
        ax[0].plot(x_var,y3_var)
        ax[0].relim()
        ax[0].autoscale_view()
        #ax[0].set_xlabel('Messpunkt')
        ax[0].set_ylabel('Druck [mbar]')
        ax[0].grid(True) 
        ax[0].set_title("Live-Sensordaten")

        
        ax[1].cla()   ;
        ax[1].plot(x_var,y4_var);
        ax[1].plot(x_var,y5_var);
        ax[1].relim();
        ax[1].autoscale_view();
        ax[1].set_ylim(-10,40);
        ax[1].set_xlabel('Messpunkt');
        ax[1].set_ylabel('Temperatur [°C]');
        ax[1].grid(True) ;

        l1, = ax[0].plot(x, y1_var);
        l2, = ax[0].plot(x, y2_var);
        l3, = ax[0].plot(x, y3_var);
        l4, = ax[1].plot(x, y4_var);
        l5, = ax[1].plot(x, y5_var);

        ax[0].legend((l1, l2,l3), ('Totaldruck', 'Differenzdruck 1', 'Differenzdruck 2'),loc='lower left');
        ax[1].legend((l4, l5), ('Temperatur 1', 'Temperatur 2'),loc='lower left');            
        
        fig.canvas.draw();
        fig.canvas.flush_events();
        counter+=1;
    except:
        print("Keyboard Interrupt")
        ser.close()
        break