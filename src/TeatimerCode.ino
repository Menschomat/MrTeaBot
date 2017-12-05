#include <Time.h>
#include <TimeLib.h>

//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/

//these pins can not be changed 2/3 are special pins
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h> //Die Servobibliothek wird aufgerufen. Sie wird benötigt, damit die Ansteuerung des Servos vereinfacht wird.
int encoderPin1 = 2;
int encoderPin2 = 3;
int encoderSwitchPin = 4; //push button switch
const int buzzer = 9;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
int seco;
int secostart;
long lastencoderValue = 0;
Servo servoblau;
int lastMSB = 0;
int lastLSB = 0;
#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error ("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int lastenco = 0;
int menustep;
#define blackteaT 270
#define greenteaT 180
#define fruitteaT 480
#define herbalteaT 420
#define whiteteaT 220
#define oolongteaT 180
void setup() {
        menustep = 0;
        pinMode(encoderPin1, INPUT);
        pinMode(encoderPin2, INPUT);

        pinMode(encoderSwitchPin, INPUT);

        digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
        digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

        digitalWrite(encoderSwitchPin, HIGH); //turn pullup resistor on

        //call updateEncoder() when any high/low changed seen
        //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
        attachInterrupt(0, updateEncoder, CHANGE);
        attachInterrupt(1, updateEncoder, CHANGE);
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

        // Clear the buffer.
        display.clearDisplay();
        display.display();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("Booting");
        display.display();
        setTime(0, 0, 0, 1, 1, 1970);
        // bla
        servoblau.attach(8);
//Servo Init
        servoblau.write(0);
        delay(1000);
        servoblau.write(180);
        delay(1000);
        servoblau.write(10);
        delay(1000);
        servoblau.detach();
        Serial.begin (9600);
        pinMode(buzzer, OUTPUT);
        tone(buzzer, 3500); // Send 1KHz sound signal...
        delay(160);  // ...for 1 sec
        noTone(buzzer);

        // Clear the buffer.
        display.clearDisplay();
        display.display();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("TEA-Tech");
        display.println("Alpha0.4");
        display.display();
}
//Methode zum Berechnen/Zählen der Position im Menü
void encoCount(int n) {
        if (abs(lastenco - n) >= 4 ) {
                if (lastenco > n && menustep > 1) {
                        --menustep;
                } else if (lastenco < n && menustep < 4) {
                        ++menustep;
                }
                lastenco = n;
        }
}
//UP-Animation
void drawUp() {
        int x;
        x = 0;
        display.fillRect(63, 0, 3, 64, WHITE);
        display.fillRect(72, 24, 7, 40, WHITE);

        while (x != 8) {
                display.drawLine(72 + x, 24, 100 + x, 5, WHITE);
                ++x;
        }
        display.setCursor(83, 30);
        display.setTextSize(1);
        display.println("ArmPos:");
        display.setCursor(83, 40);
        display.println("UP");

}
// Inhalt der Menüs mit allen Punkten
void build_menu() {


        switch (menustep) {
        case 1:
                display.clearDisplay();
                display.setTextSize(2);
                display.setTextColor(BLACK, WHITE);
                display.setCursor(0, 0);
                display.println("Black");
                display.setTextColor(WHITE, BLACK);
                display.println("Green");
                display.setTextColor(WHITE, BLACK);
                display.println("Fruit");
                display.setTextColor(WHITE, BLACK);
                display.println("Herbs");
                drawUp();
                display.display();
                break;

        case 2:
                display.clearDisplay();
                display.setTextSize(2);
                display.setTextColor(WHITE, BLACK);
                display.setCursor(0, 0);
                display.println("Black");
                display.setTextColor(BLACK, WHITE);
                display.println("Green");
                display.setTextColor(WHITE, BLACK);
                display.println("Fruit");
                display.setTextColor(WHITE, BLACK);
                display.println("Herbs");
                drawUp();
                display.display();
                break;
        case 3:
                display.clearDisplay();
                display.setTextSize(2);
                display.setTextColor(WHITE, BLACK);
                display.setCursor(0, 0);
                display.println("Black");
                display.setTextColor(WHITE, BLACK);
                display.println("Green");
                display.setTextColor(BLACK, WHITE);
                display.println("Fruit");
                display.setTextColor(WHITE, BLACK);
                display.println("Herbs");
                drawUp();
                display.display();
                break;
        case 4:
                display.clearDisplay();
                display.setTextSize(2);
                display.setTextColor(WHITE, BLACK);
                display.setCursor(0, 0);
                display.println("Black");
                display.setTextColor(WHITE, BLACK);
                display.println("Green");
                display.setTextColor(WHITE, BLACK);
                display.println("Fruit");
                display.setTextColor(BLACK, WHITE);
                display.println("Herbs");
                drawUp();
                display.display();
                break;
        }
}
// Graphische Darstellung des Timers
void timerdisplay(int t) {
        seco = now();
        secostart = now();
        while (seco < secostart + t)
        { seco = now();
          display.clearDisplay();
          display.setTextSize(3);
          display.setTextColor(WHITE, BLACK);
          display.setCursor(0, 0);
          display.print("Sec:");
          display.println(abs(secostart + t - seco));
          display.display();}

}
void loop() {
        //Do stuff here
        encoCount(encoderValue);
        build_menu();
        Serial.println(menustep);
        if (digitalRead(encoderSwitchPin)) {

        } else {
                if(menustep !=0) {
                        tone(buzzer, 4000); // Send 1KHz sound signal...
                        delay(80); // ...for 1 sec
                        noTone(buzzer);
                        servoblau.attach(8);
                        servoblau.write(180);
                }
                switch (menustep) {

                case 0:
                        menustep = 1;
                        break;
                case 1:
                        timerdisplay(blackteaT);
                        servoblau.write(0);
                        break;
                case 2:
                        timerdisplay(greenteaT);
                        servoblau.write(0);
                        break;
                case 3:
                        timerdisplay(fruitteaT);
                        servoblau.write(0);
                        break;
                case 4:
                        timerdisplay(herbalteaT);
                        servoblau.write(0);
                        break;
                }
                if(menustep !=0) {
                        delay(1000);
                        servoblau.detach();
                }
        }



        Serial.println(encoderValue);
        //just here to slow down the output, and show it will work even during a delay
}

void updateEncoder() {
        int MSB = digitalRead(encoderPin1); //MSB = most significant bit
        int LSB = digitalRead(encoderPin2); //LSB = least significant bit

        int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
        int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
        if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++; if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--; lastEncoded = encoded; //store this value for next time
}
