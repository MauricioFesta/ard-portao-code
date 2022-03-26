#include <SoftwareSerial.h>                        
SoftwareSerial esp8266(10,11);                   
#define SpeedS 115200              
#define DEBUG true       
#define LED 8                          

void setup()

{
  Serial.begin(SpeedS);           
  esp8266.begin(SpeedS);     
  pinMode(LED, OUTPUT);
  init_config();   
                            
}

void loop()                                                         
{

  if(esp8266.available())                                           
 {    
    if(esp8266.find("+IPD,"))
    {
     delay(1000);
 
     int connectionId = esp8266.read()-48;                                                
     String webpage = "<h1>ok</h1>";
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
 
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; 
     closeCommand+="\r\n";    
     sendData(closeCommand,3500,DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";                                             
    esp8266.print(command);                                          
    long int time = millis();                                      
    while( (time+timeout) > millis())                                 
    {      
      while(esp8266.available())                                      
      {
        char c = esp8266.read();                                     
        response+=c;                                                  
      }  
    }    
    if(debug)                                                        
    {

      if(response.indexOf("GET /api/dbbygqwfjcbozcwdqukrykxlcldgpqksrlq") != -1){

       
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
       
        

      }
      Serial.print(response);
      
    }    
    return response;                                                  
}

void init_config()
{

  Serial.print("Config Call..");
  sendData("AT+RST\r\n", 2000, DEBUG);                                                  
  sendData("AT+CWJAP=\"AP-02_2GHz\",\"riceferteste\"\r\n", 2000, DEBUG);        
  delay (3000);
  sendData("AT+CWMODE=1\r\n", 1500, DEBUG);                                             
  delay (1500);
  sendData("AT+CIFSR\r\n", 1500, DEBUG);                                             
  delay (1500);
  sendData("AT+CIPMUX=1\r\n", 1500, DEBUG);                                             
  delay (1500);
  sendData("AT+CIPSERVER=1,80\r\n", 1500, DEBUG);                                     

}
