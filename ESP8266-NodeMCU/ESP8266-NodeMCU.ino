#include <ESP8266WiFi.h>;
#include <WiFiClientSecure.h>;

const char* ssid = "Skynet"; //Your Network SSID
const char* password = "24081999"; //Your Network Password

const int httpPort = 443;
String host = "script.google.com"; 
  
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected...");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { // run over and over
  if (Serial.available()>0) {
    String content=Serial.readStringUntil('\n');
    Serial.println(content);
    String SfingerID = getValue(content, ',', 0);
    String Sconfidence = getValue(content, ',', 1);
    int fingerID=SfingerID.toInt();
    int confidence=Sconfidence.toInt();

WiFiClientSecure client;
client.setInsecure();
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
  }
  //https://script.google.com/macros/s/AKfycbzT0FpcLk44l2-3uZO-1rgUIjMh9FXzzdZ1lWCDtm73_dM6sHs/exec?Value1=1&Value2=Hello
  String url = "/macros/s/AKfycbzT0FpcLk44l2-3uZO-1rgUIjMh9FXzzdZ1lWCDtm73_dM6sHs/exec?Value1="+SfingerID+"&Value2="+Sconfidence;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  String section="header";
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  delay(6000);





    

  }
}
String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
