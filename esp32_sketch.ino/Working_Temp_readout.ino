#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "Tap Room Guest";
const char* password = "Hamhock69";

// Web server
AsyncWebServer server(80);

// Discord webhook URL
const char* discordWebhook = "https://discord.com/api/webhooks/1396330426557010012/s8ehOooBbbpJn4Q9dS5sVN-e3Zl7p4TqU7QJ_PxY_bVSTt_8CYDCwbMVBJnbGdcCgvfN";

// DHT settings
#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor values
float temperature = 0.0;
float humidity = 0.0;
float threshold = 76.0;  // Fahrenheit
bool alertSent = false;

unsigned long lastReadTime = 0;
const unsigned long readInterval = 2000;

void sendDiscordAlert(float temp) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(discordWebhook);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"content\": \"🚨 Temperature Alert: " + String(temp, 1) + "°F exceeds threshold of " + String(threshold, 1) + "°F!\"}";
    int httpResponseCode = http.POST(payload);

    Serial.print("Discord response: ");
    Serial.println(httpResponseCode);
    http.end();
  }
}

// Format HTML for web interface
String htmlPage() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>ESP32 Sensor</title><style>";
  html += "body{font-family:sans-serif;background:#111;color:#eee;text-align:center;padding:2em;}";
  html += "input[type=number]{font-size:1.2em;width:60px;margin:1em;}";
  html += "button{font-size:1em;padding:0.5em 1em;background:#333;color:#fff;border:none;border-radius:5px;cursor:pointer;}";
  html += "</style></head><body>";
  html += "<h2>🌡️ Temperature & Humidity</h2>";
  html += "<p><strong>Temp:</strong> " + String(temperature, 1) + " °F</p>";
  html += "<p><strong>Humidity:</strong> " + String(humidity, 1) + " %</p>";
  html += "<form action='/set' method='GET'>";
  html += "<label>Threshold: <input type='number' name='threshold' value='" + String(threshold, 0) + "'></label>";
  html += "<button type='submit'>Set</button></form>";
  html += "<form action='/reset' method='GET'><button type='submit'>Reset Alert</button></form>";
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT
  dht.begin();

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ SSD1306 allocation failed");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Booting...");
  display.display();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Setup web routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlPage());
  });

  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("threshold")) {
      threshold = request->getParam("threshold")->value().toFloat();
      alertSent = false;  // Reset alert flag
    }
    request->redirect("/");
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    alertSent = false;
    request->redirect("/");
  });

  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastReadTime >= readInterval) {
    lastReadTime = currentMillis;

    humidity = dht.readHumidity();
    temperature = dht.readTemperature(true);  // true = Fahrenheit

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("❌ Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" F, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Show on OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("T: ");
    display.print(temperature, 1);
    display.println(" F");
    display.setCursor(0, 48);
    display.print("H: ");
    display.print(humidity, 1);
    display.println(" %");
    display.display();

    // Check alert
    if (temperature > threshold && !alertSent) {
      sendDiscordAlert(temperature);
      alertSent = true;
    }
  }
}
