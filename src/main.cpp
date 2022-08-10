#include <Arduino.h>
#include "SPIFFS.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <SPI.h>
#include <ESPmDNS.h>

char configFile[] = "network.conf";

const char* ssid = "FireNetwork 7";
const char* password = "25quarters";
#define host_name "esp32"

WebServer server(80);

const int led = 13;

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

    File fd = SPIFFS.open(requestUri);
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
    digitalWrite(led, 1);
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
    digitalWrite(led, 0);
  }

  void handleAPICall() {
    String response;
    int r, g, b, l;

    if (server.method() == HTTP_GET) {
      server.send(400, "text/plain", "Bad request");
      return;
    }

    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i).equals("r")) r = server.arg(i).toInt();
      if (server.argName(i).equals("g")) g = server.arg(i).toInt();
      if (server.argName(i).equals("b")) b = server.arg(i).toInt();
      if (server.argName(i).equals("l")) l = server.arg(i).toInt();
    }

    response = String(r);
    response += ",";
    response += String(g);
    response += ",";
    response += String(b);
    response += ",";
    response += String(l);

    Serial.println(response);
    server.send(200, "text/plain", response);
  }

} myHandler;


bool initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(host_name);

  return true;
}

bool initFileSystem(void) {
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }

  return true;
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  // Configure the wifi connection
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
  // MDNS.update();
}


#ifdef NOTES_FOR_LATER

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open("/text.txt");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("File Content: ");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();

  Serial.println("");
}

void loop()
{
  // put your main code here, to run repeatedly:
  sleep(1);
  printHello();
}

#endif