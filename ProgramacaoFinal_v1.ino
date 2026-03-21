#include <WiFi.h>
#include <PubSubClient.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WIFI
const char* ssid = "MV Xiaomi";
const char* password = "123456789";


// MQTT
const char* mqtt_server = "18.191.112.34";

WiFiClient espClient;
PubSubClient client(espClient);

// GPS
TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

// BOTÕES
int botaoAzul = 17;
int botaoVermelho = 5;
int botaoBranco = 16;

// SENSORES
int sensorObstaculo = 27;

// ATUADORES
int led = 25;
int buzzer = 26;

// ESTADO
bool sistemaAtivo = false;

bool azulAnterior = HIGH;
bool vermelhoAnterior = HIGH;
bool brancoAnterior = HIGH;

// ----------------------------

void setup(){

Serial.begin(115200);
Serial.println("Iniciando sistema...");

// BOTÕES
pinMode(botaoAzul, INPUT_PULLUP);
pinMode(botaoVermelho, INPUT_PULLUP);
pinMode(botaoBranco, INPUT_PULLUP);

// SENSOR
pinMode(sensorObstaculo, INPUT);

// ATUADORES
pinMode(led, OUTPUT);
pinMode(buzzer, OUTPUT);

// I2C
Wire.begin(21,22);

// OLED
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
Serial.println("OLED nao encontrado");
}else{
Serial.println("OLED iniciado");
}

// CONFIGURA OLED
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);

// GPS
gpsSerial.begin(9600, SERIAL_8N1, 4, 2);

// WIFI
conectarWiFi();

// MQTT
client.setServer(mqtt_server,1883);

mostrarOLED("SISTEMA INICIADO","AGUARDANDO INSTRUCAO");

Serial.println("Setup finalizado");

}

// ----------------------------

void loop(){

// GPS leitura
while(gpsSerial.available()){
gps.encode(gpsSerial.read());
}

// WIFI reconectar
if(WiFi.status()!=WL_CONNECTED){
conectarWiFi();
}

// MQTT reconectar
if(!client.connected()){
conectarMQTT();
}

client.loop();

// BOTÕES
lerBotoes();

// SISTEMA
if(sistemaAtivo){
verificarObstaculo();
}

}

// ----------------------------

void conectarWiFi(){

Serial.println("Conectando WiFi...");

WiFi.begin(ssid,password);

int tentativas=0;

while(WiFi.status()!=WL_CONNECTED && tentativas<20){

delay(500);
Serial.print(".");
tentativas++;

}

if(WiFi.status()==WL_CONNECTED){

Serial.println("");
Serial.println("WiFi conectado");
Serial.println(WiFi.localIP());

}else{

Serial.println("");
Serial.println("Falha no WiFi");

}

}

// ----------------------------

void conectarMQTT(){

Serial.println("Conectando MQTT...");

int tentativas=0;

while(!client.connected() && tentativas<10){

if(client.connect("ESP32_GPS")){

Serial.println("MQTT conectado");

}else{

Serial.print("Falha MQTT, rc=");
Serial.print(client.state());
Serial.println(" tentando novamente");

delay(2000);
tentativas++;

}

}

}

// ----------------------------

void lerBotoes(){

bool azulAtual = digitalRead(botaoAzul);
bool vermelhoAtual = digitalRead(botaoVermelho);
bool brancoAtual = digitalRead(botaoBranco);

// BOTAO AZUL
if(azulAnterior==HIGH && azulAtual==LOW){

if(!sistemaAtivo){

sistemaAtivo=true;
mostrarOLED("MODO","MONITORANDO");
Serial.println("Sistema ativado");

}else{

mostrarOLED("SISTEMA","JA ATIVO");

}

}

// BOTAO VERMELHO
if(vermelhoAnterior==HIGH && vermelhoAtual==LOW){

if(sistemaAtivo){

sistemaAtivo=false;

digitalWrite(led,LOW);
noTone(buzzer);

mostrarOLED("SISTEMA INICIADO","PARADO");
Serial.println("Sistema parado");

}else{

mostrarOLED("SISTEMA INICIADO","JA PARADO");

}

}

// BOTAO BRANCO
if(brancoAnterior==HIGH && brancoAtual==LOW){

Serial.println("Envio manual GPS");

enviarGPS("manual");

}

azulAnterior=azulAtual;
vermelhoAnterior=vermelhoAtual;
brancoAnterior=brancoAtual;

}

// ----------------------------

void verificarObstaculo(){

if(digitalRead(sensorObstaculo)==LOW){

Serial.println("Obstaculo detectado");

digitalWrite(led,HIGH);
tone(buzzer,2000);

mostrarOLED("OBSTACULO","DETECTADO");

enviarGPS("obstaculo");

while(digitalRead(sensorObstaculo)==LOW){
delay(50);
}

digitalWrite(led,LOW);
noTone(buzzer);

}

}

// ----------------------------

void enviarGPS(String evento){

if(gps.location.isValid()){

float lat=gps.location.lat();
float lon=gps.location.lng();

String msg="{";
msg+="\"lat\":";
msg+=String(lat,6);
msg+=",\"lon\":";
msg+=String(lon,6);
msg+=",\"evento\":\"";
msg+=evento;
msg+="\"}";

client.publish("iot/gps",msg.c_str());

Serial.println("GPS enviado:");
Serial.println(msg);

mostrarOLED("GPS ENVIADO",evento);

}else{

Serial.println("GPS sem sinal");

mostrarOLED("GPS","SEM SINAL");

}

}

// ----------------------------

void mostrarOLED(String l1,String l2){

display.clearDisplay();

display.setCursor(0,10);
display.println(l1);

display.setCursor(0,30);
display.println(l2);

display.display();

}