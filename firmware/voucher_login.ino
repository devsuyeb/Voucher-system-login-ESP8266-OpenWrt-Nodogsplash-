#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASS";

ESP8266WebServer server(80);

#define VOUCHER_LENGTH 10  // 9 chars + null terminator
#define MAX_VOUCHERS 50
#define EEPROM_SIZE (MAX_VOUCHERS * VOUCHER_LENGTH)

// ========== EEPROM FUNCTIONS ==========
void writeVoucherToEEPROM(int index, const char* voucher) {
  int addr = index * VOUCHER_LENGTH;
  for (int i = 0; i < VOUCHER_LENGTH; i++) {
    char c = (i < strlen(voucher)) ? voucher[i] : '\0';
    EEPROM.write(addr + i, c);
  }
  EEPROM.commit();
}

void readVoucherFromEEPROM(int index, char* buffer) {
  int addr = index * VOUCHER_LENGTH;
  for (int i = 0; i < VOUCHER_LENGTH; i++) {
    buffer[i] = EEPROM.read(addr + i);
  }
  buffer[VOUCHER_LENGTH - 1] = '\0';
}

int findEmptySlot() {
  char buf[VOUCHER_LENGTH];
  for (int i = 0; i < MAX_VOUCHERS; i++) {
    readVoucherFromEEPROM(i, buf);
    if (buf[0] == '\0' || buf[0] == 0xFF) return i;
  }
  return -1;
}

int findVoucherIndex(const char* voucher) {
  char buf[VOUCHER_LENGTH];
  for (int i = 0; i < MAX_VOUCHERS; i++) {
    readVoucherFromEEPROM(i, buf);
    if (strcmp(buf, voucher) == 0) return i;
  }
  return -1;
}

// ========== HTTP HANDLERS ==========
void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  server.send(204);
}

void handleValidation() {
  // Handle OPTIONS preflight
  if (server.method() == HTTP_OPTIONS) {
    handleOptions();
    return;
  }

  if (server.method() != HTTP_POST) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(405, "application/json", "{\"error\":\"Method not allowed\"}");
    return;
  }

  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, server.arg("plain"));
  
  if (error) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    return;
  }

  String voucher = doc["voucher"].as<String>();
  voucher.trim();
  
  Serial.println("\n=== Validation Request ===");
  Serial.println("Voucher: " + voucher);

  // Validation checks
  if (voucher.length() >= VOUCHER_LENGTH) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"error\":\"Voucher too long\"}");
    return;
  }

  if (voucher.length() == 0) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"error\":\"Empty voucher\"}");
    return;
  }

  int index = findVoucherIndex(voucher.c_str());
  if (index >= 0) {
    writeVoucherToEEPROM(index, "");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"status\":\"success\",\"valid\":true}");
  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(403, "application/json", "{\"error\":\"Invalid voucher\"}");
  }
}

void handleAdminPage() {
  String html = "<h2>Voucher Admin</h2><form method='POST' action='/add'><input name='voucher'><button>Add</button></form><h3>Active Vouchers:</h3><ul>";
  
  char buf[VOUCHER_LENGTH];
  for (int i = 0; i < MAX_VOUCHERS; i++) {
    readVoucherFromEEPROM(i, buf);
    if (buf[0] != '\0' && buf[0] != 0xFF) {
      html += "<li>" + String(buf) + "</li>";
    }
  }
  html += "</ul>";
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", html);
}

void handleAddVoucher() {
  if (!server.hasArg("voucher")) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "Missing voucher");
    return;
  }

  String voucher = server.arg("voucher");
  if (voucher.length() == 0 || voucher.length() >= VOUCHER_LENGTH) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "Invalid length");
    return;
  }

  if (findVoucherIndex(voucher.c_str()) != -1) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "Voucher exists");
    return;
  }

  int slot = findEmptySlot();
  if (slot == -1) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "text/plain", "Storage full");
    return;
  }

  writeVoucherToEEPROM(slot, voucher.c_str());
  server.sendHeader("Location", "/admin");
  server.send(303);
}

// ========== MAIN FUNCTIONS ==========
void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  // Clear EEPROM (uncomment only once)
  // for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0); EEPROM.commit();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.on("/validate", HTTP_OPTIONS, handleOptions);
  server.on("/validate", HTTP_POST, handleValidation);
  server.on("/admin", HTTP_GET, handleAdminPage);
  server.on("/add", HTTP_POST, handleAddVoucher);

  server.begin();
  Serial.println("Server started with CORS support");
}

void loop() {
  server.handleClient();
}