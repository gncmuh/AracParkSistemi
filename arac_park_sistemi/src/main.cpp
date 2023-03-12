#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

LiquidCrystal_I2C lcd(0x27,16,2);
struct mesafe_veriler
{
  int mesafe,sure;
  int mesafe_gelen;
};
struct mesafe_veriler veri;
const uint8_t pinler[3] PROGMEM={3,4,5}; //3.pin echo 4.pin trig 5.pin buzzer
unsigned long zaman=0;
const int ses_toleransi=600; //bu azaltıldığın buzzer açma kapama süresi artacaktır yüksek olursa ise azalatacaktır azalması bir nesneye yakın olduğunun uyarsını yaklaştıkça neseneye artıracaktır.
int ses_cikis;
int mesafe_al();
void ses_ton(unsigned long sure_m);
void setup() {

    Serial.begin(9600);
    lcd.init();lcd.backlight();
    for(uint8_t i=0; i<3; i++)
    {
      byte gelen=pgm_read_word_near(pinler+i);
      if(i==0){pinMode(gelen,INPUT);}
      else{pinMode(gelen,OUTPUT);}
    }
   
}
void loop() {

  veri.mesafe_gelen=mesafe_al();
  lcd.setCursor(0,0);lcd.print(F("MESAFE="));lcd.print(veri.mesafe_gelen);
  if(veri.mesafe_gelen<10){lcd.setCursor(8,0);lcd.print(F(" "));}
  else if(veri.mesafe_gelen<100){lcd.setCursor(9,0);lcd.print(F(" "));lcd.print(F("CM"));}
}
int mesafe_al() {

  if(!bool(0))
  {
    digitalWrite(pinler[1],HIGH);
    delayMicroseconds(100);
    digitalWrite(pinler[1],LOW);
    veri.sure=pulseIn(pinler[0],HIGH);
    ses_cikis=(veri.sure-ses_toleransi);if(ses_cikis<100){ses_cikis=100;}
    ses_ton(ses_cikis);
    veri.mesafe=(veri.sure/2)/29.1;
  }
  return veri.mesafe;
}
void ses_ton(unsigned long sure_m) {

  if(millis()-zaman>sure_m){
    zaman=millis();
    digitalWrite(pinler[2],HIGH);
  }
  delay(10);
  digitalWrite(pinler[2],LOW);
}

