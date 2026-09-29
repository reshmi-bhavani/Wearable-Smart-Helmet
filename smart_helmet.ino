#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "SmartHelmet";
const char* password = "YOUR_PASSWORD";

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int tilt = D5;
const int alcohol = D6;
const int motor = D7;
const int buzzer = D0;

ESP8266WebServer server(80);

String tiltStatus = "";
String motorStatus = "";
String alcoholStatus = "";

void handleWebpage();

void setup()
{
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("SMART HELMET AND");

  lcd.setCursor(0, 1);
  lcd.print("FALL SAFETY SYSTEM");

  delay(2000);
  lcd.clear();

  pinMode(tilt, INPUT);
  pinMode(alcohol, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Wi-Fi access point
  WiFi.softAP(ssid, password);

  Serial.println("wifi app started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Web routes
  server.on("/", handleWebpage);

  server.on("/status", []()
  {
    String json = "{";
    json += "\"tilt\":\"" + tiltStatus + "\",";
    json += "\"alc\":\"" + alcoholStatus + "\",";
    json += "\"mot\":\"" + motorStatus + "\"";
    json += "}";

    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop()
{
  server.handleClient();

  // Read sensors
  int tiltValue = digitalRead(tilt);

  tiltStatus = (tiltValue == HIGH) ? "ALERT" : "SAFE";

  int alcoholValue = digitalRead(alcohol);

  alcoholStatus = (alcoholValue == LOW) ? "DRUNK" : "SAFE";

  if (tiltValue == 0 && alcoholValue == 1)
  {
    digitalWrite(motor, HIGH);
    motorStatus = "IGNITED";

    digitalWrite(buzzer, LOW);
  }
  else
  {
    digitalWrite(motor, LOW);
    motorStatus = "IGNITION KILLED!";

    digitalWrite(buzzer, HIGH);
  }

  lcd.setCursor(0, 0);
  lcd.print("BIKE:");
  lcd.print(tiltStatus);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print("PERSON:");
  lcd.print(alcoholStatus);
  lcd.print(" ");
}

void handleWebpage()
{
  String html = "<!DOCTYPE html><html><head>";

  html += "<meta charset='utf-8'>";
  html += "<title>VEHICLE SAFETY</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";

  html += "<style>";

  html += "body{margin:0; font-family:'Segoe UI',Arial,sans-serif; "
          "text-align:center; "
          "background:linear-gradient(180deg,#f1f5eb 0%,#accc83 100%); "
          "background-attachment:fixed; min-height:100vh; "
          "padding:20px; color:#2f3e1c;}";

  html += "h1{color:#1e4620; margin-bottom:30px; "
          "text-shadow:1px 1px 2px rgba(255,255,255,0.5)}";

  html += ".status-box{font-size:20px; font-weight:600; "
          "margin:15px auto; padding:20px; border-radius:15px; "
          "max-width:400px; background:rgba(255,255,255,0.5); "
          "backdrop-filter:blur(5px); "
          "box-shadow:0 4px 15px rgba(0,0,0,0.05); "
          "transition:all 0.4s ease; border-left:8px solid #ccc;}";

  html += ".status-box:hover{transform:scale(1.03); "
          "box-shadow:0 8px 20px rgba(0,0,0,0.1)}";

  html += "</style></head><body>";

  html += "<h1>WEARABLE SMART HELMET WITH FALL DETECTION AND ALERT SYSTEM</h1>";

  // Status boxes
  html += "<div id='tilt' class='status-box'>Bike: "
          + tiltStatus + "</div>";

  html += "<div id='alc' class='status-box'>Person: "
          + alcoholStatus + "</div>";

  html += "<div id='mot' class='status-box'>Engine: "
          + motorStatus + "</div>";

  // JavaScript for automatic update
  html += "<script>";

  html += "function updateStatus(){";

  html += "fetch('/status').then(r=>r.json()).then(data=>{";

  // Bike status
  html += "let tB=document.getElementById('tilt');"
          "tB.innerText='Bike: '+data.tilt;";

  html += "if(data.tilt=='SLIPPED'){"
          "tB.style.background='rgba(255,200,200,0.8)';"
          "tB.style.borderColor='#dc3545';"
          "tB.style.color='#721c24';"
          "}else{"
          "tB.style.background='rgba(255,255,255,0.6)';"
          "tB.style.borderColor='#28a745';"
          "tB.style.color='#155724';"
          "}";

  // Person/alcohol status
  html += "let hB=document.getElementById('alc');"
          "hB.innerText='Person: '+data.alc;";

  html += "if(data.alc=='REMOVED'){"
          "hB.style.background='rgba(255,200,200,0.8)';"
          "hB.style.borderColor='#dc3545';"
          "hB.style.color='#721c24';"
          "}else{"
          "hB.style.background='rgba(255,255,255,0.6)';"
          "hB.style.borderColor='#28a745';"
          "hB.style.color='#155724';"
          "}";

  // Engine status
  html += "let mB=document.getElementById('mot');"
          "mB.innerText='Engine: '+data.mot;";

  html += "if(data.mot=='IGNITED'){"
          "mB.style.background='rgba(255,255,255,0.6)';"
          "mB.style.borderColor='#28a745';"
          "mB.style.color='#155724';"
          "}else{"
          "mB.style.background='rgba(200,200,200,0.4)';"
          "mB.style.borderColor='#6c757d';"
          "mB.style.color='#343a40';"
          "}";

  html += "});}";

  html += "setInterval(updateStatus,3000);";

  html += "</script></body></html>";

  server.send(200, "text/html", html);
}
