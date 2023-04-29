#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include <ThingSpeak.h>

unsigned long channelID = 2128274;  // ID do canal
const char * writeAPIKey = "KECDKB3J9ECBPGKX";  // Chave de escrita do canal


#define DHTPIN 2          // pino do sensor DHT
#define DHTTYPE DHT22     // tipo de sensor DHT
#define ATUADOR1 33      // pino do atuador 1
#define ATUADOR2 32       // pino do atuador 2

DHT dht(DHTPIN, DHTTYPE);
WiFiClient Client;

LiquidCrystal lcd(13, 12, 14, 27, 26, 25);  // pinos RS, EN, D4, D5, D6, D7

void setup() {
  Serial.begin(9600);
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Conectado!");

  ThingSpeak.begin(Client);

  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(ATUADOR1, OUTPUT);
  pinMode(ATUADOR2, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Sistema de controle");
  lcd.setCursor(0, 1);
  lcd.print("de ambiente");

  delay(2000);
  lcd.clear();
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, umidade);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Dados enviados com sucesso para o Thingspeak");
  } else {
    Serial.println("Erro ao enviar dados para o Thingspeak");
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
 
  lcd.print(temperatura);
  lcd.print((char)223); // símbolo de grau
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Umi: ");
  lcd.print(umidade);
  lcd.print("%");

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" ");
  Serial.print("Umidade: ");
  Serial.println(umidade);

  if (temperatura > 35) {
    digitalWrite(ATUADOR1, HIGH);
    lcd.setCursor(10, 0);
    lcd.print("A1 ON ");
  } else {
    digitalWrite(ATUADOR1, LOW);
    lcd.setCursor(10, 0);
    lcd.print("A1 OFF");
  }

  if (umidade > 70) {
    digitalWrite(ATUADOR2, HIGH);
    lcd.setCursor(10, 1);
    lcd.print("A2 ON ");
  } else {
    digitalWrite(ATUADOR2, LOW);
    lcd.setCursor(10, 1);
    lcd.print("A2 OFF");
  }

  delay(5000); // espera 5 segundos antes de atualizar os valores
}


