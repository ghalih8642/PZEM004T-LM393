# include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 20, 4);

int encoder_pin = 4;  // The pin the encoder is connected           
unsigned int rpm;     // rpm reading
volatile byte pulses;  // number of pulses
unsigned long timeold; 
// The number of pulses per revolution
// depends on your index disc!!
unsigned int pulsesperturn = 20;
void counter()
 {
    //Update count
      pulses++;    
 }
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(encoder_pin, INPUT);
   
  //Interrupt 0 is digital pin 2, so that is where the IR detector is connected
  //Triggers on FALLING (change from HIGH to LOW)
  attachInterrupt(0, counter, FALLING);
  // Initialize
  pulses = 0;
  rpm = 0;
  timeold = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
   if (millis() - timeold >= 1000){ 
 
   detachInterrupt(0);
  
   rpm = (60 * 1000 / pulsesperturn )/ (millis() - timeold)* pulses;
   if (rpm <= 79){
       rpm = 0;
   }    
   timeold = millis();
   pulses = 0;
   lcd.setCursor(0,3);
   lcd.print("RPM= ");
   lcd.print(rpm,DEC);
   delay(500);
   //Write it out to serial port
   if( !isnan(rpm) ){
     Serial.print("RPM = ");
     Serial.println(rpm,DEC);
    } else {
        Serial.println("Error reading RPM");
    }
   
   //Restart the interrupt processing
   attachInterrupt(0, counter, FALLING);
   }
}
