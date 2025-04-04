// Definición de los pines para las luces del semáforo
int redLight = 13;    // Pin para la luz roja
int yellowLight = 12; // Pin para la luz amarilla
int greenLight = 11;  // Pin para la luz verde

// Pines para el semáforo de peatones
int pedRedLight = 10;  // Pin para la luz roja de peatones
int pedGreenLight = 9; // Pin para la luz verde de peatones

// Tiempos para cada color (en milisegundos)
const unsigned long RED_TIME = 5000;      // 5 segundos en rojo
const unsigned long GREEN_TIME = 4000;    // 4 segundos en verde
const unsigned long YELLOW_TIME = 1500;   // 1.5 segundos en amarillo
const unsigned long PED_BLINK_TIME = 250; // 0.25 segundos para el parpadeo

// Pin para el sensor de luz (fotorresistencia)
int lightSensorPin = A0;  // Conectar la fotorresistencia al pin analógico A0
int lightThreshold = 800; // Umbral para determinar si es de día o noche

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

// Función para verificar el modo día/noche
bool isNightMode()
{
    int lightLevel = analogRead(lightSensorPin);
    Serial.print("Nivel de luz: ");
    Serial.println(lightLevel);
    return lightLevel < lightThreshold;
}

void loop()
{
    if (isNightMode())
    {
        Serial.println("Modo nocturno activado");
        nightMode();
    }
    else
    {
        Serial.println("Modo diurno activado");
        dayMode();
    }
}

// Función para el modo diurno (funcionamiento normal)
void dayMode()
{
    // Fase 1: Luz roja para coches, verde para peatones
    Serial.println("COCHES: Rojo | PEATONES: Verde");
    digitalWrite(redLight, HIGH);
    digitalWrite(yellowLight, LOW);
    digitalWrite(greenLight, LOW);
    digitalWrite(pedRedLight, LOW);
    digitalWrite(pedGreenLight, HIGH);

    // Comprobación durante la fase roja
    unsigned long startTime = millis();
    while (millis() - startTime < RED_TIME)
    {
        if (isNightMode())
            return; // Salir si cambia a modo nocturno
        delay(100); // Pequeña pausa para no saturar el procesador
    }

    // Parpadeo final verde de peatones
    Serial.println("PEATONES: Verde parpadeante");
    for (int i = 0; i < 5; i++)
    {
        if (isNightMode())
            return; // Comprobar durante el parpadeo
        digitalWrite(pedGreenLight, LOW);
        delay(PED_BLINK_TIME);
        digitalWrite(pedGreenLight, HIGH);
        delay(PED_BLINK_TIME);
    }

    digitalWrite(pedRedLight, HIGH);
    digitalWrite(pedGreenLight, LOW);

    // Fase 2: Luz verde para coches
    Serial.println("COCHES: Verde | PEATONES: Rojo");
    digitalWrite(redLight, LOW);
    digitalWrite(yellowLight, LOW);
    digitalWrite(greenLight, HIGH);
    digitalWrite(pedRedLight, HIGH);
    digitalWrite(pedGreenLight, LOW);

    // Comprobación durante la fase verde
    startTime = millis();
    while (millis() - startTime < GREEN_TIME)
    {
        if (isNightMode())
            return;
        delay(100);
    }

    // Fase 3: Luz amarilla
    Serial.println("COCHES: Amarillo | PEATONES: Rojo");
    digitalWrite(redLight, LOW);
    digitalWrite(yellowLight, HIGH);
    digitalWrite(greenLight, LOW);
    digitalWrite(pedRedLight, HIGH);
    digitalWrite(pedGreenLight, LOW);

    // Comprobación durante la fase amarilla
    startTime = millis();
    while (millis() - startTime < YELLOW_TIME)
    {
        if (isNightMode())
            return;
        delay(100);
    }
}

// Función para el modo nocturno
void nightMode()
{
    digitalWrite(redLight, LOW);
    digitalWrite(greenLight, LOW);
    digitalWrite(pedRedLight, LOW);
    digitalWrite(pedGreenLight, LOW);

    // Comprobación durante el parpadeo nocturno
    unsigned long startTime = millis();
    while (millis() - startTime < 1000)
    { // Comprobar cada segundo
        if (!isNightMode())
            return; // Salir si cambia a modo diurno

        Serial.println("NOCTURNO: Amarillo parpadeante");
        digitalWrite(yellowLight, HIGH);
        delay(500);
        digitalWrite(yellowLight, LOW);
        delay(500);
    }
}