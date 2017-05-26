#include <ws2812.h>

#include <dummy.h>
#include <esp32-hal.h>

/*
Sprint Assistant MCU Code Revision Final, moved entirety of code to esp32 arduino core
- implemented web page over home network
- implemented speaker
- LED implemented
- noise conflict with speaker, speaker emits noise after LED is green

Built upon "SimpleWifiServer" Arduino IDE example: https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino
 
 */

#include <WiFi.h>


const uint16_t NUM_PIXELS = 16;
uint8_t MAX_COLOR_VAL = 150;

int i;

rgbVal *pixels;

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);


    pixels = (rgbVal*)malloc(sizeof(rgbVal) * NUM_PIXELS);
    
    ws2812_init(12, LED_WS2812);
    i = 0;
    for(i = 0; i < 16; i++){
          pixels[i] = makeRGBVal(150, 0, 0);
      }
      ws2812_setColors(16, pixels);
    

    delay(10);
    

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    i = 0;
    for(i = 0; i < 16; i++){
          pixels[i] = makeRGBVal(0, 0, 150);
      }
      ws2812_setColors(16, pixels);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();
    //dacWrite(25, 0);

}

int value = 0;
int iteration = 0; //counter to make sure it doesnt time multiple times
double displayTime; //time to display on web server



void loop(){
 WiFiClient client = server.available();   // listen for incoming clients
  
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            

            // the content of the HTTP response follows the header:
            client.print("Warmup started<br>");
            client.print("Sprint will begin in 30 seconds, wait for speaker to GO<br>");
            client.print("Click <a href=\"/H\">here</a> after sprinting to get your time<br>");
            
            if(iteration < 1){
              printf("sprint started, doing warmup then timing\n");
              double finalTime = 0.0;
              int ldr_value = 99999;
              double micro_to_sec = 1000000;
              double tempsec = 0;
              double tempusec = 0;
              double temptime = 0;
              double start = 0;
              double temp = 0;
              double time_warmed_up = 0.0;
  
              struct timeval tv;
  
              //printf("warmup time\n");
              i = 0;
              for(i = 0; i < 16; i++){
                pixels[i] = makeRGBVal(150, 0, 0);
              }
              ws2812_setColors(16, pixels);
  
              adcStart(35);
  
  
  
              //printf("got time of day \n");
              gettimeofday(&tv, NULL);
              tempsec = tv.tv_sec;
              tempusec = tv.tv_usec/micro_to_sec;
              start = tempsec + tempusec;
              while(1){
                
                gettimeofday(&tv, NULL);
                tempsec = tv.tv_sec;
                tempusec = tv.tv_usec/micro_to_sec;
                temp = tempsec + tempusec;
                temptime = temp - start;
  
                //printf("IN WARMUP LOOP\n");
                //printf("timer is at %.3lf\n", temptime);
  
                if(temptime >= 30){//insert logic to leave loop here
                time_warmed_up = temptime;
                printf("warmup ended at time: %.3lf\n", time_warmed_up);
                temptime = 0;
                temp = 0;
                start = 0;
                break;
              }
            }
  
              // TIMING
            int counter = 0;
            printf("timing\n");
            i = 0;
            for(i = 0; i < 16; i++){
                pixels[i] = makeRGBVal(0, 150, 0);
            }
            ws2812_setColors(16, pixels);
            gettimeofday(&tv, NULL);
            tempsec = tv.tv_sec;
            tempusec = tv.tv_usec/micro_to_sec;
            start = tempsec + tempusec;
            //make led green
            //speaker
            //tone(8, 330, 1000);
            while(1) {

              if(counter < 1000){
                dacWrite(25, 255);
                delay(1);
                dacWrite(25, 0);
                delay(1);

              }

              
              //printf("IN TIMING LOOP\n");
              ldr_value = analogRead(34);
              printf("LDR value: %d\n", ldr_value);
          
              gettimeofday(&tv, NULL);
              tempsec = tv.tv_sec;
              tempusec = tv.tv_usec/micro_to_sec;
              temp = tempsec + tempusec;
              temptime = temp - start;
          
              if(ldr_value > 1500){ //TWEAK VALUE HERE FOR LDR
                finalTime = temptime;
                displayTime = finalTime;
                int counter_two = 0;
                while(counter_two < 250){
                  dacWrite(25, 255);
                  delay(2);
                  dacWrite(25, 0);
                  delay(2);
                  counter_two++;

                  }
                printf("Sprint Completed \nFinal Time: %.3lf seconds \n", finalTime);
                i = 0;
                for(i = 0; i < 16; i++){
                  pixels[i] = makeRGBVal(0, 0, 200);
                }
                ws2812_setColors(16, pixels);
                
                break;
              }
  
  
      else if(temptime >= 60){
        printf("TIMEOUT REACHED at time %.3lf s, BREAKING \n", temptime);
        i = 0;
        for(i = 0; i < 16; i++){
          pixels[i] = makeRGBVal(0, 0, 0);
        }
        ws2812_setColors(16, pixels);
        break;
  
      }
      counter++;
    }
    iteration++;
   }

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        
      
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          client.print("Sprint Completed, Final Time in seconds: ");
          client.print(displayTime);
          delay(100);
          client.println();
          client.println();
          client.println();
          client.println();
          client.println();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}
