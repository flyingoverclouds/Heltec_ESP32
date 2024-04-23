/*
 * HelTec Automation(TM) Electricity detection example.
 *
 * Function summary:
 *
 * - Vext connected to 3.3V via a MOS-FET, the gate pin connected to GPIO21;
 *
 * - Battery power detection is achieved by detecting the voltage of GPIO13;
 *
 * - OLED display and PE4259(RF switch) use Vext as power supply;
 *
 * - WIFI Kit series V1 don't have Vext control function;
 *
 * HelTec AutoMation, Chengdu, China.
 * �ɶ��������Զ����Ƽ����޹�˾
 * www.heltec.cn
 * support@heltec.cn
 *
 * this project also release in GitHub:
 * https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
 * 
*/
#include "Arduino.h"
#include <Wire.h>
#include "heltec.h"

#define BATTERY_ADCPIN 13
#define Fbattery    3700  //The default battery is 3700mv when the battery is fully charged.

float XS = 0.00225;      //The returned reading is multiplied by this XS to get the battery voltage.
uint16_t MUL = 1000;
uint16_t MMUL = 100;

double ReadVoltage(byte pin){
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if(reading < 1 || reading >= 4095)
    //return 0;
  // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as required


void setup()
{

  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, false /*Serial Enable*/);

	Heltec.display->init();
	//Heltec.display->flipScreenVertically();
	//Heltec.display->setFont(ArialMT_Plain_10);
	Heltec.display->drawString(0, 0, "OLED Start");
	Heltec.display->display();
	delay(1000);
	Heltec.display->clear();

	//adcAttachPin(BATTERY_ADCPIN);
	//analogSetClockDiv(255); // 1338mS

	//Serial.begin(115200);
}

void loop()
{
   uint16_t pinBatVal=analogRead(BATTERY_ADCPIN);
   uint16_t c  =  pinBatVal*XS*MUL;
   uint16_t d  =  (pinBatVal*XS*MUL*MMUL)/Fbattery;
   //Serial.println(analogRead(BATTERY_ADCPIN));
   //   Serial.println((String)d);
   //   Serial.printf("%x",analogRead(BATTERY_ADCPIN));
   Heltec.display->drawString(0, 0, "Remaining battery still has:");
   Heltec.display->drawString(0, 10, "VBAT: ");
   Heltec.display->drawString(35, 10, (String)c);
   Heltec.display->drawString(60, 10, "(mV)");

   Heltec.display->drawString(0, 20, "Capacity: ");
   Heltec.display->drawString(50, 20, (String)d);
   //Heltec.display->drawString(98, 20, ".");
   Heltec.display->drawString(65, 20, "%");

   double voltage=ReadVoltage(13);
   Heltec.display->drawString(0,35,"Accurant voltage:");
   Heltec.display->drawString(100,35,(String)voltage);

   Heltec.display->drawString(0,50,"PROUT");
   
   Heltec.display->display();
   delay(2000);
   Heltec.display->clear();
}
