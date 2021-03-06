
///////////////////////////////////////////////////////////////////////////////////////////
//     Respirateur numérique utilisant un compresseur ou une bouteille d'air comprimé    //
// Pouvant assuré la respiration de une à trois poste pour aération avec un compresseur  //
//                                                                                       //
//                              Auteur : Andrzej kmita                                   //
//                          Coopération: Krzysztof Klopotowski                           //
//                                       Damian Karpiuk                                  //
//                                                                                       //
// Le programme présenté permet l'utilisation de n'importe quel compresseur ou cylindre  //
//                        avec de l'air comprimé comme respirateur.                      //
//                                                                                       //
//                                Le système permet:                                     //
//                                                                                       //
//    1. Régulations de la durée d'inspiration et d'expiration de 0,1 à 30 seconde       //
//                                                                                       //
//    2. régulation de la pression d'air inhalé à partir de 0,1 jusqu'à 250 hPa.         //
//                                                                                       //
//    3. Réglages de la limite de pression supérieur à laquelle une alarme sonore        //
//       et visuel est activé                                                            //
//                                                                                       //
//    4. Réglage indépendant de chaque ligne séparément dans la gamme complète           //
//       des paramètres liste.                                                           //
//                                                                                       //
//    5. Changement des paramètres individuels de chaque ligne pendant le déroulement    //
//       de la respiration                                                               //
//                                                                                       //
//    6. Le système est en outre équipé d'un programme informant de l'éventuelle         //
//       défaillance de sonde de pression individuelle avec station automatique des      //
//       des vannes sur ouverture. Pour fournir l'air  des environs.                     //
//       En même temps en quadrille ma liste d'alarmes ou de défaillance de lignes       //
//       individuelle à sur le fonctionnement indépendance des autres lignes             //
//       en réglant les vins de la ligne endommagé à souffler fermer échappement         //
//       ouverture lol.                                                                  //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////
   #include <OneWire.h>
   #include "Wire.h"
   #include <Adafruit_BMP280.h>
   #include <SPI.h>
   #include <Adafruit_Sensor.h>
 
   #define PRZYCISK1 A4
   #define PRZYCISK2 5
   #define PRZYCISK3 6
   #define PRZYCISK4 A5
   #define KRAN1 A3
   #define KRAN2  2
   #define KRAN3  3
   #define LED4  A0
   #define LED5  A1
   #define LED6  A2
   #define ALARM  4  
   #define BMP_SCL 13
   #define BMP_SDO 12
   #define BMP_SDA 11
   #define BMP_CSB1 10
   #define BMP_CSB2 9
   #define BMP_CSB3 8
   #define BMP_CSB4 7
   

   const byte SONDY_PRESSURE = 4;
   OneWire ds(SONDY_PRESSURE);
   void startPomiarPRESSURE(const byte (SONDY_PRESSURE)) {
   ds.select(SONDY_PRESSURE);}
   float readPomiarPRESSURE(const byte (SONDY_PRESSURE)); 
   byte data(SONDY_PRESSURE); 
   Adafruit_BMP280 bmp1(BMP_CSB1, BMP_SDA, BMP_SDO, BMP_SCL);
   Adafruit_BMP280 bmp2(BMP_CSB2, BMP_SDA, BMP_SDO, BMP_SCL);
   Adafruit_BMP280 bmp3(BMP_CSB3, BMP_SDA, BMP_SDO, BMP_SCL);
   Adafruit_BMP280 bmp4(BMP_CSB4, BMP_SDA, BMP_SDO, BMP_SCL);

   float pressure;
   float PRESS1 =27;
   float PRESS2 =32;
   float PRESS3 =37;
   float p5;
   float p6;
   float p7;
   float p8;
   float p9;  
   float p10;   
   float czas1 = 3800;
   float czas2 = 3500;
   float czas3 = 6000;

   int p1;
   int p2;
   int p3;
   int stop;    
   int stanKRAN1 = A3;
   int stanKRAN2 = 2;
   int stanKRAN3 = 3; 
   int licznik =0; 
   int tmp;
   char znak;
   
   unsigned long pracaKRAN1;
   unsigned long pracaKRAN2;
   unsigned long pracaKRAN3;
   unsigned long aktualnyCzas = 0;
   unsigned long zapamietanyCzasKRAN1 = 0;
   unsigned long zapamietanyCzasKRAN2 = 0;
   unsigned long zapamietanyCzasKRAN3 = 0; 

   boolean test1 = false; 
   boolean test2 = false;
   boolean test3 = false; 
   boolean test4 = false;   
   boolean test5 = false; 
   boolean test6 = false;
   boolean alarm1=false;
   boolean alarm2=false;   
   boolean alarm3=false;  
   boolean alarm4=false; 
   boolean alarm5=false; 
   boolean alarm6=false;   

void setup(){
  
   Serial.begin(9600);

   Wire.begin();
   pinMode(KRAN1,OUTPUT);
   pinMode(KRAN2,OUTPUT);
   pinMode(KRAN3,OUTPUT);
   pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);
   pinMode(LED6, OUTPUT);
   pinMode(ALARM, OUTPUT);
   pinMode(PRZYCISK1, INPUT_PULLUP);
   pinMode(PRZYCISK2, INPUT_PULLUP);
   pinMode(PRZYCISK3, INPUT_PULLUP);
   pinMode(PRZYCISK4, INPUT_PULLUP);
   digitalWrite (ALARM, LOW);
   digitalWrite (LED6, LOW);
   digitalWrite (LED5, LOW);
   digitalWrite (LED4, LOW);
   Serial.setTimeout(50);
   
   if (!bmp1.begin()) {
   Serial.println("Sensor BMP280 device 1 was not found.");
   while (1);}
   delay(50);

   if (!bmp2.begin()) {
   Serial.println("Sensor BMP280 device 2 was not found.");
   while (1);}
   delay(50);
  
   if (!bmp3.begin()) {
   Serial.println("Sensor BMP280 device 3 was not found.");
   while (1);}
   delay(50);
    
   if (!bmp4.begin()) {
   Serial.println("Sensor BMP280 device 4 was not found.");
   while (1);}
   Serial.println();
   delay(50); 

   }
 
void loop(){

   float pracaKRAN1 = czas1;
   float pracaKRAN2 = czas2;
   float pracaKRAN3 = czas3;
   
 //introduction des changement des parmètre du travaille du respirateur                  //
 
   if(Serial.available()>0){
   znak = Serial.read(); 
   tmp = Serial.parseInt();
   Serial.println();
   Serial.print("                ");
   Serial.print("ZMIANA PARAMETROW = " );
   Serial.print(tmp);
   Serial.print(" ");
   Serial.print(znak);
   Serial.println();
   
//Changement du temps- "a2800" -la durée de l'inspiration du Kran1 est changé en 2.8 sec.//

   if(tmp!=czas1 && znak=='a')
   czas1 = tmp;
   
//Changement du temps- "b2400" -la durée de l'inspiration du Kran2 est changé en 2.4 sec.//

   if(tmp!=czas2 && znak=='b')   
   czas2 = tmp;
   
//Changement du temps- "c3200" -la durée de l'inspiration du Kran3 est changé en 3.2 sec.//

   if(tmp!=czas3 && znak=='c')   
   czas3 = tmp;
   
//Changement de pression- "x22" -la base de la pression du module1 est de 22             //

   if(tmp!= PRESS1 && znak=='x') 
   PRESS1 = tmp;
   
//Changement de pression- "y25" -la base de la pression du module2 est de 25             //

   if(tmp!= PRESS2 && znak=='y') 
   PRESS2 = tmp;
   
//Changement de pression- "z30" -la base de la pression du module3 est de 30             //

   if(tmp!= PRESS3 && znak=='z') 
   PRESS3 = tmp;  }

   aktualnyCzas = millis();
   
//synchronisation de module individuel avec la respiration du patient                    //

   if (digitalRead(PRZYCISK1) == LOW) {
   digitalWrite(KRAN1, HIGH); 
   } else {
   pracaKRAN1 = czas1; }
  
   if (digitalRead(PRZYCISK2) == LOW) {
   digitalWrite(KRAN2, HIGH); 
   } else {
   pracaKRAN2 = czas2;}

   if (digitalRead(PRZYCISK3) == LOW) {
   digitalWrite(KRAN3, HIGH); 
   } else {
   pracaKRAN3 = czas3;}
  
   if (aktualnyCzas - zapamietanyCzasKRAN1 >= pracaKRAN1) {
   zapamietanyCzasKRAN1 = aktualnyCzas;
   stanKRAN1 = !stanKRAN1;
   if(!alarm1){
   digitalWrite(KRAN1, stanKRAN1); }
   }
  
   if (aktualnyCzas - zapamietanyCzasKRAN2 >= pracaKRAN2) {
   zapamietanyCzasKRAN2 = aktualnyCzas;
   stanKRAN2 = !stanKRAN2;
   if(!alarm2){
   digitalWrite(KRAN2, stanKRAN2);}
   }
 
   if (aktualnyCzas - zapamietanyCzasKRAN3 >= pracaKRAN3) {
   zapamietanyCzasKRAN3 = aktualnyCzas;
   stanKRAN3 = !stanKRAN3;
   if(!alarm3){
   digitalWrite(KRAN3, stanKRAN3);}
   }
//Egalisation des indications du capteur                                                 //

   float pressure1 = bmp1.readPressure() / 100.0 + 0.65;
   float pressure2 = bmp2.readPressure() / 100.0 - 0.50;
   float pressure3 = bmp3.readPressure() / 100.0 + 0.50;
   float pressure4 = bmp4.readPressure() / 100.0 ;
   
//remtre a zéro tout les base pour faire une unification des donné et des indication     //

   float p1 = ( pressure1 - pressure4);
   float p2 = ( pressure2 - pressure4);
   float p3 = ( pressure3 - pressure4);
   
// L'établissement de la frontiére de la plus haut pression                              //

   p5 = (PRESS1*1.6); 
   p6 = (PRESS2*1.4);
   p7 = (PRESS3*1.5);
   
//Détermination de la pression de fonctionnement des KRAN d'alimentation en air          //

   p8 = (PRESS1*1.05);
   p9 = (PRESS2*1.05);
   p10 = (PRESS3*1.05); 
   
//Alarme de dépassant la pression maximum du module 1                                    //

   if(test1 == false){
   if (p1 >= p5){  
   test1 = true;
   stanKRAN1 = stop; 
   digitalWrite(KRAN1, LOW); 
   digitalWrite (LED4, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm1 = true; }
   }

   if (digitalRead(PRZYCISK4) == LOW){
   stanKRAN1 = !stanKRAN1;  
   alarm1 = false;
   test1=false;
   digitalWrite(KRAN1, HIGH); 
   Serial.print("ALARM WYL ");
   Serial.println();    
   digitalWrite (LED4, LOW);
   digitalWrite (ALARM, LOW);
   }
//Alarme de dépassant la pression maximum du module 2                                    //

   if(test2 == false){
   if (p2 >= p6){
   stanKRAN2 = stop; 
   test2 = true;
   digitalWrite(KRAN2, LOW); 
   digitalWrite (LED5, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm2 = true;}
   }
  
   if (digitalRead(PRZYCISK4) == LOW){
   stanKRAN2 = !stanKRAN2; 
   Serial.println();
   alarm2 = false;
   test2=false;
   digitalWrite(KRAN2, HIGH); 
   Serial.print("ALARM WYL ");
   Serial.println();    
   digitalWrite (LED5, LOW);
   digitalWrite (ALARM, LOW);
   }
//Alarme de dépassant la pression maximum du module 3                                    //

   if(test3 == false){
   if (p3 >= p7){
   stanKRAN3 = stop; 
   test3 = true;
   digitalWrite(KRAN3, LOW); 
   digitalWrite (LED6, HIGH);
   digitalWrite (ALARM, HIGH);  
   alarm3 = true;}
   }

   if (digitalRead(PRZYCISK4) == LOW){
   stanKRAN3 = !stanKRAN3;  
   Serial.println();
   alarm3 = false;
   test3=false;
   digitalWrite(KRAN3, HIGH); 
   Serial.print("ALARM WYL ");
   Serial.println();  
   digitalWrite (LED6, LOW);
   digitalWrite (ALARM, LOW);
   }
//Fermeture du KRAN 1 après la pression requise                                          //

   if( p1 > p8 && p1 < p5){ 
   digitalWrite(KRAN1, LOW); 
   stanKRAN1 = stop; 
   if( p1 < p8);{
   digitalWrite(KRAN1, LOW);
   digitalWrite(KRAN1, stanKRAN1);}    
   } 
//Fermeture du KRAN 2 après la pression requise                                          //

   if( p2 > p9 && p2 < p6){ 
   digitalWrite(KRAN2, LOW); 
   stanKRAN2 = stop; 
   if( p2 < p9);{
   digitalWrite(KRAN2, LOW);
   digitalWrite(KRAN2, stanKRAN2);}    
   }  
//Fermeture du KRAN 3 après la pression requise                                          //

   if( p3 > p10 && p3 < p7){ 
   digitalWrite(KRAN3, LOW); 
   stanKRAN3 = stop; 
   if( p3 < p10);{
   digitalWrite(KRAN3, LOW);
   digitalWrite(KRAN3, stanKRAN3);}    
   }
//Fermer du du KRAN d'air et démarrer l'alarme accident du capteur BMP4                  //

   if (p1 < -10){  
   digitalWrite(KRAN1, LOW); 
   stanKRAN1 = stop;
   digitalWrite (LED4, HIGH);
   digitalWrite (ALARM, HIGH);
   digitalWrite(KRAN2, LOW); 
   stanKRAN2 = stop;
   digitalWrite (LED5, HIGH);
   digitalWrite(KRAN3, LOW); 
   stanKRAN3 = stop;
   digitalWrite (LED6, HIGH);
   
   if (p1 > -10) { 
   digitalWrite(KRAN1, stanKRAN1);
   digitalWrite(KRAN2, stanKRAN2); 
   digitalWrite(KRAN3, stanKRAN3);
   digitalWrite (LED4, LOW);
   digitalWrite (LED5, LOW);
   digitalWrite (LED6, LOW);
   digitalWrite (ALARM, LOW);}    
   }

   if(aktualnyCzas%200==0){
   switch(licznik%8){ 
   case 0:{

   Serial.print("  CZAS 1,  ");
   Serial.print("          CZAS 2, ");
   Serial.print("          CZAS 3,");
   Serial.println();  
   Serial.print("  T1 - a,  ");   
   Serial.print("          T2 - b, ");   
   Serial.print("          T3 - c,");   
   Serial.println();
   }break;
   case 1:{
   Serial.print(" CISNIENE 1,      ");
   Serial.print(" CISNIENE 2,    ");
   Serial.print("   CISNIENE 3,");
   Serial.println();
   Serial.print("  P1 - x");   
   Serial.print(",            P2 - y");   
   Serial.print(",           P3 - z");
   Serial.print(",");     
   Serial.println();
  
   }break;
   case 2:{
   Serial.print("CZAS1=");
   Serial.print(czas1);
   Serial.print(",     CZAS2=");
   Serial.print(czas2);
   Serial.print(",    CZAS3=");
   Serial.print(czas3);
   Serial.println();

   }break;
   case 3:{
 
   Serial.print(" PRESS1=");
   Serial.print(PRESS1);
   Serial.print(",     PRESS2=");
   Serial.print(PRESS2);
   Serial.print(",     PRESS3=");
   Serial.print(PRESS3);
   Serial.println();

   }break;
   case 4:{
    
   Serial.print("  "); 
   Serial.print("p5= ");
   Serial.print(p5);
   Serial.print(",        p6= ");
   Serial.print(p6);
   Serial.print(",        p7= ");
   Serial.print(p7);
   Serial.println();

   }break;
   case 5:{

   Serial.print("  "); 
   Serial.print("p8= ");
   Serial.print(p8);
   Serial.print(",        p9= ");
   Serial.print(p9);
   Serial.print(",        p10= ");
   Serial.print(p10);
   Serial.println();

   }break;
   case 6:{
   
   Serial.print(" REG p1= ");
   Serial.print(p1);
   Serial.print(",    REG p2= ");
   Serial.print(p2);
   Serial.print(",     REG p3= ");
   Serial.print(p3);
   Serial.println();
   }break;
   case 7:{
    
   Serial.print(" p1= ");
   Serial.print(pressure1);   
   Serial.print(",  p2= ");
   Serial.print(pressure2);
   Serial.print(",  p3= ");
   Serial.print(pressure3);
   Serial.print(",  p4= ");
   Serial.print(pressure4);
   Serial.println();
   
   }break;
   default:{

   }break;
   
   }
    licznik++;
   Serial.println();
 
 }
}  
