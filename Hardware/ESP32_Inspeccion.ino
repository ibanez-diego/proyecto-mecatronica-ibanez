#include <AccelStepper.h>

// ---------------------------- Pines ----------------------------
int IN1 = 18;
int IN2 = 19;
int PWM1 = 4;

#define STEP_PIN 26
#define DIR_PIN 27
#define relePin 2

AccelStepper pasoMotor(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// ---------------------------- Variables ----------------------------
bool rutinaActiva = false;
bool pasoAdelante = false;
bool pasoAtras = false;
bool rutinaCancelada = false;
bool rutinaFinalizada = true;

unsigned long tiempoInicioN20 = 0;

int pwmN20 = 900;
long pasosN20 = 25500;
long tolerancia = 50;

volatile long contador = 0;

int velocidadPasoMotor = 400;
long pasosPasoMotor = 13200;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);

  pasoMotor.setMaxSpeed(velocidadPasoMotor);
  pasoMotor.setAcceleration(200);

  attachInterrupt(digitalPinToInterrupt(16), encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(17), encoder, CHANGE);

  inicializarVariables();

  Serial.println("ESP32: Sistema listo.");
}

void loop() {
  leerSerial();

  if (rutinaActiva && !rutinaCancelada) {
    if (!pasoAdelante && !pasoAtras) {
      moverMotorN20(true, pwmN20);
      if (abs(contador) >= pasosN20) {
        detenerMotorN20();
        moverPasoMotorIda();
        pasoAdelante = true;
      }
    }

    if (pasoAdelante && pasoMotor.distanceToGo() != 0) {
      pasoMotor.run();
    } else if (pasoAdelante && pasoMotor.distanceToGo() == 0) {
      moverPasoMotorVuelta();
      pasoAdelante = false;
      pasoAtras = true;
    } else if (pasoAtras && pasoMotor.distanceToGo() != 0) {
      pasoMotor.run();
    } else if (pasoAtras && pasoMotor.distanceToGo() == 0) {
      moverMotorN20(false, pwmN20);
      rutinaActiva = false;
      rutinaFinalizada = false;
    }
  }

  if (!rutinaActiva && !rutinaCancelada && !rutinaFinalizada && abs(contador) <= tolerancia) {
    detenerMotorN20();
    digitalWrite(relePin, HIGH);
    Serial.println("OK");
    inicializarVariables();
  }

  if (rutinaCancelada) {
    if (pasoMotor.distanceToGo() != 0) {
      pasoMotor.run();
    } else if (abs(contador) <= tolerancia) {
      detenerMotorN20();
      digitalWrite(relePin, HIGH);
      Serial.println("OK");
      inicializarVariables();
    }
  }
}

// ---------------------------- Funciones Serial ----------------------------
void leerSerial() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '\n' || cmd == '\r') return;

    if (cmd == 'S' || cmd == 's') {
      if (!rutinaActiva && !rutinaCancelada && rutinaFinalizada) {
        digitalWrite(relePin, LOW);
        rutinaActiva = true;
        rutinaFinalizada = false;
        pasoAdelante = false;
        pasoAtras = false;
        contador = 0;
      }
    } else if (cmd == 'R' || cmd == 'r') {
      digitalWrite(relePin, HIGH);
    } else if (cmd == 'O' || cmd == 'o') {
      digitalWrite(relePin, LOW);
    } else if (cmd == 'C' || cmd == 'c') {
      if (rutinaActiva && !rutinaCancelada) {
        detenerMotorN20();
        pasoMotor.moveTo(0);
        moverMotorN20(false, pwmN20);
        rutinaCancelada = true;
        rutinaActiva = false;
      }
    }
  }
}

// ---------------------------- Motor N20 ----------------------------
void moverMotorN20(bool adelante, int pwm) {
  digitalWrite(IN1, !adelante);
  digitalWrite(IN2, adelante);
  analogWrite(PWM1, pwm);
}

void detenerMotorN20() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(PWM1, 0);
}

// ---------------------------- Motor Paso a Paso ----------------------------
void moverPasoMotorIda() {
  pasoMotor.moveTo(pasosPasoMotor);
}

void moverPasoMotorVuelta() {
  pasoMotor.moveTo(0);
}

// ---------------------------- Encoder ----------------------------
volatile byte ant = 0;
volatile byte act = 0;

void encoder() {
  ant = act;
  act = (digitalRead(16) << 1) | digitalRead(17);

  if (ant == 0 && act == 1) contador++;
  else if (ant == 1 && act == 3) contador++;
  else if (ant == 3 && act == 2) contador++;
  else if (ant == 2 && act == 0) contador++;
  else contador--;
}

// ---------------------------- Inicialización ----------------------------
void inicializarVariables() {
  rutinaActiva = false;
  pasoAdelante = false;
  pasoAtras = false;
  rutinaCancelada = false;
  rutinaFinalizada = true;
  contador = 0;
}
