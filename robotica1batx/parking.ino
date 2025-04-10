// Sistema de Parking con Arduino
// Control de barreras mediante botones y sensor de ultrasonidos
// Conteo de coches y control de capacidad

// Pines para los botones
const int botonEntrada = 9;
const int botonSalida = 7;

// Pines para el sensor de ultrasonidos
const int trigPin = 2; // Pin de trigger
const int echoPin = 3; // Pin de echo

// Pines para los servomotores
const int servoEntradaPin = A0;
const int servoSalidaPin = A1;

// Pines para LEDs indicadores
const int ledRojo = 11;  // LED rojo para parking lleno
const int ledVerde = 12; // LED verde para parking disponible
const int ledAzul = 13;  // LED azul para indicar movimiento de barrera

// Variables para contar coches y controlar el estado del parking
int contadorAperturasEntrada = 0;
int contadorAperturasSalida = 0;
int cochesEnParking = 0;
const int capacidadMaxima = 3;
bool barreraEntradaAbierta = false;
bool barreraSalidaAbierta = false;

// Variables para el control del sensor de ultrasonidos
unsigned long tiempoUltimoMovimiento = 0;
const unsigned long tiempoEsperaCierre = 3000; // 3 segundos para cerrar la barrera
const int distanciaUmbral = 20;                // Distancia en cm para detectar un vehículo

// Incluir la librería ESP32Servo
#include <ESP32Servo.h>

// Crear objetos Servo
Servo servoEntrada;
Servo servoSalida;