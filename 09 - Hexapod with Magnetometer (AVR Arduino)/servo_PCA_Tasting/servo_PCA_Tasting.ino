#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <math.h>


#define SERVOMIN  150 
#define SERVOMAX  600 
#define addr 0x1E //I2C Address for The HMC5883

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//Array Pose Gerakan Maju
const int movement_map[9][8] = {
 {300,300,380,250,300,300,300,300},
 {300,300,380,300,300,300,300,300},
 {300,300,380,300,200,250,300,300},
 {300,300,380,300,200,300,300,300},
 {200,300,300,300,300,300,380,300},
 {300,250,300,300,300,300,380,300},
 {300,300,300,300,300,300,380,300},
 {300,300,300,300,300,300,300,250},
 {300,300,300,300,300,300,300,300}
};

//Parameter kecepatan gerakan
int speed = 250;

void setup() {
  Serial.begin(9600);       
  pwm.begin();              
  pwm.setPWMFreq(50);  
  delay(20);

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
}

  

//Fungsi gerakan maju
void move_forward(){
  for(int i=0;i<9;i++){
      for(int j=0;j<8;j++){
        pwm.setPWM(j, 0,  movement_map[i][j]);
        //delay(20);          
        }
        delay(speed);
      }
  }

//Posisi Pose Awal
void initial_position(){
  for(int i=0;i<8;i++){
    pwm.setPWM(i, 0,  movement_map[8][i]);
  }
  delay(speed);
}


//Fungsi Putar Balik
void putar180(){
  for(int i=0;i<12;i++){
  pwm.setPWM(0, 0,  380);
  pwm.setPWM(4, 0,  380);
  pwm.setPWM(1, 0,  250);
  pwm.setPWM(5, 0,  250);
  delay(speed);
  pwm.setPWM(1, 0,  300);
  pwm.setPWM(5, 0,  300);
  delay(speed);
  pwm.setPWM(2, 0,  380);
  pwm.setPWM(6, 0,  380);
  pwm.setPWM(3, 0,  250);
  pwm.setPWM(7, 0,  250);
  delay(speed);
  pwm.setPWM(3, 0,  300);
  pwm.setPWM(7, 0,  300);
  delay(speed);
  initial_position();
  }
  
}

//Arah Kompas dalam sudut derajat
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
  az = atan2(x_raw, y_raw) * 180 / M_PI + 180.0;
  return az;
}

//Putar Ke Kiri, digunakan untuk koreksi arah
void geser_kiri(int offset){
  pwm.setPWM(0, 0,  offset);
  pwm.setPWM(4, 0,  offset);
  pwm.setPWM(1, 0,  250);
  pwm.setPWM(5, 0,  250);
  delay(speed);
  pwm.setPWM(1, 0,  300);
  pwm.setPWM(5, 0,  300);
  delay(speed);
  pwm.setPWM(2, 0,  offset);
  pwm.setPWM(6, 0,  offset);
  pwm.setPWM(3, 0,  250);
  pwm.setPWM(7, 0,  250);
  delay(speed);
  pwm.setPWM(3, 0,  300);
  pwm.setPWM(7, 0,  300);
  delay(speed);
  initial_position();
}

//Putar Ke Kanan, digunakan untuk koreksi arah
void geser_kanan(int offset){
  pwm.setPWM(0, 0,  offset);
  pwm.setPWM(4, 0,  offset);
  pwm.setPWM(1, 0,  250);
  pwm.setPWM(5, 0,  250);
  delay(250);
  pwm.setPWM(1, 0,  300);
  pwm.setPWM(5, 0,  300);
  delay(250);
  pwm.setPWM(2, 0,  offset);
  pwm.setPWM(6, 0,  offset);
  pwm.setPWM(3, 0,  250);
  pwm.setPWM(7, 0,  250);
  delay(250);
  pwm.setPWM(3, 0,  300);
  pwm.setPWM(7, 0,  300);
  delay(250);
  initial_position();
}

void loop() {
  initial_position();
  delay(3000);
  double toleransi = 5;
  double setpoint = get_compass();
  double current_pos;
  float batas_atas,batas_bawah,offset;
  
  batas_atas =  setpoint + toleransi;
    if(batas_atas > 360){
      batas_atas = batas_atas - 360;
    }
  batas_bawah = setpoint - toleransi;
    if(batas_bawah < 0){
      batas_bawah = 360 + batas_bawah;
    }

  //Maju setiap kali dalam batas toleransi, dan koreksi setelahnya.
  for(int maju=0;maju<25;){
    current_pos = get_compass();
    if(abs(setpoint-current_pos)<=toleransi){
       for(int loop=0;loop<2;loop++){
          move_forward();
       }
       //Serial.print("MAJU ");
       //Serial.println(current_pos);
       maju++;
    }else if(current_pos < batas_bawah){
      //Serial.print("Geser ke KIRI =");
      //Serial.println((setpoint-toleransi)-current_pos);
      //Serial.println(current_pos);
      offset = (setpoint-toleransi)-current_pos;
      if(offset < 45){
        geser_kiri(340);
      }else if(offset > 45 || offset < 180){
        geser_kiri(380);
      }
      
    }else if(current_pos > batas_atas){
      //Serial.print("Geser ke KANAN =");
      //Serial.println(current_pos-(setpoint+toleransi));
      //Serial.println(current_pos);
      offset = current_pos-(setpoint+toleransi);
      if(offset < 45){
        geser_kanan(240);
      }else if(offset > 45 || offset < 180){
        geser_kanan(200);
      } 
    }
  }

  //Koreksi ulang sebelum putar balik
    current_pos = get_compass();
    while(current_pos < batas_bawah || current_pos > batas_atas){
      current_pos = get_compass();
      if(current_pos < batas_bawah){
      //Serial.print("Geser ke KIRI =");
      //Serial.println((setpoint-toleransi)-current_pos);
      //Serial.println(current_pos);
      offset = (setpoint-toleransi)-current_pos;
      if(offset < 45){
        geser_kiri(340);
      }else if(offset > 45 || offset < 180){
        geser_kiri(380);
        }
      }else if(current_pos > batas_atas){
      //Serial.print("Geser ke KANAN =");
      //Serial.println(current_pos-(setpoint+toleransi));
      //Serial.println(current_pos);
      offset = current_pos-(setpoint+toleransi);
      if(offset < 45){
        geser_kanan(240);
      }else if(offset > 45 || offset < 180){
        geser_kanan(200);
        } 
      }  
    }      
    putar180();
}
