#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor

#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D6
DHT dht(dht_dpin, DHTTYPE); 
#define sensorPin1 0

char ssid[] = "hiyahiyapengenwifi";                 // Network Name
char pass[] = "omegalul123";                 // Network Password
byte mac[6];
float datasensor[10];
unsigned long tim;

WiFiServer server(80);
WiFiClient client;
MySQL_Connection conn((Client *)&client);

char INSERT_SQL[] = "INSERT INTO rm1pHfBF1b.GiriAS_07111540000118(humidity, temperature) VALUES (%f,%f)";
char query[128];

IPAddress server_addr(37,59,55,185);          // MySQL server IP
char user[] ="rm1pHfBF1b";                    // MySQL user
char password[] ="nSZ7rkF04y";                // MySQL password

void setup() {

  Serial.begin(9600);

  pinMode(sensorPin1, INPUT);

  Serial.println("Initialising connection");
  Serial.print(F("Setting static ip to : "));
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
  Serial.println("");
  Serial.print("Assigned IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  Serial.println("Connecting to database");
  


  while (conn.connect(server_addr, 3306, user, password) != true) {
    delay(1000);
    Serial.print ( "." );
  }

  Serial.println("");
  Serial.println("Connected to SQL Server!");  

   //SENSOR HUMIDITY SETUP
  dht.begin();
  Serial.println("Humidity and temperature \n\n");
  delay(700);
  
}

void loop() {
  datasensor[0]++;
  datasensor[1]++;
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  tim = millis();
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  for(int b=0;b<1;b++)
  {
      sprintf(query, INSERT_SQL, h,t);
      cur_mem->execute(query);
  }
  delete cur_mem;
  tim = millis()-tim;
  Serial.println(tim);
  delay(2000);
}
