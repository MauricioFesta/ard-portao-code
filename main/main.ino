#include <SoftwareSerial.h>                        
SoftwareSerial esp8266(10,11);                   
#define SpeedS 115200              
#define DEBUG true       
#define LED 8                          
#define SERVER true

long int contador = 0;
void setup()

{
  Serial.begin(SpeedS);           
  esp8266.begin(SpeedS);     
  pinMode(LED, OUTPUT);
  init_config();   
  
                            
}

void loop()                                                         
{

 
  if((contador) > 1000000){


     sendData("AT+CWJAP?\r\n", 2000, DEBUG);
     sendData("AT+CIPSERVER=1,76\r\n", 1000, SERVER);  
         
      
  }




  if(esp8266.available())                                           
 {   

    
      if(esp8266.find("+IPD,")){

        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);

       
        int connectionId = esp8266.read()-48;
       
        //String response = "";
           
         //while(esp8266.available())                                      
          //{
            //char c = esp8266.read();                                     
            //response+=c;    
                                                       
          //}  

         // Serial.print("Response");
          //Serial.print("\n");
          //Serial.print(response);
          //Serial.print("\n");

              
      
          
          // if(response.indexOf("/FdGAKx3jeLws2hw") != -1){
    
             // Serial.print("Entrou no IF");
      
             
              //digitalWrite(LED, HIGH);
              //delay(1200);
             // digitalWrite(LED, LOW);
      
              //delay(1000);
    
        // }


          
               String closeCommand = "AT+CIPCLOSE="; 
              closeCommand+=connectionId; 
              closeCommand+="\r\n"; 
              esp8266.print(closeCommand);
    
           
        }
    }

      contador+= 1;

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


  if(response.indexOf("ERROR") != -1 || response.indexOf("No AP") != -1){
   
    init_config();
  }
    
   
      Serial.print(response);
      contador = 0;
      
   
       
    return response;                                                  
}

void init_config()
{

  Serial.print("Config Call..");
  sendData("AT+RST\r\n", 2000, DEBUG);                                                        
  sendData("AT+CWJAP=\"AP-02_2GHz\",\"riceferteste\"\r\n", 4000, DEBUG);      
  delay (3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);                                             
  delay (3000);
  sendData("AT+CIFSR\r\n", 1000, DEBUG);                                             
  delay (3000);
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);                                             
  delay (3000);
  sendData("AT+CIPSERVER=1,76\r\n", 1000, SERVER);                                     

}
