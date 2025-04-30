#include <ESP32Servo.h>

// Definición de pines
const int BOTON_ENTRADA = 19; // Pin para el botón de entrada
const int BOTON_SALIDA = 23;  // Pin para el botón de salida
const int SERVO_ENTRADA = 17;
const int SERVO_SALIDA = 25;

// Configuración del parking
const int CAPACIDAD_MAXIMA = 3;
int plazasOcupadas = 0;

// Ángulos para las barreras
const int ANGULO_ABIERTO = 90;
const int ANGULO_CERRADO = 0;

// Tiempo que la barrera permanecerá abierta (en milisegundos)
const int TIEMPO_BARRERA_ABIERTA = 3000;

// Objetos para los servos
Servo servoEntrada;
Servo servoSalida;

// Variables para control de estado
bool barrEnt_abierta = false;
bool barrSal_abierta = false;
unsigned long tiempoInicioAperturaEntrada = 0;
unsigned long tiempoInicioAperturaSalida = 0;

// Variables para detección de estado del botón
int lastStateBotonEntrada = HIGH; // estado anterior del botón de entrada
int currentStateBotonEntrada;     // estado actual del botón de entrada
int lastStateBotonSalida = HIGH;  // estado anterior del botón de salida
int currentStateBotonSalida;      // estado actual del botón de salida

void setup()
{
  // Inicializar comunicación serial
  Serial.begin(9600);

  // Configurar pines de entrada - Nota: GPIO34 y GPIO35 son solo de entrada y no tienen pullup interno
  pinMode(BOTON_ENTRADA, INPUT);
  pinMode(BOTON_SALIDA, INPUT);

  // Pausa para estabilización
  delay(1000);

  // Inicializar servos con la frecuencia adecuada para ESP32
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  servoEntrada.setPeriodHertz(50); // Frecuencia estándar para servos
  servoSalida.setPeriodHertz(50);  // Frecuencia estándar para servos

  // Adjuntar servos a los pines
  servoEntrada.attach(SERVO_ENTRADA, 500, 2400); // Min y max pulso en microsegundos
  servoSalida.attach(SERVO_SALIDA, 500, 2400);   // Min y max pulso en microsegundos

  // Verificación visual de que los servos funcionan
  Serial.println("Comprobando servomotores...");

  // Test servo entrada
  servoEntrada.write(ANGULO_ABIERTO);
  delay(1000);
  servoEntrada.write(ANGULO_CERRADO);
  delay(1000);

  // Test servo salida
  servoSalida.write(ANGULO_ABIERTO);
  delay(1000);
  servoSalida.write(ANGULO_CERRADO);

  Serial.println("Sistema de parking inicializado");
  Serial.println("Capacidad máxima: " + String(CAPACIDAD_MAXIMA) + " plazas");
  Serial.println("Plazas ocupadas: " + String(plazasOcupadas));

  // Instrucciones de conexión
  Serial.println("\nIMPORTANTE: Conectar botones con resistencias pull-down externas");
  Serial.println("Conectar: BOTON -> 3.3V y PIN -> GND con resistencia de 10k");
  Serial.println("BOTON_ENTRADA: GPIO34");
  Serial.println("BOTON_SALIDA: GPIO35");
}

void loop()
{
  // Leer estado actual de los botones
  currentStateBotonEntrada = digitalRead(BOTON_ENTRADA);
  currentStateBotonSalida = digitalRead(BOTON_SALIDA);

  // Imprimir valores de los botones cada 2 segundos para debug
  static unsigned long ultimoTiempoPrint = 0;
  if (millis() - ultimoTiempoPrint > 2000)
  {
    Serial.print("Estado botón entrada: ");
    Serial.print(currentStateBotonEntrada);
    Serial.print(" | Estado botón salida: ");
    Serial.println(currentStateBotonSalida);
    ultimoTiempoPrint = millis();
  }

  // Comprobar botón de entrada
  if (currentStateBotonEntrada == LOW && lastStateBotonEntrada == HIGH)
  {
    procesarEntrada();
  }

  // Comprobar botón de salida
  if (currentStateBotonSalida == LOW && lastStateBotonSalida == HIGH)
  {
    procesarSalida();
  }

  // Guardar estados actuales como anteriores
  lastStateBotonEntrada = currentStateBotonEntrada;
  lastStateBotonSalida = currentStateBotonSalida;

  // Cerrar barrera de entrada si ha pasado el tiempo
  if (barrEnt_abierta && millis() - tiempoInicioAperturaEntrada > TIEMPO_BARRERA_ABIERTA)
  {
    servoEntrada.write(ANGULO_CERRADO);
    barrEnt_abierta = false;
    Serial.println("Barrera de entrada cerrada automáticamente");
  }

  // Cerrar barrera de salida si ha pasado el tiempo
  if (barrSal_abierta && millis() - tiempoInicioAperturaSalida > TIEMPO_BARRERA_ABIERTA)
  {
    servoSalida.write(ANGULO_CERRADO);
    barrSal_abierta = false;
    Serial.println("Barrera de salida cerrada automáticamente");
  }
}

// El resto de las funciones siguen igual
void procesarEntrada()
{
  if (plazasOcupadas < CAPACIDAD_MAXIMA)
  {
    // Hay plazas disponibles, abrir barrera
    servoEntrada.write(ANGULO_ABIERTO);
    barrEnt_abierta = true;
    tiempoInicioAperturaEntrada = millis();

    plazasOcupadas++;

    Serial.println("Coche entrando. Barrera de entrada abierta");
    Serial.println("Plazas ocupadas: " + String(plazasOcupadas) + " de " + String(CAPACIDAD_MAXIMA));
  }
  else
  {
    // Parking lleno
    Serial.println("PARKING COMPLETO - No se puede entrar");
  }
}

void procesarSalida()
{
  if (plazasOcupadas > 0)
  {
    // Hay coches en el parking, abrir barrera
    servoSalida.write(ANGULO_ABIERTO);
    barrSal_abierta = true;
    tiempoInicioAperturaSalida = millis();

    plazasOcupadas--;

    Serial.println("Coche saliendo. Barrera de salida abierta");
    Serial.println("Plazas ocupadas: " + String(plazasOcupadas) + " de " + String(CAPACIDAD_MAXIMA));
  }
  else
  {
    // No hay coches en el parking
    Serial.println("ERROR - No hay coches en el parking para salir");
  }
}
