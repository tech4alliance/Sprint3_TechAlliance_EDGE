#include "NMEA.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 4   
#define DHTTYPE DHT22
#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0])) 

float latitude;
float longitude;

union {
  char bytes[4];
  float valor;
} velocidadeGPS;

NMEA gps(GPRMC);  
DHT dht(DHTPIN, DHTTYPE);
 
// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
//const char* default_SSID = "FIAP-IBM"; // Nome da rede Wi-Fi
//const char* default_PASSWORD = "Challenge@24!"; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "44.195.44.28"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/TechAlliance/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/TechAlliance/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/TechAlliance/attrs/t"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_3 = "/TEF/TechAlliance/attrs/h"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_4 = "/TEF/TechAlliance/attrs/long"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_5 = "/TEF/TechAlliance/attrs/lat"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_6 = "/TEF/TechAlliance/attrs/v"; // Tópico MQTT de envio de informações para Broker
const char* default_ID_MQTT = "fiware_TechAlliance"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard
// Declaração da variável para o prefixo do tópico
const char* topicPrefix = "TechAlliance";
 
// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* TOPICO_PUBLISH_3 = const_cast<char*>(default_TOPICO_PUBLISH_3);
char* TOPICO_PUBLISH_4 = const_cast<char*>(default_TOPICO_PUBLISH_4);
char* TOPICO_PUBLISH_5 = const_cast<char*>(default_TOPICO_PUBLISH_5);
char* TOPICO_PUBLISH_6 = const_cast<char*>(default_TOPICO_PUBLISH_6);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;
 
WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';
 
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}
 
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}
 
void setup() {
    dht.begin();
    InitOutput();
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}
 
void loop() {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    handleDHT();
    handleGPS();
    Serial.println();
    delay(2000);
    MQTT.loop();
}
 
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
 
    // Garantir que o LED inicie desligado
    digitalWrite(D4, LOW);
}
 
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
 
    // Forma o padrão de tópico para comparação
    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";
 
    // Compara com o tópico recebido
    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }
 
    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}
 
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}
 
void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }
 
    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}
 
void InitOutput() {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    boolean toggle = false;
 
    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}
 
void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

void handleDHT() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    String temp = String(temperature);
    String humi = String(humidity);
    Serial.print("Temperatura: ");
    Serial.println(temp.c_str());
    Serial.print("Humidade: ");
    Serial.println(humi.c_str());
    MQTT.publish(TOPICO_PUBLISH_2, temp.c_str());
    MQTT.publish(TOPICO_PUBLISH_3, humi.c_str());
}

void handleGPS(){

  while(Serial2.available()) {             // Waits for serial port data
    char serialData = Serial2.read();       // Receives data from GPS serial port

    if(gps.decode(serialData)) {           // Checks if the GPS sentence is valid
      if(gps.gprmc_status() == 'A') {      // Checks if GPS status is 'A'

        velocidadeGPS.valor = gps.gprmc_speed(KMPH);        // Receives GPS speed in km/h

      } else {
        velocidadeGPS.valor = 0;
      }

      latitude  = gps.gprmc_latitude();
      longitude = gps.gprmc_longitude();

      // Show Latitude
      Serial.print("Latitude: ");
      Serial.println(latitude, 8);

      // Show Longitude
      Serial.print("Longitude: ");
      Serial.println(longitude, 8);

      // Show Speed ​​in km/h
      Serial.print("    Speed: ");
      Serial.print(velocidadeGPS.valor);
      Serial.println(" Km/h");

      // Sending Values to MQTT
      MQTT.publish(TOPICO_PUBLISH_4, String(longitude, 8).c_str());
      MQTT.publish(TOPICO_PUBLISH_5, String(latitude, 8).c_str());
      MQTT.publish(TOPICO_PUBLISH_6, String(velocidadeGPS.valor).c_str());

      // Converts Geographic Coordinates to Cartesian Plane
      convertCoordinatesToCartesian(latitude, longitude);
    }
  }
}

void convertCoordinatesToCartesian(float latitude, float longitude) {

  float latRadius = latitude  * (PI) / 180;  // Convert from Degrees to Radians
  float lonRadius = longitude * (PI) / 180;

  int earthRadius = 6371; // Radius in km

  float posX = earthRadius * cos(latRadius) * cos(lonRadius);
  float posY = earthRadius * cos(latRadius) * sin(lonRadius);

  Serial.print("        X: ");  Serial.println(posX);
  Serial.print("        Y: ");  Serial.println(posY);
}