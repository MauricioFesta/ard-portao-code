#include <SoftwareSerial.h>                        
SoftwareSerial esp8266(10,11);   
#include<stdio.h>
#include <string.h>                
#define SpeedS 115200              
#define DEBUG false       
#define LED 8                          
#define SERVER true
String HOST = "192.168.0.107";
String PORT = "80";
int countTimeCommand; 
int contador = 0;
int contador_command = 0;

int contador_str = 0;
void setup()

{
  Serial.begin(SpeedS);           
  esp8266.begin(SpeedS);     
  pinMode(LED, OUTPUT);
  init_config();   
  
                            
}

void loop()                                                         
{

 
  if((contador) > 4000){

     Serial.print("Entrou contador");
     sendData("AT+CWJAP?\r\n", 2000, DEBUG);
     sendData("AT+CIPSERVER=1,76\r\n", 1000, SERVER);  
     contador = 0;
         
      
  }

  //contador+=1;

 if(esp8266.available())                                           
 {    
   
      if(esp8266.find("+IPD,")){
    
        int connectionId = esp8266.read()-48;
       
        String response = "";
        contador = 0;
       
           
         while(esp8266.available())                                      
          {
            
              char c = esp8266.read();
              //char next = esp8266.read()+1;
              //Serial.print(c);
              //Serial.print("\n");
              
              if(contador_str > 10 && contador_str <= 23){

                response+=c;
              }

               contador_str+=1;
                                                      
          }  

             
               contador_str = 0;

              digitalWrite(LED, HIGH);
              delay(1000);
              digitalWrite(LED, LOW);

              String closeCommand = "AT+CIPCLOSE="; 
              closeCommand+=connectionId; 
              closeCommand+="\r\n"; 
              esp8266.print(closeCommand);
              int total = response.length()+ 66;
              Serial.print("Response: " + response);
              //response = "192.168.100.105";

              String getData = "GET /update/"+response+"-lWHgIfmZmGTJPNbdbTONZkANSNQK-62bf9dadde59f40bb7459ce4";
              sendCommand("AT+CIPMUX=1",8,"OK");
              sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,6,"OK");
              sendCommand("AT+CIPSEND=0,"+String(total),4,">");
              esp8266.println(getData);
              delay(3800);
              sendCommand("AT+CIPCLOSE=0",5,"OK");
                    
        }
    } 
      
   
     
}


void sendCommand(String command, int maxTime, char readReplay[]) {


  delay(2300);
  esp8266.println(command);
  Serial.println("Comando enviado");
  Serial.println("\n");


  //while(countTimeCommand < (maxTime*1))
  //{
    //esp8266.println(command);//at+cipsend
    //if(esp8266.find(readReplay))//ok
    //{
       ////Serial.println("OYI");
      //break;
    //}
  
    //countTimeCommand++;
  //}


  //countTimeCommand = 0;
  

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


    if(debug)
    {
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
  sendData("AT+CWMODE=1\r\n"  , 1500, DEBUG);                                             
  delay (1500);
  sendData("AT+CIFSR\r\n", 1500, DEBUG);                                             
  delay (1500);
  sendData("AT+CIPMUX=1\r\n", 1500, DEBUG);                                             
  delay (1500);
  sendData("AT+CIPSERVER=1,76\r\n", 1500, SERVER);                                      

}
