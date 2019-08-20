/*-----------------------------------------------------.::|Salidas|::.----------------------------------*/
#define vent 5
#define servo 6
#define pinLed 7
#define bomba 8
byte bitToggle = 0;
/*-----------------------------------------------------.::|Entradas|::.----------------------------------*/
#define boton A4
#define humed A3
//Entrada de luz
#define lightPin A0
//Iguales definir a usar

#define pinDHT11 3
#define pinLM35 3

#define hcT 11
#define hcE 12

/*-----------------------------------------------------.::|HCSR04|::.----------------------------------*/

#include <HCSR04.h>
UltraSonicDistanceSensor distanceSensor(hcE, hcT); // Initialize sensor that uses digital pins 11 and 12.

/*-----------------------------------------------------.::|Servo|::.----------------------------------*/

#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
#define grados 90
/*-----------------------------------------------------.::|DHT 11|::.----------------------------------*/

#include <SimpleDHT.h>
byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;

// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
SimpleDHT11 dht11(pinDHT11);

/*-----------------------------------------------------.::|Prototipos|::.----------------------------------*/
int medirTemperaturaHumedad();
int medirNivelAgua();
int medirTemperatura();

void moverPuerta();
void encenderVentilador();
void encenderLed();
void regar();
#define wait delay(500);
/*-----------------------------------------------------.::|Programm|::.----------------------------------*/

void setup() {
  Serial.begin(9600);
  wait
  //----------------------------------------------Entradas
  pinMode(boton, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(pinDHT11, INPUT);
  //----------------------------------------------Salidas
  pinMode(servo, OUTPUT);
  pinMode(vent, OUTPUT);
  //  pinMode(luz, OUTPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(bomba, OUTPUT);

  myservo.attach(servo);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  int luminosidad = medirLuminosidad();
  wait
  //Checar temperatura
  int temperatura = medirTemperaturaHumedad();
  wait
  //Medir nivel de agua
  int distanciaAgua = medirNivelAgua();
  wait
  int humedad = leerHumedad();
  wait


  if (luminosidad < 100) {
    encenderLed(true);
  } else {
    encenderLed(false);
  }
  if (temperatura > 25) {
    encenderVentilador(true);
  } else {
    encenderVentilador(false);
  }
  Serial.println(temperatura);

  if (humedad > 250) {
    Serial.println("---------Plantas Secas Falta Regar-----------");
    if (distanciaAgua > 12) {
      Serial.println("---------Nivel de agua menor de 1cm aprox-----------");
      Serial.println(distanciaAgua);
      regar(false);
    } else {
      regar(true);
    }
  } else {
    Serial.println("---------Plantas humedas,no regar-----------");
    regar(false);
  }


  if (abrirPuerta()) {
    moverPuerta();
  } else {
    Serial.println("Puerta cerrada");
    myservo.write(grados);              // tell servo to go to position in variable 'pos'
  }

}

int medirTemperatura() {
  float tempC = 0.0;
  tempC = analogRead(pinLM35);
  Serial.println("=================================");
  Serial.println("Midiendo Temperatura LM35");
  tempC = (5.0 * tempC * 100.0) / 1024.0;
  Serial.println("Midiendo Temperatura LM35");
  return (int)tempC;
}

int medirNivelAgua() {
  int cm = distanceSensor.measureDistanceCm();
  Serial.println("=================================");
  Serial.println("Distancia de agua");
  Serial.print(cm);
  Serial.println("cm");
  return cm;
}

bool abrirPuerta() {
  return (digitalRead(boton) == HIGH) ? true : false;
}

int medirTemperaturaHumedad() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return -1;
  }
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" °C, ");
  Serial.print((int)humidity); Serial.println(" H");

  // DHT11 sampling rate is 1HZ.
  delay(1500);
  return (int)temperature;
}

int medirLuminosidad() {
  int lum = analogRead(lightPin);
  Serial.println("=================================");
  Serial.println("Luminosidad");
  Serial.println(lum);
  return lum;
}

/*-----------------------------------------------------.::|Acciones|::.----------------------------------*/

void moverPuerta() {
  Serial.println("=================================");
  Serial.println("Abriendo puerta");
  
  Serial.println("=================================");
  Serial.println("Cerrando puerta");
  for (pos = grados; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
  
  for (int i = 0; i < 4; i++) {
    wait
  }
  for (pos = 0; pos <= grados; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }
}

void encenderVentilador(bool band) {
  Serial.println("=================================");
  if (band) {
    Serial.println("Encendiendo ventildor");
    digitalWrite(vent, HIGH);
    for (int i = 0; i < 4; i++) {
      wait
    }
  } else {
    Serial.println("Apagando ventildor");
    digitalWrite(vent, LOW);
  }
}

void regar(bool band) {
  if (!band) {
    Serial.println("=================================");
    Serial.println("Encendiendo bomba de agua");
    digitalWrite(bomba, HIGH);
    for (int i = 0; i < 4; i++) {
      wait
    }
  }
  else {
    Serial.println("=================================");
    Serial.println("Apagando bomba de agua");
        digitalWrite(bomba, LOW);
    for (int i = 0; i < 4; i++) {
      wait
    }
  }
}

int leerHumedad() {
  int l = analogRead(humed);
  Serial.println("=================================");
  Serial.println("Humedad en maceta");
  Serial.println(l);
  return l;
}


void encenderLed(bool band) {
  Serial.println("=================================");
  Serial.println("Encendiendo led");
  if (band) {
    digitalWrite(pinLed, HIGH);
    for (int i = 0; i < 4; i++) {
      wait
    }
  } else {
    digitalWrite(pinLed, LOW);
  }
}


