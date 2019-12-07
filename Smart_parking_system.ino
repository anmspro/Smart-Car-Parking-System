#include <LiquidCrystal.h>

int trig_out = 12, ech_out= 13, trig_in = 8, ech_in = 9;
int d4 = 4,d5 = 5,d6 = 6,d7 = 7,rs = 2,en = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool out = false, in = false, c_chg = true ;
int count = 0, tr = 10, total = 10, wait = 0, wait_tr = 5;


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  pinMode(trig_out, OUTPUT); 
  pinMode(ech_out, INPUT);
  pinMode(trig_in, OUTPUT); 
  pinMode(ech_in, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //reading from the outer sensor

  digitalWrite(trig_out, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_out, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_out, LOW);

  double duration = pulseIn(ech_out, HIGH);

  double d_out= duration*0.034/2;

//  lcd.setCursor(0, 0);
//  lcd.print("Distance: ");
//  lcd.print(d_out);

  // reading from the inner sensor

  digitalWrite(trig_in, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_in, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_in, LOW);

  duration = pulseIn(ech_in, HIGH);

  double d_in= duration*0.034/2;
  
//  lcd.setCursor(0, 1);
//  lcd.print("Distance: ");
//  lcd.print(d_in);

  // check whether if anything moves out or in

  if(d_out < tr)
  {
    if(!out && !in)
    {
      out = true;
      wait = 0;       // waits for some time to get second input
    }
    else if(!out && in)
    {
      count--;
      in = false;
      c_chg = true;   // check if count changes
    }
  }
  else if(d_in < tr)
  {
    if(!out && !in)
    {
      in = true;
      wait = 0;
    }
    else if(out && !in)
    {
      count++;
      out = false;
      c_chg = true;
    }
  }

  if(count < 0) count = 0;        // if empty garage performs out
  else if(count > total) count = total;

  if(c_chg)                 // display changes if count changes
  {
    lcd.clear();
    if(count >= total)
    {
      lcd.setCursor(0, 0);
      lcd.print("Garage is full!!");
      lcd.setCursor(0, 1);
      lcd.print("No new car entry");
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Vacant : ");
      lcd.print(total - count);
      lcd.print("/");
      lcd.print(total);
    }

    c_chg = false;
  }

//  lcd.setCursor(15,0);
//  lcd.print(out);
//  lcd.setCursor(15,1);
//  lcd.print(in);

  if(in || out)             // configuring the wait 
  {
    wait++;
    if(count<total)
    {
      lcd.setCursor(0, 1);
      lcd.print("        ");
    }
  }

  if(wait > wait_tr)    // resetting the signals after wait
  {
    in = false;
    out = false;
    wait = 0;
  }

  if(!in && !out && count<total)  // the garage is ready for 
  {                               // operations
    lcd.setCursor(0, 1);
    lcd.print("Ready...");
  }
  
  delay(100);
}
