#include <WiFi.h>
#include <AsyncMqttClient.h>

const char* ssid = " ";
const char* password = " ";
const char* mqttServer = " ";
const uint16_t mqttPort = 1883;
const char* mqttUsername = " ";
const char* mqttPassword = " ";
const char* mqttTopic = " ";

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
TimerHandle_t messageTimer;

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  xTimerStart(messageTimer, 0); // Memulai timer untuk mengirim pesan
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  xTimerStop(messageTimer, 0); // Menghentikan timer saat terputus dari MQTT
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.print("WiFi connected. IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); 
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void sendMessage(TimerHandle_t xTimer) {
  String message = "{\"message\": 78}";  // Format JSON yang benar
  uint16_t packetId = mqttClient.publish(mqttTopic, 2, false, message.c_str());
  Serial.print("Publishing at QoS 2, packetId: ");
  Serial.println(packetId);
}


void setup() {
  Serial.begin(115200);
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  
  messageTimer = xTimerCreate("messageTimer", pdMS_TO_TICKS(10000), pdTRUE, (void*)0, sendMessage);

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCredentials(mqttUsername, mqttPassword);

  connectToWifi();
}

void loop() {
  // Tidak ada yang perlu dilakukan di sini. AsyncMqttClient dan Timer akan mengurus semuanya.
}
