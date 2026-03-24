#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "Adafruit_NeoPixel.h"

#include "ADCTouch.h"

// Deklaracija varijabli i konstanti

// double angle_rad = PI/180.0;
// double angle_deg = 180.0/PI;
// void SET(double Xpos, double Ypos, double COLORnr);
double LED_NR; // Broj LED diode
double pocetni; // Početna vrijednost
Adafruit_NeoPixel strip = Adafruit_NeoPixel(25, 3, NEO_GRB + NEO_KHZ800); // Inicijalizacija LED trake
double X; // X pozicija igrača
double Y; // Y pozicija igrača
double bodovi; // Broj bodova
double X1; // X pozicija nove jabuke
double Y1; // Y pozicija nove jabuke
double A1ref; // Referentna vrijednost za senzor A1
double A2ref; // Referentna vrijednost za senzor A2
double A3ref; // Referentna vrijednost za senzor A3
double A0ref; // Referentna vrijednost za senzor A0
double currentTime = 0; // Trenutno vrijeme
double lastTime = 0; // Zadnje vrijeme
double getLastTime(){
    return currentTime = millis()/1000.0 - lastTime;
}

// Definicije funkcija

// Funkcija za postavljanje LED diode na ekranu na određenu poziciju s određenom bojom
void SET(double Xpos, double Ypos, double COLORnr)
{
    if((((Xpos) > (-1)) && ((Ypos) > (-1 ))) && (((5) > (Xpos)) && ((5) > (Ypos)))){
        LED_NR = ((((1) - (fmod(Ypos,2))) * (Xpos)) + ((5) * (Ypos))) + ((fmod(Ypos,2)) * ((4) - (Xpos)));
        SET(LED_NR,COLORnr);
    }else{
        _delay(0.001);
    }
}

// Funkcija za postavljanje cijelog reda LED dioda na ekranu
void SET_LED_ROW(double row, double LED0, double LED1, double LED2, double LED3, double LED4)
{
    SET(0,row,LED0);
    SET(1,row,LED1);
    SET(2,row,LED2);
    SET(3,row,LED3);
    SET(4,row,LED4);
}

// Funkcija za postavljanje boje određene LED diode
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

// Inicijalizacija

void setup(){
    strip.begin();    LED_NR = 0;
    X = 2; // Početna X pozicija igrača
    Y = 2; // Početna Y pozicija igrača
    bodovi = 0; // Inicijalno postavljanje broja bodova na 0
    // Inicijalizacija ekrana
    for(int __i__=0;__i__<25;++__i__)
    {
        SET(LED_NR,5); // Postavljanje svih LED dioda na ekranu u boju 5 (crna)
        LED_NR += 1;
    }
    SET(2,2,2); // Postavljanje igrača na početnu poziciju
    X1 = random(0,(4)+1); // Generiranje nasumične X pozicije nove jabuke
    Y1 = random(0,(4)+1); // Generiranje nasumične Y pozicije nove jabuke
    SET(X1,Y1,1); // Postavljanje nove jabuke na ekran
    _delay(1); // Delay od 1 sekunde
    lastTime = millis()/1000.0; // Postavljanje trenutnog vremena
    // Postavljanje referentnih vrijednosti za senzore dodira
    A1ref = ADCTouch.read(15, 100);
    A2ref = ADCTouch.read(16, 100);
    A3ref = ADCTouch.read(17, 100);
    A0ref = ADCTouch.read(14, 100);
    // Postavljanje reda LED dioda na ekranu u određenu boju
    SET_LED_ROW(1,0,0,0,0,0);
    _delay(0.2); // Delay od 0.2 sekunde
    SET_LED_ROW(1,5,5,5,5,5); // Ponovno postavljanje reda LED dioda na ekranu u drugu boju
}

// Glavna petlja

void loop(){
    _delay(0.1); // Delay od 0.1 sekunde
    if((getLastTime()) < (30)){ // Ako je prošlo manje od 30 sekundi od početka igre
        // Provjera stanja senzora dodira i kretanje igrača
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
        // Provjera hvatanja jabuke
        if((((X)==(X1))) && (((Y)==(Y1)))){
            bodovi += 1; // Povećanje broja bodova
            X1 = random(0,(4)+1); // Generiranje nove nasumične X pozicije jabuke
            Y1 = random(0,(4)+1); // Generiranje nove nasumične Y pozicije jabuke
            SET(X1,Y1,1); // Postavljanje nove jabuke na ekran
        }
    }else{ // Ako je prošlo više od 30 sekundi od početka igre
        LED_NR = 0;
        // Postavljanje LED dioda na ekranu u određenu boju ovisno o broju osvojenih bodova
        for(int __i__=0;__i__<bodovi;++__i__)
        {
            SET(LED_NR,3);
            LED_NR += 1;
        }
    }
    _loop(); // Pozivanje pomoćne petlje
}

// Pomoćne funkcije

// Funkcija za delay u sekundama
void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

// Pomoćna petlja
void _loop(){
}
