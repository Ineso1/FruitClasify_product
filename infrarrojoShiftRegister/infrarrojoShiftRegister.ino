#include <Servo.h>
 
int servoC = 5;
int infra2 = 7;
int infra3 = 6;
int pulsoEntrada = 10;
int pulsoSalida = 11;
int pulsoOperacion = 9;
int pulsoFinalLinea = 2;

bool Entrada,Salida;

Servo servoClasificacion;

void setup() {
  Serial.begin(9600);
  servoClasificacion.attach(servoC);
  servoClasificacion.write(0);

  pinMode(infra2, INPUT_PULLUP);
  pinMode(infra3, INPUT_PULLUP);
  pinMode(pulsoEntrada ,OUTPUT);
  pinMode(pulsoSalida, OUTPUT);
  pinMode(pulsoOperacion,OUTPUT);
  pinMode(pulsoFinalLinea,INPUT);
  Entrada = false;
  Salida = false;
 
}

void loop() {

  Entrada = !digitalRead(infra2);
  Salida = !digitalRead(infra3);
  
  //Serial.print(Entrada);
  //Serial.print(" ");
  //Serial.println(Salida);
  if (!Salida){
    digitalWrite(pulsoOperacion, HIGH);
    delay(200);
    if(digitalRead(pulsoFinalLinea)){
      servoClasificacion.write(55);
      }
    else{
      servoClasificacion.write(0);
      }
    }
  else{
    digitalWrite(pulsoOperacion, LOW);
    }
  delay(50);

  
  digitalWrite(pulsoEntrada, Entrada);
  digitalWrite(pulsoSalida, Salida);
  Serial.print(Entrada);
  Serial.println(Salida);


  delay(100);
}
