// Definición de los pines para las luces del semáforo
int redLight = 13;    // Pin para la luz roja
int yellowLight = 12; // Pin para la luz amarilla
int greenLight = 11;  // Pin para la luz verde

// Pines para el semáforo de peatones
int pedRedLight = 10;  // Pin para la luz roja de peatones
int pedGreenLight = 9; // Pin para la luz verde de peatones

// Tiempos para cada color (en milisegundos)
const unsigned long RED_TIME = 5000;        // 5 segundos en rojo
const unsigned long RED_YELLOW_TIME = 2000; // 2 segundos en rojo+amarillo
const unsigned long GREEN_TIME = 4000;      // 4 segundos en verde
const unsigned long YELLOW_TIME = 1500;     // 1.5 segundos en amarillo
const unsigned long PED_BLINK_TIME = 250;   // 0.25 segundos para el parpadeo

// Pin para el sensor de luz (fotorresistencia)
int lightSensorPin = A0;  // Conectar la fotorresistencia al pin analógico A0
int lightThreshold = 400; // Umbral para determinar si es de día o noche

void setup()
{
    // Configuración de pines como salidas para los LEDs
    pinMode(redLight, OUTPUT);
    pinMode(yellowLight, OUTPUT);
    pinMode(greenLight, OUTPUT);

    // Configuración de pines para el semáforo de peatones
    pinMode(pedRedLight, OUTPUT);
    pinMode(pedGreenLight, OUTPUT);

    // Iniciamos comunicación serial para depuración
    Serial.begin(9600);
    Serial.println("Inicializando semáforo...");
}

void loop()
{
    // Leer el valor del sensor de luz
    int lightLevel = analogRead(lightSensorPin);

    // Imprimir el nivel de luz para depuración
    Serial.print("Nivel de luz: ");
    Serial.println(lightLevel);

    // Comprobar si es de noche (valor bajo en el sensor significa menos luz)
    if (lightLevel < lightThreshold)
    {
        // Modo nocturno: solo parpadea la luz amarilla
        Serial.println("Modo nocturno activado");
        nightMode();
    }
    else
    {
        // Modo diurno: funcionamiento normal del semáforo
        Serial.println("Modo diurno activado");
        dayMode();
    }
}

// Función para el modo diurno (funcionamiento normal)
void dayMode()
{
    // Fase 1: Luz roja para coches, verde para peatones
    // Los coches se detienen, los peatones pueden cruzar
    Serial.println("COCHES: Rojo | PEATONES: Verde");
    digitalWrite(redLight, HIGH);
    digitalWrite(yellowLight, LOW);
    digitalWrite(greenLight, LOW);
    digitalWrite(pedRedLight, LOW);
    digitalWrite(pedGreenLight, HIGH);
    delay(RED_TIME); // Tiempo para que los peatones crucen

    // Parpadeo final verde de peatones para avisar que va a cambiar
    Serial.println("PEATONES: Verde parpadeante");
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(pedGreenLight, LOW);
        delay(PED_BLINK_TIME);
        digitalWrite(pedGreenLight, HIGH);
        delay(PED_BLINK_TIME);
    }

    // Los peatones ya no pueden cruzar - IMPORTANTE: asegurar que el verde de peatones está apagado
    digitalWrite(pedRedLight, HIGH);
    digitalWrite(pedGreenLight, LOW);

    // Fase 2: Luz roja+amarilla para coches (estilo español), peatones en rojo
    Serial.println("COCHES: Rojo+Amarillo | PEATONES: Rojo");
    digitalWrite(redLight, HIGH);
    digitalWrite(yellowLight, HIGH);
    digitalWrite(greenLight, LOW);
    digitalWrite(pedRedLight, HIGH);
    digitalWrite(pedGreenLight, LOW);
    delay(RED_YELLOW_TIME);

    // Fase 3: Luz verde para coches, roja para peatones
    // Los coches pueden pasar, los peatones se detienen
    Serial.println("COCHES: Verde | PEATONES: Rojo");
    digitalWrite(redLight, LOW);
    digitalWrite(yellowLight, LOW);
    digitalWrite(greenLight, HIGH);
    digitalWrite(pedRedLight, HIGH);
    digitalWrite(pedGreenLight, LOW);
    delay(GREEN_TIME);

    // Fase 4: Luz amarilla - preparándose para detenerse
    Serial.println("COCHES: Amarillo | PEATONES: Rojo");
    digitalWrite(redLight, LOW);
    digitalWrite(yellowLight, HIGH);
    digitalWrite(greenLight, LOW);
    digitalWrite(pedRedLight, HIGH);
    digitalWrite(pedGreenLight, LOW);
    delay(YELLOW_TIME);
}

// Función para el modo nocturno (solo luz amarilla parpadeante)
void nightMode()
{
    // Apagar luces roja y verde de coches
    digitalWrite(redLight, LOW);
    digitalWrite(greenLight, LOW);

    // Apagar completamente el semáforo de peatones
    digitalWrite(pedRedLight, LOW);
    digitalWrite(pedGreenLight, LOW);

    // Hacer parpadear la luz amarilla
    Serial.println("NOCTURNO: Amarillo parpadeante");
    digitalWrite(yellowLight, HIGH);
    delay(500); // Encendido por 0.5 segundos
    digitalWrite(yellowLight, LOW);
    delay(500); // Apagado por 0.5 segundos
}