#include <TimerOne.h>
int maximumrpm[26]={0,0,11,42,74,103,128,148,164,177,
188,197,202,211,216,220,225,229,233,233,235,238,240,242,246,
255};
int minimumrpm[26]={0,0,0,11,43,75,104,129,149,165,178,
189,198,200,212,217,221,226,230,233,233,236,239,241,243,247};
int counter=0;
int counter1=0;
int counterflag=0;
int desiredrpm,actualrpm;
int desiredspeed,actualspeed=0;
int endflag=0; 
int x[4]={0,0,0,0};
int a[4]={0,0,0,0};
int d,i,k,y;
int c=0;
int var5,var1=0;
int mindesiredrpm,maxdesiredrpm=0;
int endflag2=0;
int mode=1; 
/* setup of input and output pins and interrupts */
void setup()
{
    pinMode(13,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(22,OUTPUT);
    pinMode(23,OUTPUT);
    pinMode(25,OUTPUT);
    digitalWrite(13,LOW);
    Serial.begin(9600);
    Timer1.initialize(240000);
    Timer1.attachInterrupt(alert);
    attachInterrupt(0,blockdetect,RISING);
    digitalWrite(13,HIGH);
}
/*main program starts here */
void loop()
{
    do
    { 
        if(Serial.available())
        {
            Timer1.stop();
            d=Serial.read();
            if(d=='!')
            {
                mode=1;
                Timer1.initialize(2400000);
                Timer1.attachInterrupt(alert);
                digitalWrite(13,HIGH);
            }
            else if(d=='#')
            {
                mode=2;
                Timer1.initialize(2400000);
                Timer1.attachInterrupt(alert);
                digitalWrite(13,LOW);
            }
            else
            {
                i=d-48;
                k=0; 
                do
                {
                    if(Serial.available())
                    {
                            x[i]=Serial.read();
                            if(x[i]=='%')
                            {
                                k=1;
                            }
                            a[i]=x[i]-48;
                            i--;
                    }
                }while(k==0);
                Timer1.initialize(2400000);
                Timer1.attachInterrupt(alert);
                y=d-48;
                switch(y)
                {
                        case 1: c=a[y];
                        break;
                        case 2: c=a[y]*10+a[y-1];
                        break;
                        case 3: c=a[y]*100+a[y-1]*10+a[y-2];
                        break;
                }
                digitalWrite(22,HIGH);
                digitalWrite(23,LOW);
                analogWrite(7,c);
                desiredspeed=c;
                var5=c;
            }
        }
        else
        { 
            if(mode==2)
            {
                var1=var5/10;
                mindesiredrpm=minimumrpm[var1];
                maxdesiredrpm=maximumrpm[var1];
                if(actualrpm>=mindesiredrpm && actualrpm<=maxdesiredrpm)
                {
                    endflag2=1;
                    digitalWrite(52,HIGH);
                }
                else
                {
                    digitalWrite(52,LOW);
                    endflag2=0;
                    if(actualrpm<mindesiredrpm)
                    {
                            if(actualspeed<=245)
                            {
                                actualspeed=actualspeed+10;
                                analogWrite(7,actualspeed);
                            }
                            else
                            {
                                endflag2=1;
                            }  
                    }
                    else
                    {
                        if(actualspeed>=10)
                        {
                            actualspeed=actualspeed-10;
                            analogWrite(7,actualspeed);
                        }
                        else
                        {
                            endflag2=1;
                        }
                    }
                    counterflag=0;
                    while(counterflag=0);
                    counterflag=0;
                }
            }
        }
    }while(1);
}
// hardware interrupt subroutine for slot detecting
void blockdetect()
{
    counter++;
}
    /* timer interrupt subroutine for counting value of pulse for 
    predefined time */
void alert()
{
    counter1=counter;
    actualrpm=counter1;
    Serial.print(counter1);
    Serial.print('\n');
    counterflag=1;
    counter=0;
}
