const int pinBuzzer = 8;

void setup() 
{
}

void loop() 
{
  //generar tono de 440Hz durante 1000 ms
  tone(pinBuzzer, 440);

  //detener tono durante 500ms  
  noTone(pinBuzzer);

  //generar tono de 523Hz durante 500ms, y detenerlo durante 500ms.
  tone(pinBuzzer, 523, 300);
}
