#include <AccelStepper.h>
#include <ESP32Servo.h>

#define STEP_PIN 26
#define DIR_PIN 27
#define SERVO_PIN 15
#define RELAY_PIN 14

AccelStepper motor(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
Servo servoPuerta;

const long posEntrada = 0;
const long posSalida = 11344;
const long posInspeccion = 22419;

long destino = -1;
long posicionActual = 0;
bool cancelado = false;
bool rutinaActiva = false;

const int velocidadMovimiento = 1000;
int estadoRutina = 0;

void setup() {
  Serial.begin(115200);

  motor.setMaxSpeed(velocidadMovimiento);
  motor.setAcceleration(500);

  servoPuerta.attach(SERVO_PIN);
  servoPuerta.write(90);  // Puerta abierta al iniciar

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Cinta apagada

  posicionActual = posEntrada;
  destino = -1;

  Serial.println("ESP32 listo. Comandos: e i s c r o");
}

void loop() {
  leerSerial();
  motor.run();

  if (rutinaActiva) {
    ejecutarRutinaPasoAPaso();
  }

  // Posiciones simples (entrada / salida)
  if (!rutinaActiva && motor.distanceToGo() == 0 && destino != -1) {
    posicionActual = destino;

    if (destino == posEntrada || destino == posSalida) {
      servoPuerta.write(90);  // Abrir puerta
    }

    Serial.println("OK");
    destino = -1;
    cancelado = false;
  }
}

void leerSerial() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if ((cmd == 'E' || cmd == 'e') && destino == -1 && !rutinaActiva) {
      destino = posEntrada;
      motor.moveTo(destino);
    }
    else if ((cmd == 'S' || cmd == 's') && destino == -1 && !rutinaActiva) {
      destino = posSalida;
      motor.moveTo(destino);
    }
    else if ((cmd == 'I' || cmd == 'i') && !rutinaActiva) {
      rutinaActiva = true;
      estadoRutina = 0;
    }
    else if (cmd == 'C' || cmd == 'c') {
      cancelado = true;
    }
    else if (cmd == 'R' || cmd == 'r') {
      digitalWrite(RELAY_PIN, HIGH);
    }
    else if (cmd == 'O' || cmd == 'o') {
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}

void ejecutarRutinaPasoAPaso() {
  if (cancelado) {
    motor.stop();
    motor.moveTo(posSalida);
    servoPuerta.write(90);  // Abre puerta
    rutinaActiva = false;
    destino = -1;
    Serial.println("OK");
    cancelado = false;
    return;
  }

  switch (estadoRutina) {
    case 0:
      servoPuerta.write(0);  // Cierra puerta
      delay(300);
      motor.moveTo(posInspeccion);
      estadoRutina = 1;
      break;

    case 1:
      if (motor.distanceToGo() == 0) {
        motor.moveTo(posSalida);
        estadoRutina = 2;
      }
      break;

    case 2:
      if (motor.distanceToGo() == 0) {
        servoPuerta.write(90);  // Abre puerta
        delay(300);
        rutinaActiva = false;
        posicionActual = posSalida;
        Serial.println("OK");
      }
      break;
  }
}
