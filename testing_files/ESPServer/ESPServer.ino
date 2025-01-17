/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
  https://randomnerdtutorials.com/esp8266-web-server/
  Moified by Dies Robotics Club to be simpler
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials, ssid is for wifi network
const char* ssid     = "brandeis_open";
//the password of the wifi network
const char* password = "";
//The title of the website
String title = "Deis Robotics Test Web Server";
//The possibilities on the button, no spaces
String possibleCodes [] = {"Drive_Forward", "Drive_Backward","Turn_Right","Turn_Left", "Stop_Robot"};
//the number of possibilities
int numCodes = sizeof(possibleCodes) / sizeof(possibleCodes[0]);


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//runs once
void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  wifiConnect();
}

//runs many times
void loop() {
  //this can't be a method becuase it would always print something
  int indexUsed = runServer();
  //if the website has been told to do something, return the string command
  if (indexUsed != -1) {
    Serial.println("I have been told to " + possibleCodes[indexUsed]);
  }
}

//displays the website with website
int displayWebsite(WiFiClient client) {

  setupHeader(client);
  styleButton(client);

  // Web Page Heading
  client.println("<body><h1>" + title + "</h1>");
  int indexUsed = displayBody(client);
  client.println("</body></html>");

  // The HTTP response ends with another blank line
  client.println();
  return indexUsed;
}
//displays the buttons of the website
int displayBody(WiFiClient client) {
 
  int index=getIndex();
  displayButtons(client);
  return index;
}

//display all the possible buttons
void displayButtons(WiFiClient client){
 
  for (int index=0;index<numCodes;index++){
    client.println("<p><a href=\"/" + possibleCodes[index] + "\"><button class=\"button button2\">" +  possibleCodes[index] + "</button></a></p>");     
  }  
  
}

//gets the command that was most recently called
int getIndex(){
  boolean found = false; //to see if code was correctly identified
  //iterate through all the possible errorcodes
  int index = 0;
  while (!found && index < numCodes - 1) {
    found = hasLink(possibleCodes[index]);
    if (!found) {
      index = index + 1;
    }
  }
  //use last elemn in array as default
  if (!found) {
    index = numCodes - 1;
  }  
  return index;
}
//returns whether the website is being accessed with a link
boolean hasLink(String url) {
  return (header.indexOf("GET /" + url) >= 0);
}
//runs all the logic for the server
int runServer() {
  WiFiClient client = server.available();   // Listen for incoming clients
  int indexUsed = -1;
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            indexUsed = displayWebsite(client);
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
  return indexUsed;
}

//sets up the header of the website
void setupHeader(WiFiClient client) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");

}
//uses CSS to style the button 
void styleButton(WiFiClient client) {

  // CSS to style the on/off buttons s
  // Feel free to change the background-color and font-size attributes to fit your preferences
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println("</style></head>");

}
//connects to the internet and displays releve=ant information
void wifiConnect() {
  Serial.print("MAC aress of this device:");
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to :");
  Serial.println(ssid);
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}
