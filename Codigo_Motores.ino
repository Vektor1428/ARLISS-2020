//Codigo para locomoción utilizando el driver dual TB6612FNG
//Proyecto PROE

const int PWMA=PB9;
const int PWMB=PB8;
const int stdby=PA11;

const int INB1=PB3;
const int INB2=PB4;
const int INA1=PA15;
const int INA2=PA12;


void setup() {
 pinMode(INB1,OUTPUT);
 pinMode(INB2,OUTPUT);
 pinMode(INA1,OUTPUT);
 pinMode(INA2,OUTPUT);
 pinMode(stdby,OUTPUT);
}

void loop() {
  digitalWrite(stdby,HIGH);
  delay(2000);
  ConfiguraEscribePuenteH(200,200);
  delay(2000);
  ConfiguraEscribePuenteH(-200,-200);
  delay(2000);
  ConfiguraEscribePuenteH(200,-200);
  delay(2000);
  ConfiguraEscribePuenteH(-200,200);
  delay(2000);
  exit(0);
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
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, LOW);
}

void ConfiguracionAvanzar() {
  //Permite el avance del carro al poner las patillas correspondientes del puente H en alto
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
}

void ConfiguracionRetroceder() {
  //Configura el carro para retroceder al poner las patillas correspondientes del puente H en alto
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
}

void ConfiguracionGiroDerecho() {
  //Configura las patillas del puente H para realizar un giro derecho
  digitalWrite(INA1, HIGH);
  digitalWrite(INA2, LOW);
  digitalWrite(INB1, LOW);
  digitalWrite(INB2, HIGH);
}

void ConfiguracionGiroIzquierdo() {
  //Configura las patillas del puente H para realizar un giro izquierdo
  digitalWrite(INA1, LOW);
  digitalWrite(INA2, HIGH);
  digitalWrite(INB1, HIGH);
  digitalWrite(INB2, LOW);
}
