#include <SoftwareSerial.h>                        
SoftwareSerial esp8266(10,11);                   
#define SpeedS 115200              
#define DEBUG true       
#define LED 8                          
#define SERVER true

long int time_before = millis() + 4000; 
int contador = 0;
void setup()

{
  Serial.begin(SpeedS);           
  esp8266.begin(SpeedS);     
  pinMode(LED, OUTPUT);
  init_config();   
  
                            
}

void loop()                                                         
{


  //if((time_before+6000) > millis()){

      //Serial.print("Teste de connn.... ");
      //sendData("AT+CWJAP?\r\n", 2000, DEBUG);
       //esp8266.print("AT+CWJAP?\r\n");

       //time_before = millis(); 
    
  //}
  
    
  
  if(esp8266.available())                                           
 {    
   
      if(esp8266.find("+IPD,")){
    
        int connectionId = esp8266.read()-48;
       
        String response = "";
           contador = 0;
           
         while(esp8266.available())                                      
          {
            char c = esp8266.read();                                     
            response+=c;    
                                                       
          }  
          
           if(response.indexOf("/zdbbyrs") != -1){
    
            Serial.print("Entrou no IF");
    
           
            digitalWrite(LED, HIGH);
            delay(1000);
            digitalWrite(LED, LOW);
    
             String closeCommand = "AT+CIPCLOSE="; 
            closeCommand+=connectionId; 
            closeCommand+="\r\n"; 
            esp8266.print(closeCommand);
            delay(1000);
    
          }
    
           
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


  if(response.indexOf("ERROR") != -1){

    init_config();
  }
    
   
      Serial.print(response);
   
       
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
  sendData("AT+CIPSERVER=1,76\r\n", 1500, SERVER);                                     

}
