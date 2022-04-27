#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <Servo.h>
const byte buzzer = 12;


bool estado_tampa = true;

RTC_DS1307 rtc;
Servo myservo;


char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};

byte pinosLinhas[]  = {9,2,3,4};

byte pinosColunas[] = {5,6,7,8};

char teclas[4][4] = {{'1','2','3','A'},
                     {'4','5','6','B'},
                     {'7','8','9','C'},
                     {':','0','#','D'}};

Keypad teclado1 = Keypad( makeKeymap(teclas), pinosLinhas, pinosColunas, 4, 4);



const byte endereco = 0x3F; // Endereços comuns: 0x27, 0x3F
const byte colunas = 16;
const byte linhas = 2;
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

int i=0, j=0, horario[4], salvar=0, telahora=0, selecalarme, muda_alarme, alarme1, print_alarme,  print_alarme2; 
char hora;
char alarme[4];
char alarme2[4];
int horanum[4], dezhora=0, dezminuto=0;
int horanum2[4], dezhora2=0, dezminuto2=0;
int pos = 0;
int sairalarme = 1;

void setup() {
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  myservo.write(0);
  pinMode(buzzer, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //rtc.adjust(DateTime(2022, 04, 27, 10, 18, 30));
  myservo.attach(10);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void bip_button(){
  tone(12, 2525);
  delay(100);
  noTone(12);
}
void bip_button2(){
  tone(12, 1515);
  delay(100);
  noTone(12);
}
void MenuAlarme()
{
   lcd.clear();
   lcd.print("Qual Alarme?");
   lcd.setCursor(0, 1); 
   lcd.print("Alrm1.    Arlm2.");
   char tecla_selec = teclado1.waitForKey();

   if(tecla_selec == '1')
   {
    ConfigAlarme();
   }
   if(tecla_selec == '2')
   {
    ConfigAlarme2();
   }
   else
   { 
   }     
}

void ConverteHora()
{
    lcd.clear();
    dezhora=0;
    dezminuto=0;
    if (horanum[0] != 0)
    {
      dezhora = horanum[0] * 10;
    }
    else
    {
      dezhora=horanum[0];
    }
    dezhora = dezhora + horanum[1];
    

    if(horanum[2] != 0)
    {
      dezminuto= horanum[2] * 10;
    }
    else
    {
      dezminuto=horanum[2];
    }
    dezminuto=dezminuto + horanum[3];
}


void ConfigAlarme()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Config Alarme");
    lcd.setCursor(0, 1);
    lcd.print("Alarme:");
    for(int i=0;i<4;i++){
      alarme[i] = teclado1.waitForKey();
      lcd.print(alarme[i]);
      bip_button2();
      if (i == 1)
      {
        lcd.print(":");
      }
    }
     for(i = 0; i < 5; i++)
  {
    switch(alarme[i])
    {
      case  '1' : horanum[i] = 1; break;
      case  '2' : horanum[i] = 2; break;
      case  '3' : horanum[i] = 3; break;
      case  '4' : horanum[i] = 4; break;
      case  '5' : horanum[i] = 5; break;
      case  '6' : horanum[i] = 6; break;
      case  '7' : horanum[i] = 7; break;
      case  '8' : horanum[i] = 8; break;
      case  '9' : horanum[i] = 9; break;
      case  '0' : horanum[i] = 0; break;
    }
  }
  ConverteHora();
    lcd.clear();
    
}
void ConverteHora2()
{
    lcd.clear();
    dezhora2=0;
    dezminuto2=0;
    if (horanum2[0] != 0)
    {
      dezhora2 = horanum2[0] * 10;
    }
    else
    {
      dezhora2=horanum2[0];
    }
    dezhora2 = dezhora2 + horanum2[1];
    

    if(horanum2[2] != 0)
    {
      dezminuto2= horanum2[2] * 10;
    }
    else
    {
      dezminuto2=horanum2[2];
    }
    dezminuto2=dezminuto2 + horanum2[3];
}
void ConfigAlarme2()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Config Alarme 2");
    lcd.setCursor(0, 1);
    lcd.print("Alarme:");
    for(int j=0;j<4;j++){
      alarme2[j] = teclado1.waitForKey();
      lcd.print(alarme2[j]);
      bip_button2();
      if (j == 1)
      {
        lcd.print(":");
      }
    }
     for(j = 0; j < 5; j++)
  {
    switch(alarme2[j])
    {
      case  '1' : horanum2[j] = 1; break;
      case  '2' : horanum2[j] = 2; break;
      case  '3' : horanum2[j] = 3; break;
      case  '4' : horanum2[j] = 4; break;
      case  '5' : horanum2[j] = 5; break;
      case  '6' : horanum2[j] = 6; break;
      case  '7' : horanum2[j] = 7; break;
      case  '8' : horanum2[j] = 8; break;
      case  '9' : horanum2[j] = 9; break;
      case  '0' : horanum2[j] = 0; break;
    }
  }
  ConverteHora2();
    lcd.clear();
    
}

void loop() 
{
    char tecla_pressionada = teclado1.getKey();

    //ALARME1
    if(rtc.now().hour() == dezhora && rtc.now().minute() == dezminuto && rtc.now().second()== 0)
    {

      myservo.write(70);
      sairalarme = 1;
      while(rtc.now().hour() == dezhora && rtc.now().minute() == dezminuto && sairalarme==1)
      {
      char tecla_alarme = teclado1.getKey();

      if (tecla_alarme == '0')
      {
        sairalarme = 0;
      }
      tone(12, 2525);
      digitalWrite(11, HIGH);
      } 
    }
     //ALARME2
    if(rtc.now().hour() == dezhora2 && rtc.now().minute() == dezminuto2 && rtc.now().second()== 0)
    {

      myservo.write(70);
      sairalarme = 1;
      while(rtc.now().hour() == dezhora2 && rtc.now().minute() == dezminuto2 && sairalarme==1)
      {
      char tecla_alarme = teclado1.getKey();

      if (tecla_alarme == '0')
      {
        sairalarme = 0;
      }
      tone(12, 2525);
      digitalWrite(13, HIGH);
      } 
    }
     
    if(telahora == 1)
    {
      lcd.setCursor(4, 0);
      
      if(rtc.now().hour() < 10)
      {
      lcd.print("0");
      lcd.print(rtc.now().hour());      
      lcd.print(":"); 
      }
      else
      {
      lcd.print(rtc.now().hour());      
      lcd.print(":"); 
      }
           
      if(rtc.now().minute() < 10)
      {
      lcd.print("0");
      lcd.print(rtc.now().minute());
      lcd.print(":");            
      }
      else
      {
      lcd.print(rtc.now().minute());
      lcd.print(":");
      }
        if(rtc.now().second() < 10)
      {
      lcd.print("0");
      lcd.print(rtc.now().second());       
      
      }
      else
      {
      lcd.print(rtc.now().second());       
      }      
          
      lcd.setCursor(3, 1); 
      
      if(rtc.now().day() < 10)
      {
      lcd.print("0");
      lcd.print(rtc.now().day());       
      lcd.print("/");
      }
      else
      {
      lcd.print(rtc.now().day());       
      lcd.print("/"); 
      }
      
      if(rtc.now().month() < 10)
      {
      lcd.print("0");
      lcd.print(rtc.now().month());       
      lcd.print("/");
      }  
      else
      {
      lcd.print(rtc.now().month());     
      lcd.print("/");                  
      }
                       
      lcd.print(rtc.now().year());
    }  
     
     switch(tecla_pressionada)
  {
    case '#':
    if(telahora == 1)
    {
      lcd.clear();
      telahora=0;
      lcd.print(tecla_pressionada);
      
    }
    telahora=0;
    lcd.clear();
    break;
    
    case 'A':
    if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    bip_button();
    MenuAlarme();
    bip_button();
    break;
    
    case 'B':
    if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.clear();
    print_alarme2 = 1;
    bip_button();
    break;
    
    case 'C':
    if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    //som_alarm();
    print_alarme = 1;
    
    break;
    
    case 'D':
    lcd.clear();
    telahora=1;
    break;
    
    case ':':
    if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(":");
    myservo.write(0);
    digitalWrite(11, LOW);
    digitalWrite(13, LOW);
    bip_button();
    break;
    
    case '1':
    if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button(); 
    if (muda_alarme == 1)
    {
      horario[i]=1;
    }
    break;
    
     case '2':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    if (muda_alarme == 1)
    {
      horario[i]=2;
    }
    break;
    
     case '3':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    break;
    
     case '4':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    if (muda_alarme == 1)
    {
      horario[i]=4;
    }
    break;
    
     case '5':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    break;
    
     case '6':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    break;
    
     case '7':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    break;
    
     case '8':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    if (muda_alarme == 1)
    break;
    
     case '9':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    
    break;

     case '0':
     if(telahora == 1)
    {
      lcd.clear();
    }
    telahora=0;
    lcd.print(tecla_pressionada);
    bip_button();
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
    myservo.write(70);
  
    break; 
  }
 
  if( print_alarme == 1)
  {
    print_alarme=0;
    salvar=1;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(dezhora);
    lcd.print(":");
    lcd.print(dezminuto);
    lcd.print("  C.");
    lcd.print(alarme[0]);
    lcd.print(alarme[1]);
    lcd.print(":");
    lcd.print(alarme[2]);
    lcd.print(alarme[3]);
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("Config alarme.1");
  }

  if( print_alarme2 == 1)
  {
    print_alarme2=0;
    salvar=1;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(dezhora2);
    lcd.print(":");
    lcd.print(dezminuto2);
    lcd.print("  C.");
    lcd.print(alarme2[0]);
    lcd.print(alarme2[1]);
    lcd.print(":");
    lcd.print(alarme2[2]);
    lcd.print(alarme2[3]);
    
    delay(100);
    lcd.setCursor(0, 0);
    lcd.print("Config alarme.2");
  }
}
