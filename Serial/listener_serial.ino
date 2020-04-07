#include "SPI.h"
#include "SSD_13XX.h"
#include <JPEGDecoder.h>

#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

SSD_13XX tft = SSD_13XX(5, 16, 17);

void setup(){
	Serial.begin(921600);
	Serial.setRxBufferSize(3000);
	tft.begin();
}
 
void loop(){
	byte iters = 0;
	while(iters < 10){
		while(Serial.available() < 1){}
		if(Serial.read() == 0x55){
			iters++;
		} else {
			iters = 0;
		}
	}

	byte hb = Serial.read();
	byte lb = Serial.read();
	int size = (hb << 8) | lb;

	if(size > 5000){
		return;
	}

	Serial.print(size);

	uint8_t buf[3000] = {};

	for(int i=0;i<size;i++){
		while(Serial.available() < 1){}
		buf[i] = Serial.read();
	}

	drawArrayJpeg(buf, size, 0, 0);
}

void drawArrayJpeg(const uint8_t arrayname[], uint32_t array_size, int xpos, int ypos) {
  boolean decoded = JpegDec.decodeArray(arrayname, array_size);
  
  if (decoded) {
	Serial.print("decode succesful");
	jpegRender(xpos, ypos);
  }
}

void jpegRender(int xpos, int ypos) {
	uint16_t  *pImg;
	uint16_t mcu_w = JpegDec.MCUWidth;
	uint16_t mcu_h = JpegDec.MCUHeight;
	uint32_t max_x = JpegDec.width;
	uint32_t max_y = JpegDec.height;

  	uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  	uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  	uint32_t win_w = mcu_w;
  	uint32_t win_h = mcu_h;

  	max_x += xpos;
  	max_y += ypos;

  	while ( JpegDec.read()) {
		pImg = JpegDec.pImage;

		int mcu_x = JpegDec.MCUx * mcu_w + xpos;
		int mcu_y = JpegDec.MCUy * mcu_h + ypos;

		if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
		else win_w = min_w;
		if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
		else win_h = min_h;

		uint16_t mcu_pixels = win_w * win_h;

		if ( ( mcu_x + win_w) <= 96 && ( mcu_y + win_h) <= 64){
	  		tft.startPushData(mcu_x, mcu_y, mcu_x + win_w - 1, mcu_y + win_h - 1);

	  		while (mcu_pixels--) tft.pushData(*pImg++);
		}
		else if ( ( mcu_y + win_h) >= 64) JpegDec.abort();
		tft.endPushData();
  	}
}