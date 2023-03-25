#include <AM2320_asukiaaa.h>
#include <LiquidCrystal.h>
AM2320_asukiaaa temp_hum_sensor;
#include <WiFi.h>
#include <PubSubClient.h>

LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

const char* ssid = "Moker WiFi_ExtraWiFi2G";
const char* password = "500PilsAUB!2GHZ";

const char* mqtt_server = "192.168.2.16";
const char* mqtt_user = "esp_temp_hum";
const char* mqtt_pass = "esp_temp_hum_pass";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("started");
  Wire.begin();
  temp_hum_sensor.setWire(&Wire);

  //Set the screen size
  lcd.begin(16, 2);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}

void loop() {
  if (temp_hum_sensor.update() != 0) {
    Serial.println("Error: Cannot update sensor values.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error");
  } else {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("tmp: " + String(temp_hum_sensor.temperatureC) + " C");
    lcd.setCursor(0, 1);
    lcd.print("hum: " + String(temp_hum_sensor.humidity) + " %");

    send_mqtt(temp_hum_sensor.temperatureC, temp_hum_sensor.humidity);
  }

  delay(5000);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conn. to WiFi");
    
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/living_room/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void send_mqtt(float temperature, float humidity){
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Convert the value to a char array
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  Serial.print("Temperature: ");
  Serial.println(tempString);
  client.publish("esp32/living_room/temperature", tempString);
  
  // Convert the value to a char array
  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  Serial.print("Humidity: ");
  Serial.println(humString);
  client.publish("esp32/living_room/humidity", humString);
}
