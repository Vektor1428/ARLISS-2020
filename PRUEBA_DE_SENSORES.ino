//PARA CORRER ESTE CODIGO ES NECESARIO TENER LAS SIGUIENTES LIBRERIAS INSTALADAS PARA EL STM32 BLUE PILL
#include <Wire.h>
#include <SFE_BMP180.h>
#include <TinyGPS++.h>
#include <hmc5883l.h>
HMC5883L compass;
TinyGPSPlus gps;
SFE_BMP180 bmp180;
double PresionNivelMar=1001.2; //Segun estacion meteorologica de la UCR en Liberia, Guanacaste
float declinacion=2.09; //Declinación magnética para la zona de San Jose, Costa Rica
//Altura en la UCR: 1168 msnm (debe corregirse para la presion atmosferica dada)


void setup()
{
  //DECLARACIÓN DE PINES, ESTABLECIMIENTO DE VELOCIDADES DE COMUNICACIÓN E INICIALIZACIÓN DE SENSORES
  Serial1.begin(9600);
  Serial2.begin(9600);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  bmp180.begin();
  pinMode(LED_BUILTIN,OUTPUT); //Enciende el led del STM32 si todo se inicializó correctamente
}

void loop()
{
  //DECLARACIÓN DE TODAS LAS VARIABLES Y CONSTANTES A USAR EN EL ALGORITMO DE BÚSQUEDA
  double latitud1,longitud1; //latitud y longitud del rover dadas por el gps mediante punteros
  double latitud2=9.857459; //Latitud del punto objetivo 
  double longitud2=-83.912428; //Longitud del punto objetivo
  double hbase; //Altura sobre la cual se debe realizar la misión
  int16_t gx, gy, gz; //giro del robot dado por el IMU
  const delayGPS=10000;
  double altura;
  bool locValid;
  
  GPSDelay(delayGPS); // espera 10 000 ms para que el gps se estabilice
  lecturaGPS(&latitud1,&longitud1); //obtiene las coordenadas geograficas del robot
  IMU(&gx,&gy,&gz); //datos del giroscopio
  Serial.println("-----------------Datos----------------"); //esto es una prueba para ver que todos los sensores arrojen datos coherentes
  Serial.print("Altura: ");Serial.println(Altura());
  Serial.print("Angulo: ");Serial.println(angulo());
  Serial.print("Gx: ");Serial.print(gx); Serial.print("Gy: ");Serial.print(gy); Serial.print("Gz: ");Serial.println(gz); 
  Serial.print("Latitud: "); Serial.println(latitud1);
  Serial.print("Longitud: ");Serial.println(longitud1); 
}

//FUNCIONES PARA GPS Y BUSQUEDA
void lecturaGPS(double *latitud,double *longitud) //funcion para extraer coordenadas del gps, guarda la longitud y latitud en los punteros ingresados como parametros
{
 unsigned long start;
 *latitud=gps.location.lat();
 *longitud=gps.location.lng();
}

void GPSDelay(unsigned long ms) //Se establece un retraso para comenzar a obtener data (reemplazo de la funcion delay)
{
  unsigned long start = millis();
  do
  {
    while (Serial2.available())
    gps.encode(Serial2.read());
  } while (millis() - start < ms);
}
 
int distanciaHaversine(double latitud1,double longitud1,double latitud2,double longitud2)// formula para la distancia entre 2 puntos, esta se cambiara por las ecuacione de Vincenty
{
  
  double d = 2*(6371000)*asin(sqrt(pow(sin((PI/180)*(latitud2-latitud1)/2),2) + cos((PI/180)*latitud1)*cos((PI/180)*latitud2)*pow(sin((PI/180)*(longitud2-longitud1)/2),2))); //Se aplica directamente la fÃ³rmula de Haversine pasando todos los Ã¡ngulos a radianes
  int a=int(d);
  return a;
}

//FUNCION PARA LA ALTURA

double Altura() //devuelve la altura 
{
  char status;
  double T,P,A,Hbase;
  
  status = bmp180.startTemperature();//Inicio de lectura de temperatura
  if (status != 0)
  {   
    delay(status); //Pausa para que finalice la lectura
    status = bmp180.getTemperature(T); //Obtener la temperatura
    if (status != 0)
    {
      status = bmp180.startPressure(3);//Inicio lectura de presiÃ³n
      if (status != 0)
      {        
        delay(status);//Pausa para que finalice la lectura        
        status = bmp180.getPressure(P,T);//Obtenemos la presiÃ³n
        if (status != 0)
        {                  
          
          A= bmp180.altitude(P,PresionNivelMar); //Se obtiene la altura de manera indirecta usando la presiÃ³n 
          return A; 
        }   
           
      }      
    }   
  } 
}

//FUNCIONES PARA ORIENTACION
int angulo() { // devuelve el ángulo respecto al norte magnetico
    MagnetometerRaw raw = compass.ReadRawAxis();
    int angulo = atan2(raw.YAxis,raw.XAxis)*180/PI;
    angulo=angulo-declinacion; //corregimos la declinaciÃ³n magnÃ©tica
     //Se pasa de un rango de [-180,180] a uno de [0,360]
    if(angulo<0) 
    {
      angulo=angulo+360;
      int angulo2=int(angulo); 
      return angulo2;
    }
    else
    {
      int angulo2=int(angulo);
      return angulo2;  
    }
}

int bearing(double latitud1,double longitud1,double latitud2,double longitud2) //devuelve el angulo de la trayectorio respecto al nortye geográfico
{
  //Se calculan los diferentes parÃ¡metros para la fÃ³rmula del bearing
  double y=sin((longitud2-longitud1)*(PI/180))*cos (latitud2*(PI/180));
  double x2=(cos(latitud1*(PI/180))*sin (latitud2*(PI/180)));
  double x1=sin (latitud1*(PI/180))*cos(latitud2*(PI/180))*cos((longitud2-longitud1)*(PI/180));
  double x=x2-x1;
  double l = atan2(y,x)*(180/PI);
  int angulo=int(l); 
  //Se pasa de un rango de [-180,180] a uno de [0,360]
  if (angulo<0)
  {
  int angulo2=angulo+360;
  return angulo2;
}
else
{
  return angulo;
}
}

void IMU(int16_t *gX,int16_t *gY,int16_t *gZ) // devuelve la velocidad angular y aceleración
{
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  *gX=(Wire.read()<<8 | Wire.read())*(250.0/32768.0);
  *gY=(Wire.read()<<8 | Wire.read())*(250.0/32768.0);
  *gZ=(Wire.read()<<8 | Wire.read())*(250.0/32768.0);
}
