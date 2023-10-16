#include<dht.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

dht DHT;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

float temperaturaSetata = 25;

int buttonState = LOW;
int stareaButonului = LOW;
int buttonState1 = LOW;
int buttonState2 = LOW;
int buttonState3 = LOW;

const double Kp = 77.0474;//Acesta determină cât de repede controlul reacționează la eroarea dintre temperatura actuală și valoarea țintă. 
//Dacă doriți ca temperatura să crească rapid și apoi să rămână constantă, puteți seta un parametru P relativ mare. 
//Cu cât este mai mare, cu atât controlul va acționa mai agresiv pentru a atinge valoarea țintă.

const double Ki = 1.0455;//Acesta este utilizat pentru a trata eroarea țintă pe o perioadă mai lungă de timp. 
//Dacă doriți ca temperatura să rămână constantă și să compenseze variațiile de lungă durată, puteți seta un parametru I. 
//Cu cât este mai mare, cu atât sistemul va reacționa mai eficient la erori pe termen lung.

const double Kd = 73.3356;//Acesta reacționează la schimbările rapide ale temperaturii și ajută la prevenirea suprareglării. 
//Dacă doriți ca temperatura să rămână constantă și să evitați fluctuațiile bruște, puteți seta un parametru D. 
//Cu cât este mai mare, cu atât sistemul va reacționa mai rapid la schimbări.

int previousError = 0;
int integral = 0;

void setup() {
  DDRC &= ~(1<<0);
  DDRC |= (1<<1) | (1<<2) | (1<<3) | (1<<4);
  DDRB |= (1<<1);

  TCCR1A = (1<<7) | (1<<1) | (1<<0);
  TCCR1B = (1<<0) | (1<<3);
  OCR1A = 0;

  lcd.init();
  lcd.backlight();
  
  delay(100);
}

void loop() {

  int dht_apin = A0;//alias

  int error;

  int P;
  int I;
  int D;

  int output;

  DHT.read11(dht_apin);
  
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.print(DHT.temperature);
  lcd.print("C ");

  buttonState = PINC & (1<<1);  //OK
  buttonState1 = PINC & (1<<2); //CANCEL
  buttonState2 = PINB & (1<<2); //INCREMENT +
  buttonState3 = PINB & (1<<3); //DECREMENT -

if(buttonState) //OK
{
  stareaButonului=HIGH;
}

if(buttonState1)//CANCEL
{
  stareaButonului = LOW;
  PORTC &= ~(1<<0);
  OCR1A = 0;
  temperaturaSetata=25;
  delay(1000);
}

if(stareaButonului) 
{

  error = temperaturaSetata - DHT.temperature;

  P = Kp * error;
  integral += error;
  I = Ki * integral;
  D = Kd * (error - previousError);

  output = P + I + D  ;

  if(output < 0)
  {
    output = 0;
  }
 else if(output > 255)
  {
    output = 255;
  }
   
  OCR1A = output;
  previousError = error;
}


if(buttonState2)
{
  temperaturaSetata += 1;
  delay(1000);
}

if(buttonState3)
{
  temperaturaSetata -=1;
  delay(1000);
}

  lcd.setCursor(0,1);
  lcd.print("TempSet: ");
  lcd.print(temperaturaSetata);
  lcd.print("C ");


  delay(2000);
}
