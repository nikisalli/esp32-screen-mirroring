import mss
import time
from PIL import Image
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.bind(('0.0.0.0', 8090 ))
s.listen(0) 

width = 96
heigth = 64
offx = 400
offy = 400

bounding_box = {'top': offx, 'left': offy, 'width': width, 'height': heigth}

sct = mss.mss()

while True:
    client, addr = s.accept()
    
    while True:
        """sct_img = sct.grab(bounding_box)
        img = Image.new("RGB", sct_img.size)

        pixels = img.load()
        for x in range(sct_img.width):
            for y in range(sct_img.height):
                pixels[x, y] = sct_img.pixel(x, y)
    
        print("image taken")
        
        time.sleep(1)"""
        
        client.send(bytearray("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU", 'utf-8'))
        
        """for x in range(heigth):
            for y in range(width):
                s.write(pixels[y,x][0])
                s.write(pixels[y,x][1])
                s.write(pixels[y,x][2])"""
    
    print("Closing connection")
    client.close()

