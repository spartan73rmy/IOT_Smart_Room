/*-----------------------------------------------------.::|PrototypeMethods|::.----------------------------------*/
void DeclararIOPines();
void IniciarBluethooth();
void toggle(int PinToggle);
void medirTemperaturaHumedad();
/*-----------------------------------------------------.::|Bluethooth|::.----------------------------------*/
#include <SoftwareSerial.h>
#define RxD 0
#define TxD 1
#define RST 5 // Encendido del Modulo
#define KEY 4
SoftwareSerial BTSerial(RxD, TxD);
/*-----------------------------------------------------.::|DHT 11|::.----------------------------------*/

#include <SimpleDHT.h>
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  
// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);
/*-----------------------------------------------------.::|Salidas|::.----------------------------------*/
#define LEDS 4

byte bitToggle = 0;


/*-----------------------------------------------------.::|Programm|::.----------------------------------*/

void setup()
{
  IniciarBluethooth();
  DeclararIOPines();

}

void loop()
{
delay(1000);

  // Esperamos datos
  if (BTSerial.available()) {

    // Leemos el caracter
    char command = BTSerial.read();
    BTSerial.flush();
    Serial.println(command);

    switch (command) {
      case '1':
        Serial.println("Encendidas");
        digitalWrite(LEDS, HIGH);
        break;
      case '0':
        digitalWrite(LEDS, LOW);
        Serial.println("No Encendidas");

        break;
    }
  }
delay(1000);
  medirTemperaturaHumedad();
}


void toggle(int pinNum) {
  // Establece el pin del LED usando la variable pinEstado:
  digitalWrite(pinNum, bitToggle);
  // si el pinEstado = 0, lo establece a 1, y vice versa:
  bitToggle = !bitToggle;
}

void DeclararIOPines() {
  pinMode(LEDS, OUTPUT);
  pinMode(pinDHT11,INPUT);
}

void IniciarBluethooth() {
  /*
    #define RxD 10
    #define TxD 11
    #define RST 5 // Encendido del Modulo
    #define KEY 4
  */

  pinMode(RST, OUTPUT);
  pinMode(KEY, OUTPUT);


  // Estado inicial
  digitalWrite(RST, LOW);
  // Modo Comunicacion
  digitalWrite(KEY, LOW);

  // Encendemos el modulo.
  digitalWrite(RST, HIGH);

  // Configuracion del puerto serie por software
  // para comunicar con el modulo HC-05
  BTSerial.begin(9600);
  BTSerial.flush();
  delay(500);

  // Configuramos el puerto serie de Arduino para Debug
  Serial.begin(9600);
  Serial.println("------------------.::|Bluethooth Ready|::.------------------");

}

void medirTemperaturaHumedad(){
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
//    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" °C, "); 
  Serial.print((int)humidity); Serial.println(" H");
  
  // DHT11 sampling rate is 1HZ.
  delay(1500);
  }



