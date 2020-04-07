from mss import mss
import time
from PIL import Image
import serial
import io
import sys

width = 3240  # monitor width
heigth = 2160 # monitor height
offx = 0      # window horizontal offset
offy = 0      # window vertical offset

s = serial.Serial('/dev/ttyUSB0', 921600) #insert your serial port here

bounding_box = {'top': offx, 'left': offy, 'width': width, 'height': heigth}

sct = mss()

while True:
    sct_img = sct.grab(bounding_box)

    img = Image.frombytes("RGB", sct_img.size, sct_img.bgra, "raw", "BGRX")
    img.thumbnail([96,64])
    
    b = io.BytesIO()
    img.save(b, format='JPEG')
    jpg = b.getvalue()
    size = len(jpg)
    
    for _ in range(10):
        s.write(b'U')
        
    s.write(((size >> 8) & 0xff).to_bytes(1, 'big'))
    s.write((size & 0xff).to_bytes(1, 'big'))
    s.write(jpg)
    
    time.sleep(0.04)
    
    string = b''
    while(s.in_waiting > 0):
        string += s.read()
    print(string)
    