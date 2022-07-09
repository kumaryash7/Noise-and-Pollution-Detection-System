#include <MQ131.h> //Ozone sensor library

#define DUST_SENSOR_PIN_PM10  3  //Must be the pins that
#define DUST_SENSOR_PIN_PM25  4  //support interrupts

#define INTERVAL_COUNTDOWN 1000
#define INTERVAL_READ 30000

#include <KarserDSM501.h>       // ISRs forward declaration
void pm10_handleInterrupt();
void pm25_handleInterrupt();    // init pm10 and pm25 instances

KarserDSM501 pm10(DUST_SENSOR_PIN_PM10, pm10_handleInterrupt);
KarserDSM501 pm25(DUST_SENSOR_PIN_PM25, pm25_handleInterrupt); // handle ISRs
void pm10_handleInterrupt() { pm10.handleInterrupt(); }
void pm25_handleInterrupt() { pm25.handleInterrupt(); }


unsigned long timer = 0;

const int AOUTpin=A4;    // CO sensor AOUT pin
const int AOUTpin1=A5;   // Sound sensor AOUT pin

int conc_O3;     // ozone concentration
int conc_CO;     // carbon monoxide concentration
int pm2_5;        // (particulate matter)pm2.5
int level_dB;    // sound level in decibal

// defining lower and upper limit for each pollutant
int Lo3,Uo3,Lpm,Upm,Lco,Uco,AQI_L,AQI_U; 
 
String quality;

void setup() {
  Serial.begin(115200);

  pinMode(AOUTpin, INPUT);// set Aoutpin of CO sensor as input
  pinMode(AOUTpin1, INPUT);// set Aoutpin of Sound sensor as input
 
  MQ131.begin(2,A0, LOW_CONCENTRATION, 1000000);
  Serial.println("Ozone Calibration...");  
  MQ131.calibrate();
  Serial.println("Calibration Done");
  Serial.println();
}

void loop() {

   Serial.println("Sampling...");
   MQ131.sample();
   conc_O3= MQ131.getO3(UG_M3); // gives the output in micro gram/meter cube
   Serial.print("O3 level= ");
   Serial.print(conc_O3);
   Serial.print("ug/m3");
  
   conc_CO= 1000*analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
    if(conc_CO<0){
      
      conc_CO= -(conc_CO);
      
      }
   Serial.print("\nCO level= ");
   
   Serial.print(conc_CO);
   Serial.println("ug/m3");
  
   level_dB= map(analogRead(AOUTpin1),0,1023,0,52); //mapping sensor input value to 0-52 range
   Serial.print("Sound level= ");
   Serial.print(level_dB);
   Serial.println("dB");

  
   if (!pm10.isReady() && (millis() >= timer + INTERVAL_COUNTDOWN)) {
    timer += INTERVAL_COUNTDOWN;
  } else if (millis() >= timer + INTERVAL_READ) {
    timer += INTERVAL_READ;
    pm2_5=1000*pm25.readPM();
    Serial.print("pm2.5: "+String(pm2_5)); 
     Serial.println("ug/m3");   
  }

 // AQI calculation for CO
   if( conc_CO>0 && conc_CO<=1000){          //if conc. of CO is 0 < CO <= 1000ug/m3 then set lower limit =0 and upper limit = 50
     Lco=0;
     Uco=50;
    }
   else if(conc_CO>1000 && conc_CO<=2000){   //if conc. of CO is 1000 < CO <= 2000ug/m3 then set lower limit =51 and upper limit = 100
     Lco=51;
     Uco=100;
    }
   else if(conc_CO>2000 && conc_CO<=10000){  //if conc. of CO is 2000 < CO <= 10000ug/m3 then set lower limit =101 and upper limit = 200
     Lco=101;
     Uco=200;
    }
   else if(conc_CO>10000 && conc_CO<=17000){ //if conc. of CO is 10000 < CO <= 17000ug/m3 then set lower limit =201 and upper limit = 300
     Lco=201;
     Uco=300;
    }
   else if(conc_CO>17000 && conc_CO<=34000){ //if conc. of CO is 17000 < CO <= 34000ug/m3 then set lower limit =301 and upper limit = 400
     Lco=301;
     Uco=400;
    }
   else if(conc_CO>34000){                   //if conc. of CO is  greater than 34000ug/m3 then set lower limit =401 and upper limit = 500
     Lco=401;
     Uco=500;
    }

 // aqi calculation for O3
   if( conc_O3>=0 && conc_O3<=50){
     Lo3=0;
     Uo3=50;
    }
   else if(conc_O3>=51 && conc_O3<=100){
     Lo3=51;
     Uo3=100;
    }
   else if(conc_O3>=101 && conc_O3<=168){
     Lo3=101;
     Uo3=200;
    }

   else if(conc_O3>=169 && conc_O3<=208){
     Lo3=201;
     Uo3=300;
    }

   else if(conc_O3>=209 && conc_O3<=748){
     Lo3=301;
     Uo3=400;
    }

   else if(conc_O3>748){
     Lo3=401;
     Uo3=500;
    }


 // aqi calculation for PM2.5
   if( 1000*pm25.readPM()>0 && 1000*pm25.readPM()<=30){
     Lpm=0;
     Upm=50;
    }
   else if(1000*pm25.readPM()>=31 && 1000*pm25.readPM()<=60){
     Lpm=51;
     Upm=100;
    }
   else if(1000*pm25.readPM()>=61 && 1000*pm25.readPM()<=90){
     Lpm=101;
     Upm=200;
    }
   else if(1000*pm25.readPM()>=91 && 1000*pm25.readPM()<=120){
     Lpm=201;
     Upm=300;
    }
   else if(1000*pm25.readPM()>=121 && 1000*pm25.readPM()<=250){
     Lpm=301;
     Upm=400;
    }
   else if(1000*pm25.readPM()>250 ){
     Lpm=401;
     Upm=500;
    }

 // now we are comparing the upper limit of each pollutant 
 // and we will store the one which has highest upper limit
   if(Uco>Uo3){
     AQI_L=Lco;
     AQI_U=Uco;
    }
   else{
     AQI_L=Lo3;
     AQI_U=Uo3;
    }
   if(Upm>AQI_U){
     AQI_L=Lpm;
     AQI_U=Upm;
    }

   if( AQI_L==0 && AQI_U==50){
     quality="GOOD";
    }
   else if(AQI_L==51 && AQI_U==100){
     quality="SATISFACTORY";
    }
   else if(AQI_L==101 && AQI_U==200){
     quality="MODERATE";
    }
   else if(AQI_L==201 && AQI_U==300){
     quality="POOR";
    }
   else if(AQI_L==301 && AQI_U==400){
     quality="VERY POOR";
    }
   else if(AQI_L==401 && AQI_U==500){
     quality="SEVERE";
    }

  Serial.print("AQI is :");
  Serial.print(AQI_L);
  Serial.print(" - ");
  Serial.println(AQI_U);
  Serial.print("Air Quality: ");
  Serial.println(quality);
  Serial.println();
  delay(60000);
 
}
