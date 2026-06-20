#include <WiFi.h>
#include <WebServer.h>

#define IR_PIN 27
#define LED_PIN 26

const char* ssid = "SALONI";
const char* password = "01010101";

WebServer server(80);

int intrusionCount = 0;
bool previousMotion = false;

void handleRoot()
{
  int motion = digitalRead(IR_PIN);

  String motionStatus;
  String ledStatus;
  String alertStatus;

  if (motion == LOW)
  {
    motionStatus = "DETECTED";
    ledStatus = "ON";
    alertStatus = "ALERT";

    digitalWrite(LED_PIN, HIGH);

    if (!previousMotion)
    {
      intrusionCount++;
      previousMotion = true;
    }
  }
  else
  {
    motionStatus = "NO MOTION";
    ledStatus = "OFF";
    alertStatus = "NORMAL";

    digitalWrite(LED_PIN, LOW);
    previousMotion = false;
  }

  String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Smart Surveillance Dashboard</title>
<meta http-equiv="refresh" content="1">

<style>

body{
  margin:0;
  background:#111827;
  color:white;
  font-family:Segoe UI, Arial, sans-serif;
}

.header{
  text-align:center;
  font-size:30px;
  font-weight:600;
  padding:20px;
  background:#0f172a;
  border-bottom:2px solid #374151;
}

.container{
  width:90%;
  margin:auto;
}

.card{
  background:#1f2937;
  margin-top:20px;
  padding:25px;
  border-radius:12px;
}

.row{
  display:flex;
  justify-content:space-between;
  padding:12px 0;
  border-bottom:1px solid #374151;
  font-size:22px;
}

.label{
  color:#d1d5db;
}

.value{
  font-weight:bold;
  color:#60a5fa;
}

.camera{
  text-align:center;
}

.camera img{
  width:90%;
  max-width:700px;
  border-radius:10px;
  border:2px solid #374151;
}

.footer{
  text-align:center;
  margin:20px;
  color:#9ca3af;
}

</style>
</head>

<body>

<div class="header">
SMART SURVEILLANCE DASHBOARD
</div>

<div class="container">

<div class="card">

<div class="row">
<span class="label">Motion Status</span>
<span class="value">MOTION_STATUS</span>
</div>

<div class="row">
<span class="label">LED Status</span>
<span class="value">LED_STATUS</span>
</div>

<div class="row">
<span class="label">Security Status</span>
<span class="value">ALERT_STATUS</span>
</div>

<div class="row">
<span class="label">Intrusion Count</span>
<span class="value">COUNT_VALUE</span>
</div>

</div>

<div class="card camera">

<h2>Live Camera Feed</h2>

<img src="http://172.25.75.208/stream">

</div>

</div>

<div class="footer">
ESP32 + ESP32-CAM Security Monitoring System
</div>

</body>
</html>
)rawliteral";

  page.replace("MOTION_STATUS", motionStatus);
  page.replace("LED_STATUS", ledStatus);
  page.replace("ALERT_STATUS", alertStatus);
  page.replace("COUNT_VALUE", String(intrusionCount));

  server.send(200, "text/html", page);
}

void setup()
{
  Serial.begin(115200);

  pinMode(IR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  Serial.print("Dashboard IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();

  Serial.println("Dashboard Started");
}

void loop()
{
  server.handleClient();
}