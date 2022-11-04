#include <Arduino.h>
#include "LittleFS.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SPI.h>
#include <ESPmDNS.h>

#define host_name "esp32"

WebServer server(80);

class MyHandler : public RequestHandler {
  bool canHandle(HTTPMethod method, String uri) {
    return uri.startsWith( "/" );
  }

  bool handle(WebServer& server, HTTPMethod requestMethod, String requestUri) {
    String contentType;

    if (requestUri.equals("/")) {
      requestUri = "/index.html";
    }

    if (requestUri.equals("/api")) {
      handleAPICall();
      return true;
    }

    File fd = LittleFS.open(requestUri);
    if (!fd) {
      handleNotFound();
    } else {
      size_t filesize = fd.size(); //the size of the file in bytes
      char string[filesize + 1];
      memset(string, 0, filesize + 1);

      fd.read((uint8_t *)string, sizeof(string));
      fd.close();

      if (requestUri.endsWith(".html")) contentType = "text/html";
      else if (requestUri.endsWith(".css")) contentType = "text/css";
      else if (requestUri.endsWith(".js")) contentType = "text/javascript";
      else if (requestUri.endsWith(".ico")) contentType = "image/x-icon";
      else if (requestUri.endsWith(".png")) contentType = "image/png";
      else if (requestUri.endsWith(".jpg")) contentType = "image/jpg";
      else if (requestUri.endsWith(".jpeg")) contentType = "image/jpeg";
      else if (requestUri.endsWith(".gif")) contentType = "image/gif";
      else if (requestUri.endsWith(".svg")) contentType = "image/svg+xml";
      else if (requestUri.endsWith(".webp")) contentType = "image/webp";
      else if (requestUri.endsWith(".avif")) contentType = "image/avif";
      else if (requestUri.endsWith(".apng")) contentType = "image/apng";

      Serial.print("Request: ");
      Serial.print(requestUri);
      Serial.print(" 200 ");
      Serial.println(contentType);

      server.send(200, contentType, string);
      return true;
    }

    return false;
  }

  void handleNotFound() {
    String message = "File Not Found\n\n";
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

    server.send(404, "text/plain", message);
  }

  void handleAPICall() {
    String response;
    int pin = -1;
    int value = -1;

    if (server.method() == HTTP_GET) {
      server.send(400, "text/plain", "Bad request");
      return;
    }

    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i).equals("p")) pin = server.arg(i).toInt();
      if (server.argName(i).equals("v")) value = server.arg(i).toInt();
    }

    if (pin != -1 && value != -1) {
      digitalWrite(pin, value);
    }

    response = String(pin);
    response += ",";
    response += String(value);

    Serial.println(response);
    server.send(200, "text/plain", response);
  }

} myHandler;


bool initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nConnected to: %s\n", WIFI_SSID);
  Serial.printf("IP address: %s\n", WiFi.localIP().toString());
  Serial.printf("Hostname: %s\n", host_name);

  return true;
}

bool initFileSystem(void) {
  if (!LittleFS.begin(true)) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return false;
  }

  return true;
}

void setup(void) {

  Serial.begin(115200);

  pinMode(2, OUTPUT);
  for (int pin = 12; pin < 24; pin++) {
    pinMode(pin, OUTPUT);
  }

  // Configure the filesystem connection
  if (!initFileSystem()) return;

  // Configure the wifi connection
  if (!initWifi()) return;

  if (MDNS.begin(host_name)) {
    Serial.println("MDNS responder started");
  }

  server.addHandler (&myHandler);
  server.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
