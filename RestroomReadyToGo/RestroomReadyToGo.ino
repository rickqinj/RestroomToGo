/**
 * Restroom Ready-To-Go
 * 
 * © 2023 Lincoln Appliance Service Centre Inc.
 **/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>

//PINs definition.
const int WIFI_CFG_PIN = 4;  //GPIO4
const int SENSOR_PIN = 5;    //GPIO5

//WiFi Manager
WiFiManager wm;
//web server
ESP8266WebServer server(80);

int sensorVal = 0;
boolean allSetReady = false;

void prepareService() {
  Serial.println("mDNS starting:");
  while (!MDNS.begin("restroom")) {
    Serial.println("Retry to start mDNS...");
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
  }
  Serial.println("mDNS http://restroom.local/ ready");

  //Prepare web server.
  server.on("/", requestStatus);
  server.onNotFound(handleNotFound);
  
  server.stop();
  delay(2000);
  server.begin();
  
  allSetReady = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(WIFI_CFG_PIN, INPUT_PULLUP);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  
  digitalWrite(2, HIGH);
  
  WiFi.mode(WIFI_STA);

  wm.setConnectTimeout(90);
  wm.setBreakAfterConfig(true);
  wm.autoConnect("RestroomCfgAP");
  prepareService();
  digitalWrite(2, LOW);

  delay(1000);
}

void handleNotFound() {
  String message = "Page Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
}

void requestStatus() {
  String str = "<html>";
  str += "\n<head><meta name='viewport' content='width=device-width, initial-scale=1' />";
  str += "\n<title>Restroom Ready-To-Go</title>";
  str += "\n</head>";
  str += "\n<body><script>";
  str += "setTimeout(function(){window.location.reload();}, 2000);";
  str += "\n</script><h2><strong>";
  if (sensorVal == 0) {
    Serial.println("Occupied.");
    str += "Occupied";
  } else {
    Serial.println("Ready.");
    str += "Ready";
  }
  str += "</strong></h2></body></html>";
  server.send(200, "text/html", str);
}



void configWiFi() {
  Serial.println(" ");
  Serial.println("Configure WiFi connection:");
  digitalWrite(2, HIGH);
  wm.resetSettings();
  wm.setConnectTimeout(90);
  wm.setBreakAfterConfig(true);
  wm.autoConnect("RestroomCfgAP");

  delay(2000);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Configuration failed. Please retry.");
    return;
  }
  
  prepareService();

  digitalWrite(2, LOW);
}

void loop() {
  delay(200);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
    allSetReady = false;
    if (digitalRead(WIFI_CFG_PIN) == LOW) {
      configWiFi();
    }
  }
  if (digitalRead(WIFI_CFG_PIN) == LOW) {
    configWiFi();
  }
  if (allSetReady == false) {
    prepareService();
  }

  sensorVal = digitalRead(SENSOR_PIN);
  Serial.println(sensorVal);
  
  server.handleClient();
  MDNS.update();
}
