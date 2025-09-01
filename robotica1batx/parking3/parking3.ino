#include <ESP32Servo.h>

// Definición de pines
const int PIN_BOTON_ENTRADA = 2;  // Botón de entrada en IO2
const int PIN_BOTON_SALIDA = 4;   // Botón de salida en IO4
const int PIN_SERVO_ENTRADA = 14; // Servo de barrera de entrada en IO14
const int PIN_SERVO_SALIDA = 27;  // Servo de barrera de salida en IO27

// Configuración de parking
const int PLAZAS_TOTALES = 5;     // Número máximo de plazas
int plazasOcupadas = 0;           // Contador de plazas ocupadas

// Control de tiempo para cierre automático de barreras
unsigned long tiempoCierreEntrada = 0;
unsigned long tiempoCierreSalida = 0;
const int TIEMPO_ESPERA = 5000;   // 5000ms de espera antes de cerrar

// Estado de barreras
bool barreraEntradaAbierta = false;
bool barreraSalidaAbierta = false;

// Objetos servo
Servo servoEntrada;
Servo servoSalida;

void setup() {
  // Inicialización de comunicación serial
  Serial.begin(115200);
  
  // Configurar pines de botones como entrada con resistencia pull-up
  pinMode(PIN_BOTON_ENTRADA, INPUT_PULLUP);
  pinMode(PIN_BOTON_SALIDA, INPUT_PULLUP);
  
  // Configurar servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  servoEntrada.setPeriodHertz(50);
  servoSalida.setPeriodHertz(50);
  
  servoEntrada.attach(PIN_SERVO_ENTRADA);
  servoSalida.attach(PIN_SERVO_SALIDA);
  
  // Inicializar barreras cerradas
  servoEntrada.write(0);  // 0 grados = barrera cerrada
  servoSalida.write(0);
  
  Serial.println("Sistema de parking inicializado");
  Serial.println("Plazas disponibles: " + String(PLAZAS_TOTALES - plazasOcupadas));
}

void loop() {
  // Comprobar botón de entrada
  if (digitalRead(PIN_BOTON_ENTRADA) == LOW) { // Botón presionado (LOW con pull-up)
    if (plazasOcupadas < PLAZAS_TOTALES && !barreraEntradaAbierta) {
      // Abrir barrera de entrada
      servoEntrada.write(90);  // 90 grados = barrera abierta
      barreraEntradaAbierta = true;
      tiempoCierreEntrada = millis();
      plazasOcupadas++;
      Serial.println("Barrera entrada abierta. Plazas disponibles: " + String(PLAZAS_TOTALES - plazasOcupadas));
      delay(200); // Pequeño debounce para el botón
    } else if (plazasOcupadas >= PLAZAS_TOTALES) {
      Serial.println("Parking completo, no se puede entrar");
    }
  }
  
  // Comprobar botón de salida
  if (digitalRead(PIN_BOTON_SALIDA) == LOW) { // Botón presionado (LOW con pull-up)
    if (plazasOcupadas > 0 && !barreraSalidaAbierta) {
      // Abrir barrera de salida
      servoSalida.write(90);  // 90 grados = barrera abierta
      barreraSalidaAbierta = true;
      tiempoCierreSalida = millis();
      plazasOcupadas--;
      Serial.println("Barrera salida abierta. Plazas disponibles: " + String(PLAZAS_TOTALES - plazasOcupadas));
      delay(200); // Pequeño debounce para el botón
    } else if (plazasOcupadas <= 0) {
      Serial.println("No hay vehículos en el parking");
    }
  }
  
  // Comprobar si es hora de cerrar la barrera de entrada
  if (barreraEntradaAbierta && (millis() - tiempoCierreEntrada >= TIEMPO_ESPERA)) {
    servoEntrada.write(0);  // Cerrar barrera
    barreraEntradaAbierta = false;
    Serial.println("Barrera de entrada cerrada automáticamente");
  }
  
  // Comprobar si es hora de cerrar la barrera de salida
  if (barreraSalidaAbierta && (millis() - tiempoCierreSalida >= TIEMPO_ESPERA)) {
    servoSalida.write(0);  // Cerrar barrera
    barreraSalidaAbierta = false;
    Serial.println("Barrera de salida cerrada automáticamente");
  }
  
  // Pequeña pausa para estabilidad
  delay(50);
}
