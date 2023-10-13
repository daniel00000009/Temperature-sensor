#include<dht.h>

dht DHT;

float temperaturaSetata = 25;

int buttonState = LOW;
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

  Serial.begin(9600);
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

  Serial.print("\n");
  Serial.print("Temperature: ");
  Serial.print(DHT.temperature);
  Serial.print("C \n");

  buttonState = PINC & (1<<1);  //OK
  buttonState1 = PINC & (1<<2); //CANCEL
  buttonState2 = PINC & (1<<3); //INCREMENT +
  buttonState3 = PINC & (1<<4); //DECREMENT -


if(buttonState) //Place for the OK buttoon but the process works fine without it, if you want you can change your own. This way is more like an instant response of the system.
  //You will have to keep somewhere the state of the button so once it was pressed it will remain like that (atleast the microcontroller thinks so).
{

  
}
else
{
  if(buttonState1)// CANCEL button
  {
    temperaturaSetata=25;
    delay(1000);
  }

  if(buttonState2)//INCREMENT button
  {
    temperaturaSetata += 1;
    delay(1000);
  }

  if(buttonState3)//DECREMENT button
  {
    temperaturaSetata -=1;
    delay(1000);
  }
}
  Serial.print("TempSetata: ");
  Serial.print(temperaturaSetata);
  Serial.print("C \n");

  error = temperaturaSetata - DHT.temperature;

  P = Kp * error;
  integral += error;
  I = Ki * integral;
  D = Kd * (error - previousError);

  output = P + I + D  ;//the duty cycle for the PWM

  if(output < 0)//this microcontroler is working with a 8 bit PWM so the value can go up to 255 which is a 100% duty cycle and 0 is 0%;
  {
    output = 0;
  }
 else if(output > 255)
  {
    output = 255;
  }
   
  analogWrite(9, output);

  previousError = error;

  delay(2000);//I delay my code because I use a DHT11 temperature sensor which it's doing better reading every 2 sec.

}





