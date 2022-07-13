// MQTT + WIFIMANAGER
// AUTOMATIC CHANGE WIFI
// Coded and Tested By Ari

#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char *mqtt_server = "rmq2.*******.id";
const char *mqtt_user   = "/mahasiswaubl:ubliot";
const char *mqtt_pass   = "********";
const char *mqtt_topic  = "homeautoLSKK";
const char *mqtt_cl     = "LSKK";
char smqtt_port[5] = "1883";
bool shouldSaveConfig = false;
int KondisiLed = 0 ;



WiFiClient espClient;
PubSubClient client(espClient);

void setup (){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  String(smqtt_port).toCharArray(smqtt_port, 5);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", smqtt_port, 5);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user, 40);
  WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqtt_pass, 40);

  WiFiManager WM;
  WM.setSaveConfigCallback(saveConfigCallback);
  WM.addParameter(&custom_mqtt_server);
  WM.addParameter(&custom_mqtt_port);
  WM.addParameter(&custom_mqtt_user);
  WM.addParameter(&custom_mqtt_password);

  WM.resetSettings();
  WM.setAPStaticIPConfig(IPAddress(192,168,0,1), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
  WM.autoConnect("ESP666", "12345678");
  Serial.print("IP Address : ");
  Serial.print(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}
void loop (){
   if(!client.connected()){
    reconnect();
   }else{
    client.loop();
    digitalWrite(LED_BUILTIN, KondisiLed);
   }
}
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
void callback(char* mqtt_topic, byte* payload, unsigned int length) {
  String response;

  for (int i = 0; i < length; i++) {
    response += (char)payload[i];
  }

  Serial.println(response);
  if(response == "M"){
    Serial.println("LAMPU DIMATIKAN");
    KondisiLed = 1;
  }
  if (response == "H"){
    Serial.println("LAMPU DIHIDUPKAN");
    KondisiLed = 0;
  }
  if (response == "B"){
    Serial.println("LAMPU NGEBLINK");
    ngeblink();
  }
}
void reconnect() {
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect(mqtt_cl, mqtt_user, mqtt_pass)) {
      client.subscribe(mqtt_topic);
      Serial.println("connected");
      Serial.print("Subcribed to: ");
      Serial.println(mqtt_topic);    
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void ngeblink(){
  for(int a = 0;a <= 10; a++){
    digitalWrite(LED_BUILTIN, 1);
    delay(1000);
    digitalWrite(LED_BUILTIN, 0);
    delay(1000);
  }
}
