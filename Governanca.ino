
 
#include <SPI.h>
#include <Ethernet.h>
String readString;
 
//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F };
byte ip[] = { 10, 0, 0, 180 };
byte gateway[] = { 10, 0, 0, 1 };
//byte ip[] = { 10, 1, 1, 150 };
//byte gateway[] = { 10, 1, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };
int pinos[12] = {2,3,4,5,6,7,8,9,10,11,12,13};
EthernetServer server(80);
 
void setup()
{

  
 
  //Inicializa Ethernet Shield
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
 

 
  //Desliga os dois reles
  for(int i = 0; i < sizeof(pinos) - 1; i++) {
    pinMode(pinos[i], OUTPUT);
    digitalWrite(pinos[i], HIGH);
  }
  Serial.begin(9600);
}
 
void loop()
{
  EthernetClient client = server.available();
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == 'n')
        {
          //Controle do rele1
          Serial.println(readString);
          
          
          //Liga o Rele 1
          if (readString.indexOf("?") > 0)
          {
            String res = readString.substring(6,8);
            int lamp = res.substring(0,1).toInt();
            int statu = res.substring(1,2).toInt();
            digitalWrite(lamp, statu);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println("<html>");
            client.println("<head>");
            client.println("<title>Automacao Residencial</title>");
            client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
            client.println("</head>");
            client.println("<body>");
            client.println("</body>");
            client.println("</head>");
          }

          if(readString.indexOf("?_")  > 0) {
            int lamp = readString.substring(7,8).toInt();
            client.println("{\"");
            client.println(lamp);
            client.println("\":\"");
            client.println(!digitalRead(lamp));
            client.println("\"}");
            client.stop();
           }

          delay(1);

          readString = ""; 
          client.stop();
        }
      }
    }
  }
}
