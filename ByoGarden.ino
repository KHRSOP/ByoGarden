// Amarelo; Azul; Verde; Vermelho; Preto

//controle de LED RGB Endereçavel
#include <Adafruit_NeoPixel.h>
#define SET_ALL 255

const byte UMIDADE_PIN = A4;
const byte MOTOR_PIN = 3;

const byte LEDWS_PIN = 8;
const byte LEDR_PIN = 9;
const byte LEDG_PIN = 10;
const byte LEDB_PIN = 11;
const byte LEDLOAD_PIN = A5;

const byte WS_PIN = LEDWS_PIN;
const byte WS_NUMPIXELS = 4;

int umidade_media = 10;
//const int umidade_media = 1800;

int umidade_soma = 0;
int umidade_i = 0; 

const int MOTOR_TEMPO = 10;
int ativa_motor = 5; // tempo inicial ajuste

byte led_low = 0;
byte estagio_umidade = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(WS_NUMPIXELS, WS_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(UMIDADE_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  pinMode(LEDWS_PIN, OUTPUT);
  pinMode(LEDR_PIN, OUTPUT);
  pinMode(LEDG_PIN, OUTPUT);
  pinMode(LEDB_PIN, OUTPUT);
  pinMode(LEDLOAD_PIN, OUTPUT);

  digitalWrite(MOTOR_PIN, LOW);

  analogWrite(LEDR_PIN, 255);
  analogWrite(LEDG_PIN, 0);
  analogWrite(LEDB_PIN, 30);

  
  setCoresLED_WS(SET_ALL,0,0,0);
      delay(500);
      setCoresLED_WS(0,0,255,0);
      delay(500);
      setCoresLED_WS(1,0,255,0);
      delay(500);
      setCoresLED_WS(2,0,255,0);
      delay(500);
      setCoresLED_WS(3,0,255,0);
      delay(500);
  
}

void loop() {
  umidade_soma = umidade_soma + map(analogRead(UMIDADE_PIN), 700, 350, 0, 100);
  //Serial.println(analogRead(UMIDADE_PIN));
  Serial.println(map(analogRead(UMIDADE_PIN), 700, 350, 0, 100));
  
  umidade_i++;
  if(umidade_media == umidade_i){
    umidade_soma = umidade_soma / umidade_media;

    if (umidade_soma > 75){
      estagio_umidade = 4;
      ativa_motor = 0;
      setCoresLED_WS(SET_ALL,0,255,0);
      Serial.println("U > 75");
    }
    else if (umidade_soma > 50){
      estagio_umidade = 3;
      ativa_motor = 0;
      setCoresLED_WS(0,0,255,0);
      setCoresLED_WS(1,0,255,0);
      setCoresLED_WS(2,0,255,0);
      setCoresLED_WS(3,0,0,0);
      Serial.println("U > 50");
    }
    else if (umidade_soma > 25){
      estagio_umidade = 2;
      ativa_motor = MOTOR_TEMPO;
      setCoresLED_WS(0,0,255,0);
      setCoresLED_WS(1,0,255,0);
      setCoresLED_WS(2,0,0,0);
      setCoresLED_WS(3,0,0,0);
      Serial.println("U > 25");
    }
    else {
      estagio_umidade = 1;
      ativa_motor = MOTOR_TEMPO;
      //setCoresLED_WS(0,0,255,0);
      setCoresLED_WS(1,0,0,0);
      setCoresLED_WS(2,0,0,0);
      setCoresLED_WS(3,0,0,0);
      
      Serial.println("U > 0");
    }
    if (umidade_media > 60){
      umidade_media = 1800;
    }
    else{
      umidade_media = umidade_media + 30;
    }

    Serial.print("UM: ");
    Serial.println(umidade_media);
    
    umidade_soma = 0;
    umidade_i = 0;
  }

  if (ativa_motor > 0){
    analogWrite(MOTOR_PIN, 150);
    //digitalWrite(MOTOR_PIN, HIGH);
    Serial.println("MOTOR - LIGADO");
    ativa_motor--;
  }
  else{
    
    digitalWrite(MOTOR_PIN, LOW);
    Serial.println("MOTOR - DESLIGADO");
  }

  if(estagio_umidade == 1){
    if(led_low==0){
      LED_WS_Fade_off();
      led_low=1;
    }
    else{
      LED_WS_Fade_on();
      led_low=0;
    } 
    delay(250);
  }
  else{
    delay(1000);
  }

  
  digitalWrite(LEDLOAD_PIN, !digitalRead(LEDLOAD_PIN));
}

void setCoresLED_WS(byte index, byte R, byte G, byte B){
  setCoresLED_WS(index, R, G, B, true);
}
void setCoresLED_WS(byte index, byte R, byte G, byte B, bool atualizar){
  
  if (index != SET_ALL){
    pixels.setPixelColor(index, pixels.Color(R,G,B));
    if (atualizar == true){
      pixels.show();
    }
  }else{
    for(int i=0;i<WS_NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(R,G,B));    
    }
    pixels.show();
  }
}

/**************************
'* Name    : LED_WS_Oscilar                                                 *
'* Purpose :                                                                *
'*************************/
void LED_WS_Fade_on(){
    for(byte x=0;x<255;x++){
      //setCoresLED_WS(SET_ALL, 0,x,0);
       pixels.setPixelColor(0, pixels.Color(0,x,0));
       pixels.show();
      delay(2);
    }
}

void LED_WS_Fade_off(){
    for(byte x=0;x<255;x++){
      //setCoresLED_WS(SET_ALL, 0,x,0);
       pixels.setPixelColor(0, pixels.Color(0,255-x,0));
       pixels.show();
      delay(3);
    }
}
