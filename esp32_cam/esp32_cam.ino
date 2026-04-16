#include "esp_camera.h"
#include <WiFi.h>
#include "esp_wifi.h"
#include <WebServer.h>

// AI Thinker ESP32-CAM pin config
// don't change these unless you're using a different module
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// AP credentials - change password if deploying outside lab
const char* ssid = "ESP32-CAM";
const char* password = "12345678";

WebServer server(80);

// streams mjpeg frames to browser
void handleStream() {
  WiFiClient client = server.client();

  // send multipart header first
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  server.sendContent(response);

  while (true) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) continue; // skip if frame not ready

    client.write("--frame\r\n", 9);
    client.write("Content-Type: image/jpeg\r\n\r\n", 26);
    client.write(fb->buf, fb->len);
    client.write("\r\n", 2);

    esp_camera_fb_return(fb);

    if (!client.connected()) break; // stop if client disconnects
  }
}

// basic root page, just shows the stream
void handleRoot() {
  String html = "<html><body><h2>ESP32 CAM Stream</h2>";
  html += "<img src='/stream'>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  // camera config - using QVGA for better performance over WiFi
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size   = FRAMESIZE_QVGA;
  config.jpeg_quality = 10; // lower = better quality, higher = faster
  config.fb_count     = 2;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed, check wiring");
    return;
  }

  WiFi.softAP(ssid, password);
  Serial.println("AP started");
  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("IP: ");   Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/stream", HTTP_GET, handleStream);
  server.begin();
  Serial.println("Server running");
}

void loop() {
  server.handleClient();
}