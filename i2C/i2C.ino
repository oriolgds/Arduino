int valor = 0; // Variable donde almacenaremos el valor del potenciometro

void setup() {
  Serial.begin(9600);  // Inicializamos la comunicación serial
}

void loop() {
  valor = analogRead(A0); // Leemos del pin A0 valor

  // Imprimimos el valor por el monitor serie (0 - 1023)
  Serial.print("Valor analogico : ");
  Serial.println(valor);
  delay(1000);
}
