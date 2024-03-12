#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "Adafruit_NeoPixel.h"

#include "ADCTouch.h"

// double angle_rad = PI/180.0;
// double angle_deg = 180.0/PI;
// void SET(double Xpos, double Ypos, double COLORnr);
double LED_NR;
// // void SET_LED_ROW(double row, double LED0, double LED1, double LED2, double LED3, double LED4);
// // void SET(double LEDnr, double COLORnr);
double pocetni;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(25, 3, NEO_GRB + NEO_KHZ800);
double X;
double Y;
double bodovi;
double X1;
double Y1;
double A1ref;
double A2ref;
double A3ref;
double A0ref;
double currentTime = 0;
double lastTime = 0;
double getLastTime(){
    	return currentTime = millis()/1000.0 - lastTime;
}

void SET(double Xpos, double Ypos, double COLORnr)
{
    if((((Xpos) > (-1)) && ((Ypos) > (-1 ))) && (((5) > (Xpos)) && ((5) > (Ypos)))){
        LED_NR = ((((1) - (fmod(Ypos,2))) * (Xpos)) + ((5) * (Ypos))) + ((fmod(Ypos,2)) * ((4) - (Xpos)));
        SET(LED_NR,COLORnr);
    }else{
        _delay(0.001);
    }
}

void SET_LED_ROW(double row, double LED0, double LED1, double LED2, double LED3, double LED4)
{
    SET(0,row,LED0);
    SET(1,row,LED1);
    SET(2,row,LED2);
    SET(3,row,LED3);
    SET(4,row,LED4);
}

void SET(double LEDnr, double COLORnr)
{
    if(((2)==(3))){
        pocetni = ADCTouch.read(17, 100);
    }
    if(((COLORnr)==(0))){
        strip.setPixelColor(LEDnr, 0, 0, 0);
        strip.show();
    }else{
        if(((COLORnr)==(2))){
            strip.setPixelColor(LEDnr, 100, 0, 0);
            strip.show();
        }else{
            if(((COLORnr)==(1))){
                strip.setPixelColor(LEDnr, 0, 100, 0);
                strip.show();
            }else{
                if(((COLORnr)==(3))){
                    strip.setPixelColor(LEDnr, 0, 0, 100);
                    strip.show();
                }else{
                    if(((COLORnr)==(4))){
                        strip.setPixelColor(LEDnr, 100, 100, 0);
                        strip.show();
                    }else{
                        if(((COLORnr)==(5))){
                            strip.setPixelColor(LEDnr, 0, 100, 100);
                            strip.show();
                        }else{
                            strip.setPixelColor(LEDnr, 100, 0, 100);
                            strip.show();
                        }
                    }
                }
            }
        }
    }
    _delay(0.01);
}

void setup(){
    strip.begin();    LED_NR = 0;
    X = 2;
    Y = 2;
    bodovi = 0;
    for(int __i__=0;__i__<25;++__i__)
    {
        SET(LED_NR,5);
        LED_NR += 1;
    }
    SET(2,2,2);
    X1 = random(0,(4)+1);
    Y1 = random(0,(4)+1);
    SET(X1,Y1,1);
    _delay(1);
    lastTime = millis()/1000.0;
    A1ref = ADCTouch.read(15, 100);
    A2ref = ADCTouch.read(16, 100);
    A3ref = ADCTouch.read(17, 100);
    A0ref = ADCTouch.read(14, 100);
    SET_LED_ROW(1,0,0,0,0,0);
    _delay(0.2);
    SET_LED_ROW(1,5,5,5,5,5);
}

void loop(){
    _delay(0.1);
    if((getLastTime()) < (30)){
        if(((ADCTouch.read(15, 100)) - (A1ref)) > (10)){
            if((X) < (4)){
                SET(X,Y,5);
                X += 1;
                SET(X,Y,2);
            }
        }
        if(((ADCTouch.read(16, 100)) - (A2ref)) > (10)){
            if((Y) < (4)){
                SET(X,Y,5);
                Y += 1;
                SET(X,Y,2);
            }
        }
        if(((ADCTouch.read(14, 100)) - (A0ref)) > (10)){
            if((0) < (X)){
                SET(X,Y,5);
                X += -1;
                SET(X,Y,2);
            }
        }
        if(((ADCTouch.read(17, 100)) - (A3ref)) > (10)){
            if((0) < (Y)){
                SET(X,Y,5);
                Y += -1;
                SET(X,Y,2);
            }
        }
        if((((X)==(X1))) && (((Y)==(Y1)))){
            bodovi += 1;
            X1 = random(0,(4)+1);
            Y1 = random(0,(4)+1);
            SET(X1,Y1,1);
        }
    }else{
        LED_NR = 0;
        for(int __i__=0;__i__<bodovi;++__i__)
        {
            SET(LED_NR,3);
            LED_NR += 1;
        }
    }
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
}