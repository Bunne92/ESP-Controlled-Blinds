#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define STEP_DELAY  2
#define STEPS       200
 
ESP8266WebServer Webserver(80);
 
// Replace with your network credentials
const char* ssid = "Your_Network_SSID";
const char* password = "Your_WiFi_Password";

//String to build your webpage
String HTMLpage = "";

/*Connection between stepper motor and ESP8266
 * in1 = GPIO4 
 * in2 = GPIO5
 * in3 = GPIO12
 * in4 = GPIO13
 */

int in1 = 4;
int in2 = 5;
int in3 = 12;
int in4 = 13;
 
void setup(void){
  //Output mode for stepper motor
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  HTMLpage += "<head><title>Blinds Automation</title></head>";
  HTMLpage += "<h3>ESP8266 Webserver Blinds Automation</h3>";
  HTMLpage += "<p>Blinds <a href=\"BlindsUp\"><button>UP</button></a>&nbsp;";
  HTMLpage += "<a href=\"BlindsDown\"><button>Down</button></a>";
  HTMLpage += "<a href=\"StopBlinds\"><button>Stop</button></a></p>";
   

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  if (MDNS.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
 
  Webserver.on("/", [](){
    Webserver.send(200, "text/html", HTMLpage);
  });
  Webserver.on("/BlindsUp", [](){
    Webserver.send(200, "text/html", HTMLpage+"<p>Blinds Up</p>");
    Forward(STEPS, STEP_DELAY);
    //delay(1000);
  });
  Webserver.on("/BlindsDown", [](){
    Webserver.send(200, "text/html", HTMLpage+"<p>Blinds Down</p>");
    Backward(STEPS, STEP_DELAY);
    //delay(1000); 
  });

 
  Webserver.begin();
  Serial.println("HTTP Webserver started");

  

}
 
void loop(void){
  Webserver.handleClient();
}


/************************************************************************************************************************************
 * This function makes the stepper motor move forward. 
 ***********************************************************************************************************************************/
void Forward(int steps, int step_delay){
  for(int i=0; i<=STEPS; i++){
    if(Webserver.send(200, "text/html", HTMLpage+"<p>Blinds Stop</p>")){
        Webserver.on("/StopBlinds", [](){
        Webserver.send(200, "text/html", HTMLpage+"<p>Blinds Stop</p>");
    Stop();
  });
    }
      digitalWrite(in1, HIGH);   digitalWrite(in2, LOW);    digitalWrite(in3, LOW);     digitalWrite(in4, LOW);     delay(step_delay);
      digitalWrite(in1, LOW);    digitalWrite(in2, HIGH);   digitalWrite(in3, LOW);     digitalWrite(in4, LOW);     delay(step_delay);
      digitalWrite(in1, LOW);    digitalWrite(in2, LOW);    digitalWrite(in3, HIGH);    digitalWrite(in4, LOW);     delay(step_delay);
      digitalWrite(in1, LOW);    digitalWrite(in2, LOW);    digitalWrite(in3, LOW);     digitalWrite(in4, HIGH);    delay(step_delay);
      Stop();
  }
}

/************************************************************************************************************************************
 * This function makes the stepper motor move backward. 
 ***********************************************************************************************************************************/
void Backward(int steps, int step_delay){
  for(int i=0; i<=steps; i++){
      digitalWrite(in1, LOW);   digitalWrite(in2, LOW);    digitalWrite(in3, LOW);     digitalWrite(in4, HIGH);    delay(step_delay);
      digitalWrite(in1, LOW);   digitalWrite(in2, LOW);    digitalWrite(in3, HIGH);    digitalWrite(in4, LOW);     delay(step_delay);
      digitalWrite(in1, LOW);   digitalWrite(in2, HIGH);   digitalWrite(in3, LOW);     digitalWrite(in4, LOW);     delay(step_delay);
      digitalWrite(in1, HIGH);  digitalWrite(in2, LOW);    digitalWrite(in3, LOW);     digitalWrite(in4, LOW);     delay(step_delay);
      Stop();
  }
}
/************************************************************************************************************************************
 * Turns off stepper motor.
 ***********************************************************************************************************************************/
void Stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}


