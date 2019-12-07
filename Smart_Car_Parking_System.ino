#include <LiquidCrystal.h>
 
int trigger_out = 12, echo_out= 13, trigger_in = 8, echo_in = 9;
int d4 = 4, d5 = 5, d6 = 6, d7 = 7, rs = 2, en = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool out = false, in = false, c_chg = true ;
int count = 0, tr = 15, total = 5, wait = 0, wait_tr = 5;
 
 
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  pinMode(trigger_out, OUTPUT); 
  pinMode(echo_out, INPUT);
  pinMode(trigger_in, OUTPUT); 
  pinMode(echo_in, INPUT);
}
 
void loop() {
  // put your main code here, to run repeatedly:
 
  //reading from the outer sensor
 
  digitalWrite(trigger_out, LOW);
  delayMicroseconds(2);
 
  digitalWrite(trigger_out, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_out, LOW);
 
  double duration = pulseIn(echo_out, HIGH);
 
  double d_out = duration*0.034/2;
 
  // reading from the inner sensor
 
  digitalWrite(trigger_in, LOW);
  delayMicroseconds(2);
 
  digitalWrite(trigger_in, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_in, LOW);
 
  duration = pulseIn(echo_in, HIGH);
 
  double d_in= duration*0.034/2;
 
 
//  lcd.setCursor(0, 0);
//  lcd.print("Distance: ");
//  lcd.print(d_out);
//
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
