/*
 Name:		PC_STATS_DISPLAY_SSD1306_OLED.ino
 Created:	1/27/2025 5:28:05 PM
 Author:	fustok
*/
  
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int cpuTemp = 0;
int cpuUsage = 0;
int gpuTemp = 0;
int gpuUsage = 0;
String staticData = "";

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200); //115200 112500 9600
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(2000);
    display.clearDisplay();

    //display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("Starting..."));
    display.display();
    delay(2000);
    display.clearDisplay();
}

// the loop function runs over and over again until power down or reset
void loop() {
    if (Serial.available() > 0)
    {
        staticData = Serial.readString();
        cpuTemp = staticData.substring(1, 4).toInt();
        cpuUsage = staticData.substring(5, 8).toInt();
        gpuTemp = staticData.substring(9, 12).toInt();
        gpuUsage = staticData.substring(13, 16).toInt();
        if (cpuTemp >= 0 && cpuUsage >= 0 && gpuTemp >= 0 && gpuUsage >= 0)
        {
            //displayStatic();
            display.clearDisplay();
            display.setTextColor(SSD1306_WHITE);
            display.setTextSize(1);
            display.setFont(NULL);
            display.setCursor(0, 0);

            //display.println(F("<<display connected>>"));
            //display.println(" ");

            display.println("IP: 192.168.1.23/WiFi"); //1
            display.println(" ");                     //2

            display.println("SSH: is On");            //3

            display.print("CPU: ");                   //4
            display.print(cpuTemp);
            display.print(".");
            display.print("C - ");
            display.print(cpuUsage);
            display.println("%");

            display.print("GPU: ");                   //5       
            display.print(gpuTemp);
            display.print(".");
            display.print("C - ");
            display.print(gpuUsage);
            display.println("%");

            display.println("UpTime: 10d18h43m");     //6
            display.print("Memory: 8Gb - 12%");       //7

            //display.println("8"); not printed, only 7 printed


            display.display();
            delay(2000);
            display.clearDisplay();
        }
        else
        {
            //displayError(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.println(F("display error..."));
            display.display();
            delay(2000);

        }
    }
    else
    {
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println(F("no serial data..."));
        display.display();
        delay(2000);

    }
    delay(1000);
    display.clearDisplay();
}

void displayError(int code) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Error TwT"));
    display.setCursor(0, 1);
    switch (code) {
    case 1: {
        display.println("No Data");
    }
    }
}

void displayStatic() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("CPU:"));
    display.setCursor(8 - checkNumUnit(cpuTemp), 0);          //0123456789012345
    display.println(cpuTemp);                                //CPU: 100.C 100%
    display.println(F("\xDF"));
    display.println(F("C "));
    display.setCursor(14 - checkNumUnit(cpuUsage), 0);
    display.println(cpuUsage);
    display.println(F("%"));
    display.setCursor(0, 1);
    display.println(F("GPU:"));
    display.setCursor(8 - checkNumUnit(gpuTemp), 1);          //0123456789012345
    display.println(gpuTemp);                                //GPU: 100.C 100%
    display.println(F("\xDF"));
    display.println(F("C "));
    display.setCursor(14 - checkNumUnit(gpuUsage), 1);
    display.println(gpuUsage);
    display.println(F("%"));
}

int checkNumUnit(int num) {
    int temp = num;
    int numUnit = 1;
    while (temp >= 10) {
        temp /= 10;
        numUnit++;
    }
    return numUnit;
}
