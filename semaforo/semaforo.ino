// Definición de los pines para las luces del semáforo
int redLight = 11;    // Pin para la luz roja
int yellowLight = 12; // Pin para la luz amarilla
int greenLight = 13;  // Pin para la luz verde

// Pin para el sensor de luz (fotorresistencia)
int lightSensorPin = A0;  // Conectar la fotorresistencia al pin analógico A0
int lightThreshold = 400; // Umbral para determinar si es de día o noche

void setup()
{
    // Configuración de pines como salidas para los LEDs
    pinMode(redLight, OUTPUT);
    pinMode(yellowLight, OUTPUT);
    pinMode(greenLight, OUTPUT);

    // Iniciamos comunicación serial para depuración (opcional)
    Serial.begin(9600);
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
        nightMode();
    }
    else
    {
        // Modo diurno: funcionamiento normal del semáforo
        dayMode();
    }
}

// Función para el modo diurno (funcionamiento normal)
void dayMode()
{
    // Luz roja encendida
    digitalWrite(redLight, HIGH);
    digitalWrite(yellowLight, LOW);
    digitalWrite(greenLight, LOW);
    delay(5000); // Esperar 5 segundos

    // Luz amarilla encendida
    digitalWrite(redLight, LOW);
    digitalWrite(yellowLight, HIGH);
    digitalWrite(greenLight, LOW);
    delay(1500); // Esperar 2 segundos
    // Luz verde encendida
    digitalWrite(redLight, LOW);
    digitalWrite(yellowLight, LOW);
    digitalWrite(greenLight, HIGH);
    delay(4000); // Esperar 5 segundos

    // No es necesario volver a encender la luz roja aquí
    // ya que el ciclo loop() volverá a ejecutar dayMode()
    // y comenzará con la luz roja encendida
}

// Función para el modo nocturno (solo luz amarilla parpadeante)
void nightMode()
{
    // Apagar luces roja y verde
    digitalWrite(redLight, LOW);
    digitalWrite(greenLight, LOW);

    // Hacer parpadear la luz amarilla
    digitalWrite(yellowLight, HIGH);
    delay(500); // Encendido por 0.5 segundos
    digitalWrite(yellowLight, LOW);
    delay(500); // Apagado por 0.5 segundos
}