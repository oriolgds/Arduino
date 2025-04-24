#include <ESP32Servo.h>

// Configuración de pines (mantenidos del código original)
const int botonEntrada = 34;
const int botonSalida = 35;
const int trigPin = 4;
const int echoPin = 2;
const int servoEntradaPin = 25;
const int servoSalidaPin = 17;

// Configuración del parking
const int CAPACIDAD_MAXIMA = 3;
const int DISTANCIA_DETECCION = 15; // cm
const int ANGULO_ABIERTO = 90;
const int ANGULO_CERRADO = 0;

// Variables de estado
int cochesEnParking = 0;
int contadorEntradas = 0;
int contadorSalidas = 0;

// Objetos Servo
Servo barreraEntrada;
Servo barreraSalida;

void setup() {
  Serial.begin(9600);
  
  // Configurar pines
  pinMode(botonEntrada, INPUT_PULLUP);
  pinMode(botonSalida, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Inicializar servos
  barreraEntrada.attach(servoEntradaPin);
  barreraSalida.attach(servoSalidaPin);
  barreraEntrada.write(ANGULO_CERRADO);
  barreraSalida.write(ANGULO_CERRADO);
  
  Serial.println("Sistema inicializado");
}

int medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duracion = pulseIn(echoPin, HIGH);
  return duracion * 0.034 / 2;
}

void gestionarEntrada() {
  if (digitalRead(botonEntrada) == LOW && cochesEnParking < CAPACIDAD_MAXIMA) {
    barreraEntrada.write(ANGULO_ABIERTO);
    cochesEnParking++;
    contadorEntradas++;
    Serial.println("Barrera entrada abierta. Total: " + String(cochesEnParking));
  }
  
  // Si la barrera está abierta y no hay coche, cerrar
  if (barreraEntrada.read() == ANGULO_ABIERTO) {
    if (medirDistancia() > DISTANCIA_DETECCION) {
      barreraEntrada.write(ANGULO_CERRADO);
      Serial.println("Barrera entrada cerrada automáticamente");
    }
  }
}

void gestionarSalida() {
  if (digitalRead(botonSalida) == LOW && cochesEnParking > 0) {
    barreraSalida.write(ANGULO_ABIERTO);
    cochesEnParking--;
    contadorSalidas++;
    Serial.println("Barrera salida abierta. Total: " + String(cochesEnParking));
  }
  
  // Si la barrera está abierta y no hay coche, cerrar
  if (barreraSalida.read() == ANGULO_ABIERTO) {
    if (medirDistancia() > DISTANCIA_DETECCION) {
      barreraSalida.write(ANGULO_CERRADO);
      Serial.println("Barrera salida cerrada automáticamente");
    }
  }
}

void mostrarEstado() {
  static unsigned long ultimoInforme = 0;
  if (millis() - ultimoInforme >= 2000) {
    ultimoInforme = millis();
    Serial.println("\n--- Estado del Parking ---");
    Serial.println("Coches dentro: " + String(cochesEnParking) + "/" + String(CAPACIDAD_MAXIMA));
    Serial.println("Total entradas: " + String(contadorEntradas));
    Serial.println("Total salidas: " + String(contadorSalidas));
    Serial.println("Plazas libres: " + String(CAPACIDAD_MAXIMA - cochesEnParking));
    Serial.println("----------------------");
  }
}

void loop() {
  gestionarEntrada();
  gestionarSalida();
  mostrarEstado();
  delay(100);
}
