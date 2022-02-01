# include <Wire.h>
#include <PZEM004Tv30.h>

/* Use software serial for the PZEM
 * Pin 11 Rx (Connects to the Tx pin on the PZEM)
 * Pin 12 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(2, 3); // 2 ke Tx, 3 ke Rx

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 20, 4);

float Pin = 41.751;
int Htime_rpm;              //integer for storing high time
int Ltime_rpm;                //integer for storing low time
float Ttime_rpm;            // integer for storing total time of a cycle
float frequency_rpm;        //storing frequency
int rpm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(7,INPUT); //pin rpm
  lcd.clear();
}
void PZEM() {
    int voltage = pzem.voltage();
    lcd.setCursor(0,0);
    lcd.print("V= ");
    lcd.setCursor(2,0);
    lcd.print(voltage);
    //lcd.print('V');
    if( !isnan(voltage) ){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    float current = pzem.current();
    lcd.setCursor(9,0); lcd.print("A= "); lcd.setCursor(11,0); lcd.print(current); //Serial.println("A");
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current); Serial.println("A");
        
    } else {
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    lcd.setCursor(9,1); lcd.print("P= "); lcd.setCursor(11,1); lcd.print(power); //lcd.println("W");
    float n1 = power/Pin;
    float n = n1*100; 
    lcd.setCursor(9,2); lcd.print("n= "); lcd.setCursor(11,2); lcd.print(n); //lcd.println("W");
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power); Serial.println("W"); Serial.print(n);
    } else {
        Serial.println("Error reading power");
    }

    float energy = pzem.energy();
    lcd.setCursor(0,1); lcd.print("E= "); lcd.setCursor(3,1); lcd.print(energy,3); // lcd.println("kWh");
    if( !isnan(energy) ){
        Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
        
    } else {
        Serial.println("Error reading energy");
    }

    int frequency = pzem.frequency();
    lcd.setCursor(0,2); lcd.print("F= "); lcd.setCursor(3,2); lcd.print(frequency, 1); //lcd.println("Hz");
    if( !isnan(frequency) ){
        Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
        
    } else {
        Serial.println("Error reading frequency");
    }

    float pf = pzem.pf();
    lcd.setCursor(9,3);lcd.print("PF= "); lcd.setCursor(13,3); lcd.print(pf); delay(500); // lcd.setCursor(13,3); lcd.print("0,93"); delay(800);lcd.setCursor(13,3); lcd.print("0,91"); delay(500);lcd.setCursor(13,3); lcd.print("0,90"); delay(400);
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
       
    } else {
        Serial.println("Error reading power factor");
    }
    
    delay(500);
}

void RPM1 () {
  Htime_rpm = pulseIn(7,HIGH);      //read high time
  Ltime_rpm = pulseIn(7,LOW);        //read low time
  Ttime_rpm = Htime_rpm + Ltime_rpm;
  frequency_rpm = 1000000/Ttime_rpm;    //getting frequency with Ttime is in Micro seconds
  if (frequency_rpm >= 0) {
    rpm = frequency_rpm*60;
  }
  Serial.println(rpm);
  lcd.setCursor(0,3);
  lcd.print("RPM=");
  lcd.setCursor(4,3);
  lcd.print(rpm);
//  delay(500);
//  lcd.setCursor(4,3);
//  lcd.print("1500");
//  delay(500);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  PZEM();
  RPM1();
}
