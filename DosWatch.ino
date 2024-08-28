#include "src/Watchy.h"
#include <Fonts/FreeMonoOblique24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include "settings.h"

//Watchy watchy(settings);

bool led_phase;

//#define watch

class DosWatchFace : public Watchy{ //inherit and extend Watchy class
    private: 
      bool led_phase;
      DosData lastDos;

    public:
        DosWatchFace(const watchySettings& s) : Watchy(s) {}
        void drawWatchFace(){ 
          display.fillScreen(GxEPD_WHITE);
          display.setTextColor(GxEPD_BLACK);

          DosData currentDos = getDosData(settings.weatherUpdateInterval);

          float VBAT = getBatteryVoltage();
          float batteryVoltage = getBatteryVoltage();
          float voltageMin = 3;
          float voltageMax = 3.9;
          float voltageRange = voltageMax - voltageMin;
          float batteryPercentage = round(((VBAT - voltageMin) / voltageRange) * 100 / 5) * 5;
          if (batteryPercentage > 100) batteryPercentage = 100;
          if (batteryPercentage < 0) batteryPercentage = 0;

          bool charging = digitalRead(USB_DET_PIN);


          display.setFont(&FreeSans9pt7b);
          
          display.setCursor(0, 199);
          display.print("SPACEDOS monitor");
          display.drawLine(0, 184, 200, 184, GxEPD_BLACK);

          display.setCursor(0, 13);
          
          char batteryVoltageString[10];

          snprintf(batteryVoltageString, sizeof(batteryVoltageString), "BAT: %.2f", batteryVoltage);
          display.print(batteryVoltageString);
          display.print("V , ");
          display.print(int(batteryPercentage));
          display.print(charging ? "+%" : "%");
          display.drawLine(0, 16, 200, 16, GxEPD_BLACK);


          if(currentDos.updated){
            if(currentDos.alert && lastDos.alert != currentDos.alert){
              vibMotor(75, 4);
            }

            display.setCursor(5, 105);
            
            if (currentDos.alert){
              
              display.fillRect(0, 64, 200, 90, GxEPD_BLACK);
              display.setTextColor(GxEPD_WHITE);

              //display.print("ALERT");
            } else {

              display.drawRect(0, 64, 200, 90, GxEPD_BLACK);
              //display.print("OK");
            }

            //char chCTS[25];
            //snprintf(chCTS, sizeof(chCTS), "%d %d", currentDos.cps, currentDos.cps_rate ? "UP" : "DN");

            display.setCursor(5, 120);
            display.setFont(&FreeMonoOblique24pt7b);
            #ifdef watch
              display.print(currentDos.cps);            
              display.setFont(&FreeSans9pt7b);
              display.print(" CPS");
            #else

              char DString[20];
              snprintf(DString, sizeof(DString), "%.2f", (currentDos.total_dose/10000.0) );
              display.print(DString);
              display.setFont(&FreeSans9pt7b);
              display.print("mSv");
              display.setCursor(5, 83);
              display.print("Dose:");

            #endif

            display.setCursor(5, 145);
            display.print("state:");

            display.print(currentDos.alert? "ALERT" : "OK");

            display.setTextColor(GxEPD_BLACK);

            display.setCursor(5, 175);
            char DoseString[20];
            #ifdef watch
              snprintf(DoseString, sizeof(DoseString), "Tot. dos: %.3f mSv", (currentDos.total_dose/10000.0) );
              display.print(DoseString);
            #else
              display.print("Current: ");
              display.print(currentDos.cps);
              display.print(" CPS");
            #endif
            

          } else {
              display.setFont(&FreeMonoOblique24pt7b);
              display.setTextColor(GxEPD_BLACK);
              display.setCursor(2, 120);
              display.fillRect(0, 64, 200, 90, GxEPD_WHITE);
              display.drawRect(0, 64, 200, 90, GxEPD_BLACK);
              display.print("No DATA");
              display.drawLine(0, 64, 200, 151, GxEPD_BLACK);
              display.drawLine(0, 151, 200, 64, GxEPD_BLACK);
          }


          if(led_phase == true){
            display.fillRect(189, 0, 199, 10, GxEPD_BLACK);
            led_phase = false;
          } else {
            display.fillRect(189, 0, 199, 10, GxEPD_WHITE);
            led_phase = true;
          }
          
          //display.fillRect(190, 0, 200, 10, led_phase? GxEPD_BLACK : GxEPD_WHITE);
          //display.drawRect(190, 0, 200, 10, GxEPD_BLACK);
          //led_phase = !led_phase;
          

          display.setTextColor(GxEPD_BLACK);
          display.setFont(&FreeMonoOblique24pt7b);
          display.setCursor(25, 53);
          if(currentTime.Hour < 10){ //use the currentTime struct to print latest time
            display.print("0");
          }
          display.print(currentTime.Hour);
          display.print(":");
          if(currentTime.Minute < 10){
            display.print("0");
          }  
          display.print(currentTime.Minute);   
          display.setFont(&FreeSans9pt7b);
          display.println("UT");


          lastDos = currentDos;
        }
};

DosWatchFace m(settings); //instantiate your watchface


void setup(){
  m.init();
}

void loop(){}