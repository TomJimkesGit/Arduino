

// include the library code:
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <HTTPClient.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

int up_button_pin = 18;
int down_button_pin = 19;
int enter_button_pin = 21;
int back_button_pin = 22;

int up_state = 0;
int down_state = 0;
int enter_state = 0;
int back_state = 0;

String lamps[6] = {"Standing Lamp", "Spot Lamp", "Kitchen Lamp", "Hallway Lamps", "Bedroom Lamp", ""};
int brightness[5] = {0, 0, 0, 0, 0};
int state[5] = {0, 0, 0, 0, 0};
int current_index = 0;

// 0 = main, 1 = lamp
int menu_depth = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  write_main_menu();

  pinMode(up_button_pin, INPUT_PULLDOWN);
  pinMode(down_button_pin, INPUT_PULLDOWN);
  pinMode(enter_button_pin, INPUT_PULLDOWN);
  pinMode(back_button_pin, INPUT_PULLDOWN);

  //Wifi setup
  connect_to_wifi();
  get_lamps();
}

void loop() {

  int up_state_read = digitalRead(up_button_pin);
  int down_state_read = digitalRead(down_button_pin);
  int enter_state_read = digitalRead(enter_button_pin);
  int back_state_read = digitalRead(back_button_pin);

  if (up_state_read > up_state){
    if(menu_depth == 0){
      scroll_up();
    } else {
      increase_brightness();
      write_lamp_menu();
    } 
  } else if (down_state_read > down_state){
    if(menu_depth == 0){
      scroll_down();
    } else {
      decrease_brightness();
      write_lamp_menu();
    } 
  } else if (enter_state_read > enter_state){
    if (menu_depth == 1){
      switch_lamp_state();
      write_lamp_menu();
    } else {
      menu_depth = 1;
      write_lamp_menu();
    }
  } else if (back_state_read > back_state){
    if (menu_depth == 1){
      menu_depth = 0;
      write_main_menu();
    }
  }
  
  enter_state = enter_state_read;
  up_state = up_state_read;
  down_state = down_state_read;
  back_state = enter_state;
  
  delay(10);
}

void scroll_up(){
  if(current_index == 0){
    return;
  }
  current_index -= 1;
  write_main_menu();
}

void scroll_down(){
  if(current_index == 4){
    return;
  }
  current_index += 1;
  write_main_menu();
}

void write_lamp_menu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> ");
  lcd.print(lamps[current_index]);

  lcd.setCursor(2, 1);
  if (state[current_index] == 0){
    lcd.print("off ");
  } else {
    lcd.print("on  ");
  }
  lcd.print("br: ");
  lcd.print(brightness[current_index]);
  lcd.print("%");
  
}

void increase_brightness(){
  if (brightness[current_index] < 100){
    brightness[current_index] += 10;
  }
}

void decrease_brightness(){
  if (brightness[current_index] > 0){
    brightness[current_index] -= 10;
  }
}

void switch_lamp_state(){
  if (state[current_index] == 0){
    state[current_index] = 1;
  } else {
    state[current_index] = 0;
  }
}

void write_main_menu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> ");
  lcd.print(lamps[current_index]);

  lcd.setCursor(2, 1);
  lcd.print(lamps[current_index + 1]);
}


//--------------- web request code ----------------

char* ha_access_token = "Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI3NTc2OGNhMDg0ZWE0ODNjYjE1YmQxNjQ0OTgxNWIwZiIsImlhdCI6MTY2ODcxMjkyOSwiZXhwIjoxOTg0MDcyOTI5fQ.OGfaiL675QN8dvqFq0tgYespo-5A2w2ZsFd-6tmlvZA";
char* ha_api_endpoint = "http://homeassistant.local/api/";
int ha_api_port = 8123;

const char* ssid = "Moker WiFi_ExtraWiFi2G";
const char* password = "500PilsAUB!2G";

void connect_to_wifi(){

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void get_lamps(){

  WiFiClient wifi;
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    char* serverPath = "http://homeassistant.local:8123/api/";
    http.begin(serverPath);
    int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    
    /*
      HttpClient client = HttpClient(wifi, ha_api_endpoint, ha_api_port);
      client.beginRequest();
      client.get("/");
      client.sendHeader("Authorization", ha_access_token);
      client.sendHeader("Content-Type", "application/json");
      client.endRequest();

      int statusCode = client.responseStatusCode();
      String response = client.responseBody();
    
      Serial.print("GET Status code: ");
      Serial.println(statusCode);
      Serial.print("GET Response: ");
      Serial.println(response);
      */
  }
}

String format_request_string(){
  
}
