#include <SoftWire.h>
#include <Wire.h>

//Código para magnetometro HMC5883L
//Pines
//B6 (SCL),B7 (SDA),5V,G en STM32
//Proyecto ARLISS 2021

#define address 0x1E //0011110b, I2C 7bit address of HMC5883

void setup(){  
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  Wire.begin();  
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop(){
  short x,y,z; //triple axis data
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  
 
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
  }
  float angulo = atan2(y, x);
    angulo=angulo*(180/M_PI);//convertimos de Radianes a grados
    angulo=angulo-1.11; //corregimos la declinación magnética
    //Mostramos el angulo entre el eje X y el Norte
    Serial.print("AnguloX-N: ");
    Serial.println(angulo,0);
    delay(100);
}
