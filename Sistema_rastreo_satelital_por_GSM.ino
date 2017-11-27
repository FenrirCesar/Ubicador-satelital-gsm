#include <SoftwareSerial.h>
#include <TinyGPS.h>
SoftwareSerial gsm(7, 8); // RX, TX
TinyGPS gps;
SoftwareSerial ss(4, 3);  // RX, TX
char numero_cell[]="985819817";//meter numero de telefono
int estado=13;
int led =12;
int i=0;
int j=0;

float lati,longi=0;
char DAT;
char DAT_dos;
char DAT_GPS;
float flat, flon;
unsigned long age;
char coordenada_gps;
char datosSERIAL[30];
//char clave_uno[]= "LEDON"; //OTRA FORMA DE USAR UN ARREGLO
char clave_uno[]={'L','E','D','O','N'};//AQUI CAMBIAMOS TODO EL MENSAJE DE CONTROL
char clave_dos[]={'L','E','D','O','F'};//AQUI CAMBIAMOS TODO EL MENSAJE DE CONTROL
char clave_gps[]={'G','P','S'};
void setup()
{
  
  pinMode(led,OUTPUT);
  pinMode(estado,OUTPUT);
  Serial.begin(9600);
  gsm.begin(9600);
  
  delay(2000);
  configuracion_inicial();
  Serial.println("sistema de alarma GPS + SIM900 encendida");
 
  delay(1000);
}
/////////////////////////
void loop()
{
 
Serial.println("esperando recibir mensaje...");
while(true)
{
  leer_mensaje(); 
}
 
}
///////////////////////
void configuracion_inicial()//configura los codigos de lectura de mensajes
{
  gsm.println("AT+IPR=9600");
  Serial.println("AT+IPR=9600");
  delay(500);
  gsm.println("AT+CMGF=1");//modo texto
  Serial.println("AT+CMGF=1");//modo texto
  delay(500);
  gsm.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
  Serial.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
 delay(500);
  gsm.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  Serial.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
  
  delay(500);
  Serial.println("configuracion terminada");
  delay(300);
  /////////////////activar gps desde el inicio
  gsm.println("AT+CGNSPWR=1");// activo gps
  Serial.println("AT+CGNSPWR=1");// activo gps
 // gsm.println("AT+CGNSPWR=1");// activo gps
  //Serial.println(" AT + CGNSTST =1");
   delay(400);
    
}
///////////////////
void leer_mensaje()
{
  salir:
    if (gsm.available()>0)
    {
     
      char DAT =gsm.read();
      if(DAT=='@')//el arroba hace detectar el inicio del codigo
      {//If arroba
      digitalWrite(estado,HIGH);//led de testeo de mensaje
      Serial.println("llego dato");//para verificar si lo detecto
      //////////////////////
      while(true)//ingresa en un while para leer solo los codigos futuros que estan por llegar despues de la arroba
      {
          if (gsm.available()>0)
           {//cierre del segundo if
               char DAT_dos =gsm.read();//@LEDON enter
               datosSERIAL[j]= DAT_dos;//almacena en cadena de caracteres, suma de caracteres     
               j++;
               //////////////////////////
               if (DAT_dos=='\n')//cuando termine de entregar todos los datos dara un enter
               //garantizando el final del codigo
                {
                 
                 Serial.println("Lectura correcta del codigo enviado:");//IMPRIME LOS CARACTERES ALMACENADOS PARA VER SI TODO ESTA OK
                 for(int i=0;i<=j;i++)
                  {
                 Serial.print(datosSERIAL[i]);//IMPRIME TODO EL CODIGO GUARDADO EN EL ARRAY
                  }
                  led_on();//llama a la la funcion y verifica codigo
                  led_off();//llama ala funcion para ver si es de apagar el led
                  gps_on();//verifica si es codigo de activacion de gps
                  digitalWrite(estado,LOW); //apaga el led despues de verificar
                  
                  for(int i=0;i<=j;i++)
                  {
                   datosSERIAL[i]==0;//borro array
                   DAT_dos=0;
                   DAT=0;
                  }
                  j=0;//borra el puntero o acumulador si no se hace esto no detecta los sigueintes codigos
                  goto salir;//sale de todos los ciclos y va al inicio para volver a leer codigo
                 }//CIERRA AL /N
               ////////////////////////     
            } //cierre del segundo if
          }//while
       }//arroba
    
    }//serial available 
  }  


void led_off()
{
  /////////////////////////
  if(datosSERIAL[0]==clave_dos[0] && datosSERIAL[1]==clave_dos[1] && datosSERIAL[2]==clave_dos[2] && datosSERIAL[3]==clave_dos[3] && datosSERIAL[4]==clave_dos[4])
   {
    digitalWrite(led,LOW);
    Serial.println(" LED  DESACTIVADO.....");
    mensaje_apagado();
   }
   else Serial.println(" codigo incorrecto led off");
                  
  
}
/////////////////////////////////
//////////////////

void led_on()
{
  /////////////////////////
  if(datosSERIAL[0]==clave_uno[0] && datosSERIAL[1]==clave_uno[1] && datosSERIAL[2]==clave_uno[2] && datosSERIAL[3]==clave_uno[3] && datosSERIAL[4]==clave_uno[4])
   {
    digitalWrite(led,HIGH);
    Serial.println(" LED ACTIVADO.....");
    mensaje_encendido();
   }
   else Serial.println(" codigo incorrecto led on");               
  
}
/////////////////////////////////
//////////////////

void gps_on()
{
  /////////////////////////
  if(datosSERIAL[0]==clave_gps[0] && datosSERIAL[1]==clave_gps[1] && datosSERIAL[2]==clave_gps[2] )
   {
    Serial.println(" codigo correcto gps");
    activacion_gps();
    mensaje_gps_coordenada();//envia las variables de latitud y longitud por gsm al celular programado
     Serial.println(" lectura gps enviada....");
      Serial.print("LATtt= ");
      Serial.println(lati);
      Serial.print("LONnn= ");
      Serial.println(longi);
    
   }
   else Serial.println(" codigo incorrecto gps");
                  
  
}
/////////////////////////////////
//////////////////
    void mensaje_apagado()
    {
      gsm.println("AT+CMGF=1");//modo texto 
      //Imprime los datos al puerto serie como texto ASCII seguido de un retorno de carro
      delay(1000);
      gsm.print("AT+CMGS=");// comando de envio de mensaje a un numero determinado
      delay(1000);
      //Imprime los datos al puerto serie como texto ASCII
      gsm.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
      gsm.print(985819817);//colocamos numero de telefono
      gsm.println((char)34);//volvemos a poner el caracter "
      delay(1000);//tiempo para que de respuesta el modulo >
      gsm.print("led apagado");//mensaje que enviare
      gsm.print((char)26);//ponemos el simbolo ascii 26,que corresponde a CTRL+Z,con lo que el modulo sabe que el sms termino
      
      Serial.print("led apagado");//mensaje que enviare
    }
    ///////////////////////////////////
    /////////////////////////////////


    void mensaje_encendido()
    {
      gsm.println("AT+CMGF=1");//modo texto 
      //Imprime los datos al puerto serie como texto ASCII seguido de un retorno de carro
      delay(1000);
      gsm.print("AT+CMGS=");// comando de envio de mensaje a un numero determinado
      delay(1000);
      //Imprime los datos al puerto serie como texto ASCII
      gsm.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
      gsm.print(985819817);//colocamos numero de telefono
      gsm.println((char)34);//volvemos a poner el caracter "
      delay(1000);//tiempo para que de respuesta el modulo >
      gsm.print("led encendido");//mensaje que enviare
      gsm.print((char)26);//ponemos el simbolo ascii 26,que corresponde a CTRL+Z,con lo que el modulo sabe que el sms termino
      
      Serial.print("led encendido");//mensaje que enviare
    }
    ///////////////////////////////////
    /////////////////////////////////


    void mensaje_gps_coordenada()
    {
      gsm.println("AT+CMGF=1");//modo texto 
      //Imprime los datos al puerto serie como texto ASCII seguido de un retorno de carro
      delay(1000);
      gsm.print("AT+CMGS=");// comando de envio de mensaje  a un numero determinado
      delay(1000);
      //Imprime los datos al puerto serie como texto ASCII
      gsm.print((char)34);//ponemos las comillas ", para que lo tome debe ser char de lo contrario el serial envia caracter por caracter
      gsm.print(985819817);//colocamos numero de telefono
      gsm.println((char)34);//volvemos a poner el caracter "
      delay(1000);//tiempo para que de respuesta el modulo >
      gsm.print("LAT= ");
      gsm.print(lati);
      gsm.print("LON= ");
      gsm.print(longi);
      gsm.print((char)26);//ponemos el simbolo ascii 26,que corresponde a CTRL+Z,con lo que el modulo sabe que el sms termino
      delay(200);
      lati,longi=0;//borro los datos de la variable
      
    }
    ///////////////////////////////////
    void activacion_gps()
    {
      gsm.println("AT+CGNSPWR=1");// activo gps
      Serial.println("AT+CGNSPWR=1");// activo gps
      delay(400);
      Serial.println("por favor espere 10 segundos estabilizando GPS...");//mensaje para indicar que se esta activando el gps
      delay(10000);//espero x segundos para esperar que el gps se estabilice y tome datos del satelite
      Serial.println("empezando lectura y conversion");
      
      tyni_gps_leer();//leer cordenadas por medio de las librerias y las convierte a latitud longitud lamacenando los datos en dos variables
     /* 
      while(1)//toma 3 veces los datos del puertos serial
      {
      solo_lectura_gps();//nunca sale de este ciclo   
      
      }  
      */   
      //gsm.println("AT+CGNSPWR=0");// apago gps
      //Serial.println("gps apagado");
      delay(400);
      
    }
   ////////////////////////////////////////////////////
   /// leer cordenadas con libreria tyni
   void tyni_gps_leer()
   {
    i=0;
    while(i<5)
    {
     i++;
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gsm.available())
    {
      char c = gsm.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // ¿Entró una nueva sentencia válida?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
     gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    lati = flat;
    Serial.print("LON=");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    longi = flon;

   }
  
}
i=0;
}

void solo_lectura_gps() // run over and over
{

    if(gsm.available())
    Serial.print((char)gsm.read());

}
