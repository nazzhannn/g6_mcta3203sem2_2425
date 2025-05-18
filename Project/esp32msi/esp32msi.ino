#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "-";
const char* password = "-";

const char* firebaseApiKey = "-";
const char* firebaseEmail = "-";
const char* firebasePassword = "-";

String idToken = "";
const char* projectId = "washingmachine-6f7d6";
const char* sessionId = "0991";

String tempReceived = "";
int priceReceived = 0;

void setup() {
  Serial.begin(115200);      // Serial Monitor
  Serial2.begin(9600, SERIAL_8N1, 16, 17);     // Serial to Arduino

  Serial.println("=== ESP32 SETUP START ===");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  if (signIn()) {
    Serial.println("Firebase signed in");
  } else {
    Serial.println("Firebase login failed");
  }

  Serial.println("=== SETUP COMPLETE ===");
}

void loop() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    data.trim();
    Serial.println("\nReceived from Arduino: " + data);

    int commaIndex = data.indexOf(',');
    if (commaIndex > 0) {
      tempReceived = data.substring(0, commaIndex);
      priceReceived = data.substring(commaIndex + 1).toInt();
      Serial.println("Temp: " + tempReceived + " | Price: RM" + String(priceReceived));

      if (idToken != "") {
        updateFirestoreData(sessionId, tempReceived, priceReceived);
      } else {
        Serial.println("No Firebase token, skipping update");
      }
    } else {
      Serial.println("Invalid data format from Arduino");
    }
  }

  if (idToken != "") {
    String status = checkFirestoreStatus(sessionId);
    if (status == "paid") {
      Serial.println("paid");
      Serial2.println("paid");
    } else {
      Serial.println("Status: " + status);
      Serial2.println(status);
    }
  }

  delay(2000);
}

bool signIn() {
  Serial.println("Signing in to Firebase...");
  HTTPClient http;
  http.begin("https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + String(firebaseApiKey));
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"email\":\"" + String(firebaseEmail) + "\",\"password\":\"" + String(firebasePassword) + "\",\"returnSecureToken\":true}";
  Serial.println("Auth Payload: " + payload);

  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println("Auth Response: " + response);

    DynamicJsonDocument doc(2048);
    deserializeJson(doc, response);
    idToken = doc["idToken"].as<String>();
    return true;
  } else {
    Serial.printf("Auth failed, HTTP code: %d\n", httpCode);
    Serial.println("Response: " + http.getString());
    return false;
  }
}

void updateFirestoreData(String sessionId, String temp, int price) {
  HTTPClient http;

  String url = "https://firestore.googleapis.com/v1/projects/" + String(projectId) +
               "/databases/(default)/documents/sessions/" + sessionId +
               "?updateMask.fieldPaths=temp&updateMask.fieldPaths=price";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + idToken);

  DynamicJsonDocument doc(512);
  JsonObject fields = doc.createNestedObject("fields");
  fields["temp"]["stringValue"] = temp;
  fields["price"]["integerValue"] = price;

  String jsonPayload;
  serializeJson(doc, jsonPayload);

  Serial.println("PATCH Firestore URL: " + url);
  Serial.println("Payload: " + jsonPayload);

  int httpCode = http.PATCH(jsonPayload);
  String response = http.getString();

  if (httpCode > 0) {
    Serial.println("Firestore update successful");
    Serial.println("Response: " + response);
  } else {
    Serial.printf("Firestore update failed: %d\n", httpCode);
    Serial.println("Response: " + response);
  }

  http.end();
}

String checkFirestoreStatus(String sessionId) {
  HTTPClient http;
  String url = "https://firestore.googleapis.com/v1/projects/" + String(projectId) +
               "/databases/(default)/documents/sessions/" + sessionId;

  http.begin(url);
  http.addHeader("Authorization", "Bearer " + idToken);

  int httpCode = http.GET();
  String payload = http.getString();

  if (httpCode > 0) {
    Serial.println("GET status response: " + payload);

    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);
    String status = doc["fields"]["status"]["stringValue"];
    return status;
  } else {
    Serial.printf("GET failed: %d\n", httpCode);
    Serial.println("Response: " + payload);
    return "";
  }

  http.end();
}
