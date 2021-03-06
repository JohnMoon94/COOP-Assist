#include <ESP8266WiFi.h>

const char* ssid = "Chuan6";
const char* password = "12341234";

int ledPin = 2;
WiFiServer server(80);

// Status of the LED
int value = HIGH;
  
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request


  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, LOW);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = LOW;
  }
  if (request.indexOf("/LED=SWT") != -1)  {
    if(value == LOW) {
      digitalWrite(ledPin, LOW);
      value = HIGH;
    }
    else {
      digitalWrite(ledPin, HIGH);
      value = LOW;     
    }
  }

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=2\">");
  client.println("<meta http-equiv=\"refresh\" content=\"5; URL=/LED\">");
  client.println("</head");
  
  client.println("<body>");
  client.print("Led pin is now: ");

  if (value == HIGH) {
    client.print("<font color=\"green\"<b>On</b></font>");
  } else {
    client.print("<font color=\"red\"<b>Off</b></font>");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}

