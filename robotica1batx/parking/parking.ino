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

// Incluir la librería Servo
#include <Servo.h>

// Crear objetos Servo
Servo servoEntrada;
Servo servoSalida;

void setup()
{
  // Inicializar comunicación serial
  Serial.begin(9600);

  // Configurar pines de los botones
  pinMode(botonEntrada, INPUT_PULLUP);
  pinMode(botonSalida, INPUT_PULLUP);

  // Configurar pines del sensor de ultrasonidos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Configurar pines de LEDs
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // Adjuntar servomotores
  servoEntrada.attach(servoEntradaPin);
  servoSalida.attach(servoSalidaPin);

  // Posición inicial de las barreras (cerradas)
  servoEntrada.write(0);
  servoSalida.write(0);

  // Inicializar LED verde (parking disponible)
  actualizarLEDs();

  Serial.println("Sistema de parking inicializado");
  Serial.println("Capacidad máxima: 3 coches");
}

// Función para medir la distancia con el sensor de ultrasonidos
long medirDistancia()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  long distancia = duracion * 0.034 / 2;

  return distancia;
}

void loop()
{
  // Leer estado de los botones
  int estadoBotonEntrada = digitalRead(botonEntrada);
  int estadoBotonSalida = digitalRead(botonSalida);

  // Medir distancia con el sensor de ultrasonidos
  long distancia = medirDistancia();

  // Actualizar tiempo de último movimiento si se detecta un vehículo
  if (distancia < distanciaUmbral)
  {
    tiempoUltimoMovimiento = millis();
  }

  // Controlar barrera de entrada
  if (estadoBotonEntrada == LOW)
  { // Botón presionado (LOW por INPUT_PULLUP)
    controlarBarreraEntrada();
    delay(200); // Debounce
  }

  // Controlar barrera de salida
  if (estadoBotonSalida == LOW)
  { // Botón presionado (LOW por INPUT_PULLUP)
    controlarBarreraSalida();
    delay(200); // Debounce
  }

  // Verificar si hay que cerrar las barreras por falta de movimiento
  verificarCierreBarreras();

  // Actualizar LEDs según el estado
  actualizarLEDs();

  // Mostrar información en el monitor serial
  mostrarInformacion();

  // Pequeña pausa
  delay(100);
}

void controlarBarreraEntrada()
{
  // Solo abrimos si hay espacio en el parking
  if (cochesEnParking < capacidadMaxima)
  {
    // Abrir barrera
    servoEntrada.write(90);
    barreraEntradaAbierta = true;
    contadorAperturasEntrada++;
    cochesEnParking++;

    // Encender LED azul para indicar movimiento
    digitalWrite(ledAzul, HIGH);

    Serial.println("Barrera de entrada abierta");
  }
  else
  {
    Serial.println("Parking lleno, no se puede abrir la barrera de entrada");
  }
}

void controlarBarreraSalida()
{
  // Solo abrimos si hay coches en el parking
  if (cochesEnParking > 0)
  {
    // Abrir barrera
    servoSalida.write(90);
    barreraSalidaAbierta = true;
    contadorAperturasSalida++;
    cochesEnParking--;

    // Encender LED azul para indicar movimiento
    digitalWrite(ledAzul, HIGH);

    Serial.println("Barrera de salida abierta");
  }
  else
  {
    Serial.println("Parking vacío, no se puede abrir la barrera de salida");
  }
}

void verificarCierreBarreras()
{
  // Verificar cierre de barrera de entrada
  if (barreraEntradaAbierta && (millis() - tiempoUltimoMovimiento > tiempoEsperaCierre))
  {
    cerrarBarreraEntrada();
  }

  // Verificar cierre de barrera de salida
  if (barreraSalidaAbierta && (millis() - tiempoUltimoMovimiento > tiempoEsperaCierre))
  {
    cerrarBarreraSalida();
  }
}

void cerrarBarreraEntrada()
{
  servoEntrada.write(0);
  barreraEntradaAbierta = false;
  digitalWrite(ledAzul, LOW);
  Serial.println("Barrera de entrada cerrada por falta de movimiento");
}

void cerrarBarreraSalida()
{
  servoSalida.write(0);
  barreraSalidaAbierta = false;
  digitalWrite(ledAzul, LOW);
  Serial.println("Barrera de salida cerrada por falta de movimiento");
}

void actualizarLEDs()
{
  // Actualizar LEDs según el estado del parking
  if (cochesEnParking >= capacidadMaxima)
  {
    // Parking lleno
    digitalWrite(ledRojo, HIGH);
    digitalWrite(ledVerde, LOW);
  }
  else
  {
    // Hay espacios disponibles
    digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);
  }
}

void mostrarInformacion()
{
  // Mostrar información cada segundo
  static unsigned long ultimaActualizacion = 0;
  if (millis() - ultimaActualizacion >= 1000)
  {
    ultimaActualizacion = millis();

    Serial.print("Coches en el parking: ");
    Serial.print(cochesEnParking);
    Serial.print(" / ");
    Serial.println(capacidadMaxima);

    Serial.print("Plazas disponibles: ");
    Serial.println(capacidadMaxima - cochesEnParking);

    Serial.print("Número de aperturas entrada: ");
    Serial.println(contadorAperturasEntrada);

    Serial.print("Número de aperturas salida: ");
    Serial.println(contadorAperturasSalida);

    Serial.print("Distancia medida: ");
    Serial.print(medirDistancia());
    Serial.println(" cm");

    Serial.println("------------------------");
  }
}
