//Codigo para locomoción utilizando el driver L298N
//Proyecto ARLISS
//Pines para PWM

const int PWMA=PB8;
const int PWMB=PB9;

//Pines para direccion
const int INB4=PA0;
const int INB3=PA1;
const int INA1=PA2;
const int INA2=PA3;

//Pines para encoders
const int ENC_A_C1=PB12;
const int ENC_A_C2=PB13;
const int ENC_B_C1=PB14;
const int ENC_B_C2=PB15;

const int ppr=540; //pulsos por revolución

//Contadores de los encoders
int contPulsos_A=0;
int contPulsos_B=0;
int contPulsos_A_Pasado=0;
int contPulsos_B_Pasado=0;

//Variables que almacenan el desplazamiento angular de cada rueda
float posActualRueda_A=0.0;
float posActualRueda_B=0.0;

//Variables del valor de velocidad en cada rueda
float velActual_A=0.0;
float velActual_B=0.0;

void setup() {
 pinMode(INB3,OUTPUT);
 pinMode(INB4,OUTPUT);
 pinMode(INA1,OUTPUT);
 pinMode(INA2,OUTPUT);
 pinMode(PWMA, PWM);
 pinMode(PWMB, PWM);
 pinMode(ENC_A_C1,INPUT_PULLDOWN);
 pinMode(ENC_A_C2,INPUT_PULLDOWN);
 pinMode(ENC_B_C1,INPUT_PULLDOWN);
 pinMode(ENC_B_C2,INPUT_PULLDOWN);
 attachInterrupt(ENC_A_C1, PulsosRuedaAC1,CHANGE);  //conectado el contador C1 motor A
 attachInterrupt(ENC_A_C2, PulsosRuedaAC2,CHANGE); 
 attachInterrupt(ENC_B_C1, PulsosRuedaBC1,CHANGE);  //conectado el contador C1 motor B
 attachInterrupt(ENC_B_C2, PulsosRuedaBC2,CHANGE);
 delay(1000);
}

void loop() {
  ConfiguraEscribePuenteH(150,150);
}

void resetContadores(){
    contPulsos_A=0;
    contPulsos_B=0;
    contPulsos_A_Pasado=0;
    contPulsos_B_Pasado=0;
 }

void PulsosRuedaAC1(){
  
  }

void PulsosRuedaAC2(){
  
  }
 
void PulsosRuedaBC1(){
  
  }
 
void PulsosRuedaBC2(){
  
  }
  
void ConfiguraEscribePuenteH (int pwmRuedaDer, int pwmRuedaIzq) {
  //Determina si es giro, avance, o retroceso en base a los valores de PWM y configura los pines del Puente H
  if (pwmRuedaDer >= 0 && pwmRuedaIzq >= 0) {
    ConfiguracionAvanzar();
    analogWrite(PWMB, pwmRuedaDer);
    analogWrite(PWMA, pwmRuedaIzq);

  } else if (pwmRuedaDer < 0 && pwmRuedaIzq < 0) {
    ConfiguracionRetroceder();
    analogWrite(PWMB, -pwmRuedaDer);
    analogWrite(PWMA, -pwmRuedaIzq);

  } else if (pwmRuedaDer > 0 && pwmRuedaIzq < 0) {
    ConfiguracionGiroDerecho();
    analogWrite(PWMB, pwmRuedaDer);
    analogWrite(PWMA, -pwmRuedaIzq);

  } else if (pwmRuedaDer < 0 && pwmRuedaIzq > 0) {
    ConfiguracionGiroIzquierdo();
    analogWrite(PWMB, -pwmRuedaDer);
    analogWrite(PWMA, pwmRuedaIzq);
  }
}

void ConfiguracionParar() {
  //Deshabilita las entradas del puente H, por lo que el carro se detiene
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, LOW);
  digitalWrite(INB3, LOW);
  digitalWrite(INB4, LOW);
}

void ConfiguracionAvanzar() {
  //Permite el avance del carro al poner las patillas correspondientes del puente H en alto
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  digitalWrite(INB3, LOW);
  digitalWrite(INB4, HIGH);
}

void ConfiguracionRetroceder() {
  //Configura el carro para retroceder al poner las patillas correspondientes del puente H en alto
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  digitalWrite(INB3, HIGH);
  digitalWrite(INB4, LOW);
}

void ConfiguracionGiroDerecho() {
  //Configura las patillas del puente H para realizar un giro derecho
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  digitalWrite(INB3, LOW);
  digitalWrite(INB4, HIGH);
}

void ConfiguracionGiroIzquierdo() {
  //Configura las patillas del puente H para realizar un giro izquierdo
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  digitalWrite(INB3, HIGH);
  digitalWrite(INB4, LOW);
}
