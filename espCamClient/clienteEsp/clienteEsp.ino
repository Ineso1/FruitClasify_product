#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "esp_camera.h"
#include "img_converters.h"

#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>

#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>

#include <ESP32Servo.h>

// Replace with your network credentials
//const char* ssid = "INFINITUM1C84";
//const char* password = "7g9N9n9YA7";

//const char* ssid = "Iphone de Cesar";
//const char* password = "holamundo";

const char* ssid = "INFINITUM6CDF";
const char* password = "Pm2Fb9Zv3d";

AsyncWebServer server(80);

boolean takeNewPhoto = false;

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
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

HTTPClient client;
String serverClasification = "http://192.168.1.76:5000/newSample";
String serverDB = "http://192.168.1.76:6000/newSample";
//String serverClasification = "http://192.168.1.70:5000/newSample";
//Mi ip 192.168.1.70

#define infraTest   14
#define pulsoCalidad   15
#define SERVO1   12
#define SERVO2   13
bool Captura = false;
bool Calidad = false;

Servo servo1;
Servo servo2;

String idMaquina = "1001";

void cameraInit(){
  // Configuracion pines para camara OV2640
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
}

camera_fb_t* takePhoto(){
  camera_fb_t* fb = NULL;
  fb = esp_camera_fb_get();
  return fb;
}

void parsingResult(String response){
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, response);
  JsonArray array = doc.as<JsonArray>();
  for(JsonVariant v : array){
    JsonObject object = v.as<JsonObject>();
    const char* description = object["description"];
    float score = object["score"];
    String label = "";
    label += description;
    label += ":";
    label += score;
    Serial.print(label);
  }
}

void postingImage(camera_fb_t *fb){

  client.begin(serverClasification);
  
  client.addHeader("Content-Type", "image/jpeg");
  int httpResponseCode = client.POST(fb->buf, fb->len);
  Serial.println(httpResponseCode);
  if(httpResponseCode == 200){
    String response = client.getString();
    Serial.println("Rifo");
    Serial.println(response);
    clasify(response);
    parsingResult(response);
  }else{
    Serial.println("Error en post de la imagen");
  }
  client.end();
}

void activador(){
   camera_fb_t *fb = takePhoto();
   if(!fb || fb->format != PIXFORMAT_JPEG){
    Serial.println("Camera capture failed");
    esp_camera_fb_return(fb);
    return;
    }
    else{
    Serial.println("\nCaptura exitosa");
    postingImage(fb);
    }
    esp_camera_fb_return(fb);
}

String calidadFruta(float perimetro, int area, int porcentajeAzul, int porcentajeVerde, int porcentajeRojo){
  if (porcentajeRojo > 90){
    desechar();
    return "Mala";
  }
  else{ 
    if(perimetro<1200 || porcentajeVerde > 90){
      procederProceso(0);
     return "Media";
    }
    else{
      procederProceso(1);
      return "Excelente";
      }
     return "Indefinido";
  }
  }

void saveOnDb(String sampleData){
  client.begin(serverDB);
  sampleData = idMaquina + sampleData;
  int httpResponseCode = client.POST(sampleData);
  Serial.println(httpResponseCode);
  if(httpResponseCode == 200){
    String response = client.getString();
    Serial.println(response);
    Serial.println("Guardado en DB");
  }else{
    Serial.println("Error en post de la db");
  }
  client.end();
  }

//Formato de respuesta: perimetro, area, porcentaje azul, porcentaje verde, porcentaje rojo
void clasify(String response){
  char separador = ',';
  String calidad;
  float res[6];
  int inicio, fin, i;
  inicio=0;
  i=0;
  fin=response.indexOf(separador,inicio);
  while (fin!=-1) {
    res[i] = response.substring(inicio, fin).toFloat(); 
    Serial.println(res[i]);
    inicio = fin+1;
    fin = response.indexOf(separador, inicio);
    i++;
  }
  res[i] = response.substring(inicio, response.length()).toFloat();
  Serial.println(res[i]);
  calidad = calidadFruta(res[0], res[1], res[2], res[3], res[4]);
  delay(100);
  response = response + calidad;
  saveOnDb(response);
  }

void desechar(){
  Serial.println("Desechado");
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);

  servo1.write(0);
  servo2.write(180);
  delay(1500);
  servo2.write(0);
  }

void procederProceso(int calidad){
  Serial.print("Procede con codigo ");
  Serial.println(calidad);
  Calidad = calidad;
  digitalWrite(pulsoCalidad, Calidad);
  
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo2.write(0);
  servo1.write(90);
  delay(1500);
  servo1.write(0);
  }

void setup()
{
  /* Inicializacion puerto Serial */
  Serial.begin(115200);
  pinMode(infraTest, INPUT_PULLUP);
  pinMode(pulsoCalidad, OUTPUT);

  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo1.write(0);
  servo2.write(0);
  /* Intento coneccion a la red local */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  /* Inicializacion de almacenamiento SPIFFS */
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }
  /* Coneccion exitosa a la red local */
  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  /* set en 0 de brownout detector */
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  cameraInit();
  Serial.print("Camara conectada");


}

void loop()
{   
    Captura = !digitalRead(infraTest);
    if(Captura){
      activador();
      delay(500);
      }
}
