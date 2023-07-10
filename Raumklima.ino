

/*   
 *   Basic example code for controlling a stepper without library
 *      
 *   by Dejan, https://howtomechatronics.com
 */

// defines pins for motor
#define stepPin 2
#define dirPin 5 
#define zeroPos 8
#define reset 9

// include Bib for sensor
#include "Seeed_BME280.h"
#include <Wire.h> 



BME280 bme280;
//Var
int PERFTEMP = 22;
int PERFHUM = 50;
long currentPos = 0;
long diffHum = 0;
long diffTemp = 0;
boolean direct = HIGH;
boolean res = LOW;


 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(zeroPos,INPUT);
  pinMode(reset,INPUT);
  // Vars
  

  // BME config
  Serial.begin(9600); // Einstellen der Baudrate
   if(!bme280.init()) // Wenn keine Daten vom BME abgefragt werden können...
   
        {
          Serial.println("No Data from BME!"); // ...dann soll eine Fehlermeldung ausgegeben werden.
        }
        
    //Set zero position of Scale
  
  
  digitalWrite(dirPin,LOW);//direction Clockwise
  
  while (digitalRead(zeroPos) == LOW){
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(50);    
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(50); 
  }
  delay(500);
  digitalWrite(dirPin,HIGH);//direction CounterClockwise
  for(long h = 0; h < 71000; h++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(50);
  }
  
  
  digitalWrite(dirPin,LOW);//direction Clockwise (von null)

  
  
}
void loop() {

 
  
  long temp = bme280.getTemperature();
  long humidity = bme280.getHumidity();
  //Ausgabe zum testen;

  

  
  Serial.print("Temperatur: ");
  Serial.println(temp);
  Serial.print("Feuchtigkeit: ");
  Serial.println(humidity);
  



  diffTemp = PERFTEMP - temp;
  diffTemp = diffTemp % 1;//Differenztemperatur zur Optimaltemperatur (mit mod auf ganze gerundet
  diffHum = abs(PERFHUM - humidity);

  long newPos = (diffTemp * 1500) + (diffHum * 700); //z.b. 12grad + 20 feucht ......ggf Faktor dazu jeweils, je nachdem wie viele stepps eine umdrehunbg sind. 
  long steps = newPos - currentPos;
  currentPos = newPos;
  
  Serial.print("steps: ");
  Serial.println(steps);
  Serial.print("Position: ");
  Serial.println(newPos);

  delay(5000);
  
  if(steps >= 0){
    digitalWrite(dirPin,LOW);
  }
  else{
    digitalWrite(dirPin,HIGH);
    steps = steps * -1;
  }
  for(int x = 0; x <= steps && digitalRead(zeroPos) == LOW; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(800);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(800); 
  }
  

  
  


}
