#include <PubSubClient.h>
#include <WiFi.h>

const char* SSID = "dlink";                // nome da rede WiFi que vai ser conectada
const char* PASSWORD = "billypink";   // senha da rede WiFi que vai ser conectada
WiFiClient wifiClient;

const char* BROKER_MQTT = "mqtt.eclipse.org"; //URL do broker MQTT utilizado
int BROKER_PORT = 1883;

#define ID_MQTT  "TMP_MQTT_PROJECT"            //informando ID e tópicos do projeto 
#define TOPIC_PUBLISH "TMP_SEND"
#define TOPIC_SUBSCRIBE "PB"
PubSubClient MQTT(wifiClient);        // instanciando o cliente MQTT

void mantemConexoes();  //garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //faz conexão com WiFi
void conectaMQTT();     //faz conexão com Broker MQTT
void enviaPacote();
void recebePacote(char* topic, byte* payload, unsigned int length);
void automatico();

int sens; //criando as variaveis e nomeando os pins
float voltage;
float milliVolt;
float tmp;
bool automatic = false;
int pinSensor = 4;
#define pinLED1 32

void setup() {
  pinMode(pinLED1, OUTPUT); //fazendo o setup dos pins e baud rate
  pinMode(pinSensor, INPUT);
  
  Serial.begin(115200);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
  MQTT.setCallback(recebePacote);
}

void loop() {
  if (automatic==true){
    automatico();  
  }
  mantemConexoes();
  enviaPacote();
  MQTT.loop();
  
  delay(5000);
}

void mantemConexoes() {
    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
    
    conectaWiFi(); //refaz a conexão
}

void conectaWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
        
  Serial.print("conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println(";");

  WiFi.begin(SSID, PASSWORD); // conecta na rede Wi-Fi  
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  
  Serial.println();
  Serial.print("conectado com sucesso na rede: ");
  Serial.print(SSID);  
  Serial.print("IP obtido: ");
  Serial.println(WiFi.localIP()); 
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("conectado ao Broker com sucesso!");
        } 
        else {
            Serial.println("não foi possível se conectar ao broker.");
            Serial.println("nova tentativa de conexão em 10s");
            delay(10000);
        }
    }
}

void enviaPacote() {

  sens = analogRead(pinSensor); //lê o sensor do sensor (em volts)
  voltage = (sens*5000)/1024; //calcula o valor em celsius a partir do valor em volts
  tmp = voltage/10;

  Serial.print("Celsius: ");
  Serial.println(tmp);
  
  if (tmp>32){
    MQTT.publish(TOPIC_PUBLISH, "0");
  }
  if (tmp<32){
    MQTT.publish(TOPIC_PUBLISH, "1");  
  }
}

void recebePacote(char* topic, byte* payload, unsigned int length) 
{
    String msg;

    //obtem a string do payload
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

    if (msg == "0") {
       digitalWrite(pinLED1, LOW);
    }

    if (msg == "1") {
       digitalWrite(pinLED1, HIGH);
    }

    if (msg == "3"){
      Serial.println("automatico desativado");
      automatic = false;
    }

    if (msg == "4"){
      Serial.println("automatico ativado");
      automatic = true;
    }
}

void automatico(){
  if (tmp>32){ // se a temperatura estiver alta, liga o ventilador
    digitalWrite(pinLED1, HIGH);
  }
  if (tmp<32){ // se a temperatura estiver baixa, desliga o ventilador
    digitalWrite(pinLED1, LOW);
  }
}
