void setup(){

    for(int i = 1; i < 11; i = i + 1){

        pinMode(i, OUTPUT);

    }

}

void loop(){
    digitalWrite(3, HIGH);

    for(int j = 1; j < 11; j = j + 1){ //Vamos encendiendo todos los segmentos de uno a uno

        digitalWrite(3, HIGH);

        delay(400);

    }

    for(int j = 1; j < 11; j = j + 1){ //Vamos apagando todos los segmentos de uno a uno

        digitalWrite(j, LOW);

        delay(400);

    }
    

}
