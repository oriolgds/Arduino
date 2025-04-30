#include <ESP32Servo.h>

// Definición de pines
const int PIN_BOTON_ENTRADA = 34;
const int PIN_BOTON_SALIDA = 35;
const int PIN_SERVO_ENTRADA = 25;
const int PIN_SERVO_SALIDA = 17;
// Pines del sensor de ultrasonidos (no utilizados en esta versión)
// const int PIN_ULTRASONIDOS_TRIG = 4;
// const int PIN_ULTRASONIDOS_ECHO = 2;

// Configuración del parking
const int CAPACIDAD_MAXIMA = 10;
const int ANGULO_ABIERTO = 90;   // ángulo para barrera abierta
const int ANGULO_CERRADO = 0;    // ángulo para barrera cerrada
const unsigned long TIEMPO_INFORME = 5000; // tiempo entre informes de estado (5 segundos)
const unsigned long TIEMPO_CIERRE_BARRERA = 4000; // 4 segundos para cierre automático

// Variables de estado
int cochesEnParking = 0;
int entradaTotal = 0;
int salidaTotal = 0;
bool barreraEntradaAbierta = false;
bool barreraSalidaAbierta = false;
unsigned long ultimoInforme = 0;
unsigned long tiempoAperturaEntrada = 0;
unsigned long tiempoAperturaSalida = 0;

// Objetos
Servo servoEntrada;
Servo servoSalida;

void setup() {
  // Inicializar comunicación serial a 9600 baudios
  Serial.begin(9600);
  Serial.println("Iniciando sistema de parking...");
  
  // Configurar pines de entrada
  pinMode(PIN_BOTON_ENTRADA, INPUT_PULLUP);
  pinMode(PIN_BOTON_SALIDA, INPUT_PULLUP);
  // No configuramos los pines del ultrasonidos en esta versión
  
  // Inicializar servomotores
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  servoEntrada.setPeriodHertz(50);    // Frecuencia estándar para servos
  servoSalida.setPeriodHertz(50);
  servoEntrada.attach(PIN_SERVO_ENTRADA, 500, 2400);
  servoSalida.attach(PIN_SERVO_SALIDA, 500, 2400);
  
  // Posición inicial de las barreras (cerradas)
  servoEntrada.write(ANGULO_CERRADO);
  servoSalida.write(ANGULO_CERRADO);
  
  Serial.println("Sistema iniciado. Parking vacío.");
  Serial.print("Capacidad máxima: ");
  Serial.println(CAPACIDAD_MAXIMA);
}

void loop() {
  // Gestionar entrada y salida de vehículos
  gestionarEntrada();
  gestionarSalida();
  
  // Control de cierre automático
  gestionarCierreAutomatico();
  
  // Mostrar estado periódicamente
  if (millis() - ultimoInforme >= TIEMPO_INFORME) {
    mostrarEstado();
    ultimoInforme = millis();
  }
}

void gestionarCierreAutomatico() {
  // Comprobar si hay que cerrar la barrera de entrada
  if (barreraEntradaAbierta && (millis() - tiempoAperturaEntrada >= TIEMPO_CIERRE_BARRERA)) {
    servoEntrada.write(ANGULO_CERRADO);
    barreraEntradaAbierta = false;
    cochesEnParking++;
    entradaTotal++;
    Serial.println("Barrera de entrada cerrada automáticamente");
    Serial.print("Vehículo entró. Total en parking: ");
    Serial.println(cochesEnParking);
  }
  
  // Comprobar si hay que cerrar la barrera de salida
  if (barreraSalidaAbierta && (millis() - tiempoAperturaSalida >= TIEMPO_CIERRE_BARRERA)) {
    servoSalida.write(ANGULO_CERRADO);
    barreraSalidaAbierta = false;
    cochesEnParking--;
    salidaTotal++;
    Serial.println("Barrera de salida cerrada automáticamente");
    Serial.print("Vehículo salió. Total en parking: ");
    Serial.println(cochesEnParking);
  }
}

void gestionarEntrada() {
  // Leer estado del botón (activo en LOW porque usamos PULLUP)
  bool botonPresionado = (digitalRead(PIN_BOTON_ENTRADA) == LOW);
  
  // Si el botón está presionado y hay espacio en el parking, abrir barrera
  if (botonPresionado && !barreraEntradaAbierta && cochesEnParking < CAPACIDAD_MAXIMA) {
    servoEntrada.write(ANGULO_ABIERTO);
    barreraEntradaAbierta = true;
    tiempoAperturaEntrada = millis(); // Guardar tiempo de apertura
    Serial.println("Barrera de entrada abierta");
  }
}

void gestionarSalida() {
  // Leer estado del botón (activo en LOW porque usamos PULLUP)
  bool botonPresionado = (digitalRead(PIN_BOTON_SALIDA) == LOW);
  
  // Si el botón está presionado y hay coches en el parking, abrir barrera
  if (botonPresionado && !barreraSalidaAbierta && cochesEnParking > 0) {
    servoSalida.write(ANGULO_ABIERTO);
    barreraSalidaAbierta = true;
    tiempoAperturaSalida = millis(); // Guardar tiempo de apertura
    Serial.println("Barrera de salida abierta");
  }
}

void mostrarEstado() {
  Serial.println("\n--- Estado del Parking ---");
  Serial.print("Vehículos en parking: ");
  Serial.print(cochesEnParking);
  Serial.print("/");
  Serial.println(CAPACIDAD_MAXIMA);
  Serial.print("Plazas disponibles: ");
  Serial.println(CAPACIDAD_MAXIMA - cochesEnParking);
  Serial.print("Total entradas: ");
  Serial.println(entradaTotal);
  Serial.print("Total salidas: ");
  Serial.println(salidaTotal);
  Serial.println("------------------------\n");
}
