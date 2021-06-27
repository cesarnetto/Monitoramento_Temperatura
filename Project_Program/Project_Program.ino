#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTTYPE DHT22 // DHT 22 (AM2302)
#define DHTPIN 4 // GPIo4 ou porta D2 do NodeMCU
DHT dht(DHTPIN, DHTTYPE, 11);

//float  h; //variáveis que armazerão temperatura e umidade lidas do DHT22
float t; //variáveis que armazerão temperatura

String apiWritekey = "D6Q7HB1EKIX3BAAI"; // substitua com a sua chave THINGSPEAK WRITEAPI aqui
const char* ssid = "KROSSCMA"; // seu nome SSID wifi
const char* password = "kross4444" ;// senha do wifi
 
const char* server = "api.thingspeak.com";
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu conectado a wifi...");
  Serial.println(ssid);
  Serial.println();
}
 
void loop() {
  
  float t = dht.readTemperature(); //Chamada de leitura do sensor DHT22
  //float h = dht.readHumidity();
  
  if (client.connect(server,80))
  {  
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(t);
           tsData += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // Os 2 retornos de carro indicam o fechamento dos campos de cabeçalho e o início dos dados
     client.print(tsData);
 
     Serial.print("Temperatura: ");
     Serial.print(t);
     Serial.println("Enviado para o servidor Thingspeak....");
  }
  client.stop();
 
  Serial.println("Aguardando para fazer upload da próxima leitura...");
  Serial.println();
  // thingspeak precisa de um atraso mínimo de 15 s entre as atualizações
  delay(15000);
}
