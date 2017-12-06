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
#define Version "Beta 0.5"
static const unsigned char PROGMEM CupBMP[] =
{// Tasse mit Dampf
        0x00, 0x40, 0x80, 0x00, 0x00, 0xe1, 0xc0, 0x00, 0x00, 0xe1, 0xc0, 0x00, 0x01, 0xc1, 0x80, 0x00,
        0x00, 0xc1, 0x80, 0x00, 0x00, 0xe1, 0xc0, 0x00, 0x00, 0xe0, 0xc0, 0x00, 0x00, 0x70, 0xe0, 0x00,
        0x00, 0x70, 0xe0, 0x00, 0x00, 0xf1, 0xe0, 0x00, 0x00, 0xe1, 0xc0, 0x00, 0x00, 0x40, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xdc,
        0x7f, 0xff, 0xff, 0xce, 0x7f, 0xff, 0xff, 0xc6, 0x7f, 0xff, 0xff, 0xc6, 0x3f, 0xff, 0xff, 0x86,
        0x3f, 0xff, 0xff, 0x8e, 0x1f, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xfe, 0xe0,
        0x07, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xf0,
};
static const unsigned char PROGMEM CupCoolBMP[] =
{// Tasse ohne Dampf
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0xff, 0xdc,
        0x7f, 0xff, 0xff, 0xce, 0x7f, 0xff, 0xff, 0xc6, 0x7f, 0xff, 0xff, 0xc6, 0x3f, 0xff, 0xff, 0x86,
        0x3f, 0xff, 0xff, 0x8e, 0x1f, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xfe, 0xe0,
        0x07, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x3f, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xf0,
};
static const unsigned char PROGMEM Loading0BMP[] =
{// Ladekreis 0 Grad
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
        0x04, 0x01, 0x80, 0x00, 0x0e, 0x01, 0x80, 0x00, 0x07, 0x01, 0x80, 0x00, 0x03, 0x81, 0x80, 0x00,
        0x01, 0xc1, 0x80, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x01, 0xff,
        0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x20, 0x04, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x01, 0xc1, 0x83, 0x80,
        0x03, 0x81, 0x81, 0xc0, 0x07, 0x01, 0x80, 0xe0, 0x0e, 0x01, 0x80, 0x70, 0x04, 0x01, 0x80, 0x20,
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
};
static const unsigned char PROGMEM Loading90BMP[] =
{// Ladekreis 90 Grad
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
        0x04, 0x01, 0x80, 0x20, 0x0e, 0x01, 0x80, 0x70, 0x07, 0x01, 0x80, 0xe0, 0x03, 0x81, 0x81, 0xc0,
        0x01, 0xc1, 0x83, 0x80, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x20, 0x04, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x01, 0xff,
        0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x20, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x01, 0xc1, 0x80, 0x00,
        0x03, 0x81, 0x80, 0x00, 0x07, 0x01, 0x80, 0x00, 0x0e, 0x01, 0x80, 0x00, 0x04, 0x01, 0x80, 0x00,
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
};
static const unsigned char PROGMEM Loading180BMP[] =
{// Ladekreis 180 Grad
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
        0x04, 0x01, 0x80, 0x20, 0x0e, 0x01, 0x80, 0x70, 0x07, 0x01, 0x80, 0xe0, 0x03, 0x81, 0x81, 0xc0,
        0x01, 0xc1, 0x83, 0x80, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x20, 0x04, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x01, 0xff,
        0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x01, 0x83, 0x80,
        0x00, 0x01, 0x81, 0xc0, 0x00, 0x01, 0x80, 0xe0, 0x00, 0x01, 0x80, 0x70, 0x00, 0x01, 0x80, 0x20,
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
};
static const unsigned char PROGMEM Loading270BMP[] =
{// Ladekreis 270 Grad
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
        0x00, 0x01, 0x80, 0x20, 0x00, 0x01, 0x80, 0x70, 0x00, 0x01, 0x80, 0xe0, 0x00, 0x01, 0x81, 0xc0,
        0x00, 0x01, 0x83, 0x80, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x04, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x01, 0xff,
        0xff, 0x80, 0x01, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x20, 0x04, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x01, 0xc1, 0x83, 0x80,
        0x03, 0x81, 0x81, 0xc0, 0x07, 0x01, 0x80, 0xe0, 0x0e, 0x01, 0x80, 0x70, 0x04, 0x01, 0x80, 0x20,
        0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00,
};
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
        display.println("TEA-Tech");
        display.println(Version);
        display.println("Booting...");
        display.display();
        setTime(0, 0, 0, 1, 1, 1970);
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
        Beep(3500,160);
        // Clear the buffer.
        display.clearDisplay();
        display.display();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println("TEA-Tech");
        display.println(Version);
        display.display();
}
//Methode zum Berechnen/Zählen der Position im Menü
void Beep(int khz,int dur){
        tone(buzzer, khz); // Send 1KHz sound signal...
        delay(dur); // ...for 1 sec
        noTone(buzzer);
}
void encoCount(int n) {
        if (abs(lastenco - n) >= 4 ) {
                Beep(1000,0.6);
                if (lastenco > n && menustep > 1) {
                        --menustep;
                } else if (lastenco < n && menustep < 4) {
                        ++menustep;
                }
                lastenco = n;
        }
}
//UP-Animation für das Menü
void drawUp() {
        switch (menustep) {
        case 1:
                display.fillRect(63, 0, 2, 64, WHITE);
                display.setCursor(67, 0);
                display.setTextSize(1);
                display.setTextColor(WHITE, BLACK);
                display.println("Infos:");
                display.setCursor(67, 12);
                display.println("4 Spoons");
                display.setCursor(67, 22);
                display.println("per Liter!");
                display.setCursor(67, 32);
                display.println("95C Water");
                display.setCursor(67, 42);
                display.println("Temp!");
                display.setCursor(67, 52);
                display.println("270 Sec.");
                break;
        case 2:
                display.fillRect(63, 0, 2, 64, WHITE);
                display.setCursor(67, 0);
                display.setTextSize(1);
                display.setTextColor(WHITE, BLACK);
                display.println("Infos:");
                display.setCursor(67, 12);
                display.println("3 Spoons");
                display.setCursor(67, 22);
                display.println("per Liter!");
                display.setCursor(67, 32);
                display.println("80C Water");
                display.setCursor(67, 42);
                display.println("Temp!");
                display.setCursor(67, 52);
                display.println("180 Sec.");
                break;
        case 3:
                display.fillRect(63, 0, 2, 64, WHITE);
                display.setCursor(67, 0);
                display.setTextSize(1);
                display.setTextColor(WHITE, BLACK);
                display.println("Infos:");
                display.setCursor(67, 12);
                display.println("5 Spoons");
                display.setCursor(67, 22);
                display.println("per Liter!");
                display.setCursor(67, 32);
                display.println("95C Water");
                display.setCursor(67, 42);
                display.println("Temp!");
                display.setCursor(67, 52);
                display.println("480 Sec.");
                break;
        case 4:
                display.fillRect(63, 0, 2, 64, WHITE);
                display.setCursor(67, 0);
                display.setTextSize(1);
                display.setTextColor(WHITE, BLACK);
                display.println("Infos:");
                display.setCursor(67, 12);
                display.println("4 Spoons");
                display.setCursor(67, 22);
                display.println("per Liter!");
                display.setCursor(67, 32);
                display.println("100C Water");
                display.setCursor(67, 42);
                display.println("Temp!");
                display.setCursor(67, 52);
                display.println("420 Sec.");
                break;
        }
}
//Down-Animation für das Menü
void drawDown() {
        //Hier noch eintragen
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
        int count = 1;
        int secolast = now();
        while (seco < secostart + t) {
                seco = now();
                display.clearDisplay();
                display.setTextSize(3);
                display.setTextColor(WHITE, BLACK);
                display.setCursor(0, 0);
                display.print("Sec:");
                display.println(abs(secostart + t - seco));

                switch (count) {
                case 1:
                        display.drawBitmap(10, 32, CupCoolBMP, 32, 32, 1);
                        display.drawBitmap(86, 32, Loading0BMP, 32, 32, 1);
                        break;
                case 2:
                        display.drawBitmap(10, 32, CupBMP, 32, 32, 1);
                        display.drawBitmap(86, 32, Loading90BMP, 32, 32, 1);
                        break;
                case 3:
                        display.drawBitmap(10, 32, CupCoolBMP, 32, 32, 1);
                        display.drawBitmap(86, 32, Loading180BMP, 32, 32, 1);
                        break;
                case 4:
                        display.drawBitmap(10, 32, CupBMP, 32, 32, 1);
                        display.drawBitmap(86, 32, Loading270BMP, 32, 32, 1);
                        
                        break;
                }
                if (secolast!=now()) {
                        if(count !=4) {
                                ++count;
                        }else{
                                count=1;
                        }
                        secolast=now();
                }
                display.display();
        }
}
void loop() {
        //Do stuff here
        encoCount(encoderValue);
        build_menu();
        Serial.println(menustep);
        if (digitalRead(encoderSwitchPin)) {
        } else {
                if(menustep !=0) {
                        tone(buzzer, 4000); // Sendet 4KHz Ton
                        delay(80); //für 80ms
                        noTone(buzzer); //Ton deaktivieren
                        servoblau.attach(8);
                        servoblau.write(180);
                }
                switch (menustep) {
                //Auswahl der Teesorte und Timerstart
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
}

void updateEncoder() {
        int MSB = digitalRead(encoderPin1); //MSB = most significant bit
        int LSB = digitalRead(encoderPin2); //LSB = least significant bit
        int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
        int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
        if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
        if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--;
        lastEncoded = encoded; //store this value for next time
}
