/*******************************************************
  Utilisation d'un écran OLED avec ESP32, ESP8266 ou STM32
  (bibliothèque u8g2)
  Plus d'infos:
  https://electroniqueamateur.blogspot.com/2019/09/ecran-oled-sh1106-i2c-et-esp32-ou.html
  https://electroniqueamateur.blogspot.com/2019/11/ecran-oled-sh1106-i2c-et-stm32.html
  
********************************************************/

#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// U8G2_R0 ou U8G2_R2: mode paysage, U8G2_R1 ou U8G2_R3: mode portrait

/* Définition d'une image bitmap: logo du blog Électronique en Amateur
sauvegardé dans un fichier XBM, puis ouvert avec un éditeur de texte: */
static const unsigned char logoBitmap[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 
  0x80, 0x07, 0x00, 0x00, 0x00, 0x40, 0x0A, 0x00, 0x00, 0x00, 0x60, 0x18, 
  0x00, 0x00, 0x00, 0x20, 0x11, 0x00, 0x00, 0x00, 0x10, 0x22, 0x00, 0x00, 
  0x00, 0x10, 0x21, 0x00, 0x00, 0x00, 0x08, 0x42, 0x00, 0x00, 0x00, 0x08, 
  0x82, 0x00, 0x00, 0x00, 0x04, 0x81, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01, 
  0x00, 0x00, 0x02, 0x01, 0x01, 0x00, 0x00, 0x81, 0x07, 0x02, 0x00, 0x00, 
  0x61, 0x1B, 0x06, 0x00, 0x80, 0x18, 0x62, 0x04, 0x00, 0xC0, 0x07, 0x81, 
  0x0F, 0x00, 0xC0, 0x01, 0x00, 0x0E, 0x00, 0x60, 0x00, 0x02, 0x1C, 0x00, 
  0x80, 0x01, 0x02, 0x0E, 0x00, 0x40, 0x07, 0x81, 0x19, 0x00, 0xC0, 0x0C, 
  0xC2, 0x0E, 0x00, 0xC0, 0x33, 0x30, 0x07, 0x00, 0x80, 0xCD, 0xDF, 0x04, 
  0x00, 0x00, 0xB3, 0x27, 0x02, 0x00, 0x00, 0x62, 0x18, 0x03, 0x00, 0x00, 
  0x84, 0x07, 0x01, 0x00, 0x00, 0x0C, 0x83, 0x00, 0x00, 0x00, 0x08, 0x40, 
  0x00, 0x00, 0x00, 0x10, 0x62, 0x00, 0x00, 0x00, 0x20, 0x21, 0x00, 0x00, 
  0x00, 0x60, 0x12, 0x00, 0x00, 0x00, 0x40, 0x09, 0x00, 0x00, 0x00, 0x80, 
  0x0A, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

long randNumber;

char*  i;
WiFiClient espClient;
PubSubClient client(espClient);

void reconnectmqttserver() {
while (!client.connected()) {
Serial.print("Attempting MQTT connection...");
String clientId = "ESP8266Client-";
 clientId += String(random(0xffff), HEX);
if (client.connect(clientId.c_str())) {
Serial.println("connected");
  client.subscribe("value");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println(" try again in 5 seconds");
delay(5000);
}
}
}

void callback(char* topic, byte* payload, unsigned int length) {
  String MQTT_DATA = "";
  for (int i=0;i<length;i++) {
   MQTT_DATA += (char)payload[i];}
  Serial.print(MQTT_DATA);

}


void setup()
{
  i = "";
Serial.begin(9600);
  WiFi.disconnect();
  delay(1000);
  Serial.println("start");
   WiFi.begin("CASTELLOU28","Q4vW_Sk55!J18#FtQ_7*Yb?G0mfDrU50");
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("Connected");
    Serial.print("your ip is");
    Serial.print((WiFi.localIP().toString()));
    client.setServer("192.168.88.60", 1883);
    client.setCallback(callback);

  }
  u8g2.begin();
  u8g2.enableUTF8Print(); //nécessaire pour écrire des caractères accentués
}

void loop(void) {
   
  /********* On dessine le logo (bitmap défini plus haut) ***************/
  u8g2.clearBuffer(); // on efface ce qui se trouve déjà dans le buffer
  u8g2.drawXBMP( 2, 8, 34, 48, logoBitmap); // position, largeur, hauteur

  /************** On écrit "Électronique en amateur" *********************/
  u8g2.setFont(u8g2_font_7x13B_tf); // choix d'une police de caractère
  u8g2.setCursor(40, 25); // position du début du texte
  u8g2.print("ETC :"); // écriture de texte
  u8g2.setCursor(70, 35);
  u8g2.print("");
  u8g2.setCursor(55, 45);
  u8g2.print(MQTT_DATA);
  u8g2.sendBuffer();  // l'image qu'on vient de construire est affichée à l'écran

    if (!client.connected()) {
    reconnectmqttserver();
    }
    client.loop();

}
}