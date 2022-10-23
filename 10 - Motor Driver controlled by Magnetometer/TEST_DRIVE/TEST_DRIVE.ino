#include <Wire.h>
#include <math.h>
#define addr 0x1E //Device Adress HMC5883L

float arah;
int MotorSpeed;

int EnableMotor = 3;      
int Logic1 = 4;       
int Logic2 = 5;       



void setup() {
  //Inisiasi Magnetometer
  Serial.begin(9600);     
  Wire.begin();  
  //Konfigurasi Mode
  Wire.beginTransmission(addr); 
  Wire.write(0x02); 
  Wire.write(0x00); //Continous Measure
  Wire.endTransmission();
  //Konfigurasi Register B
  Wire.beginTransmission(addr); 
  Wire.write(0x01); 
  Wire.write(0x20); // Gain = (0xE0 = 219, 0xC0 = 307, Default=0x20)
  Wire.endTransmission();
  delay(20);

  pinMode(EnableMotor, OUTPUT);
  pinMode(Logic1, OUTPUT);  
  pinMode(Logic2, OUTPUT);
}

double get_compass(){
    short x_raw,y_raw,z_raw; 
    double az;
    Wire.beginTransmission(addr);
    Wire.write(0x03); 
    Wire.endTransmission();
    Wire.requestFrom(addr, 6);
  if(6<=Wire.available()){
    x_raw = Wire.read()<<8; 
    x_raw |= Wire.read(); 
    z_raw = Wire.read()<<8;
    z_raw |= Wire.read(); 
    y_raw = Wire.read()<<8;
    y_raw |= Wire.read(); 
  }
  az = atan2(x_raw, y_raw) * 180 / M_PI;
  return az;
}



void loop() {
  digitalWrite(Logic1, HIGH);
   digitalWrite(Logic2, LOW);
   analogWrite(EnableMotor, 127);

}
