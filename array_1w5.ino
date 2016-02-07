#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_1 2
//////////////////////
#define PIN_WR1 3
//#define PIN_WR2 2
#define PIN_A0 13
#define PIN_DB0 9
#define PIN_DB1 10
#define PIN_DB2 11
#define PIN_DB3 12
#define SEG_A 8
#define SEG_B 32
#define SEG_C 64
#define SEG_D 4
#define SEG_E 2
#define SEG_F 128
#define SEG_G 1
#define SEG_H 16
//////////////////////////////
static byte num[11] = {
  SEG_F | SEG_E | SEG_D | SEG_A | SEG_B | SEG_C,
  SEG_B | SEG_C ,
  SEG_E | SEG_D | SEG_A | SEG_B | SEG_G,
  SEG_D | SEG_A | SEG_B | SEG_C | SEG_G,
  SEG_F | SEG_B | SEG_C | SEG_G,
  SEG_F | SEG_D | SEG_A | SEG_C | SEG_G,
  SEG_F | SEG_E | SEG_D | SEG_A | SEG_C | SEG_G,
  SEG_A | SEG_B | SEG_C ,
  SEG_F | SEG_E | SEG_D | SEG_A | SEG_B | SEG_C | SEG_G,
  SEG_F | SEG_D | SEG_A | SEG_B | SEG_C | SEG_G
    };
static byte STR_HELLO[5] = {
  SEG_F | SEG_E | SEG_G | SEG_B | SEG_C,
  SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,
  SEG_F | SEG_E | SEG_D,
  SEG_F | SEG_E | SEG_D,
  SEG_F | SEG_E | SEG_D | SEG_A | SEG_B | SEG_C  };
  //////////////////////////////
OneWire oneWire_in(ONE_WIRE_BUS_1);
DallasTemperature sensor_inhouse(&oneWire_in);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    Serial.println("Dallas Temperature Control Library Demo - TwoPin_DS18B20");
    sensor_inhouse.begin();
////////
delay(3000);//обнуление
pinMode(PIN_A0, OUTPUT); pinMode(PIN_WR1, OUTPUT);//pinMode(PIN_WR2, OUTPUT);
pinMode(PIN_DB0, OUTPUT);pinMode(PIN_DB1, OUTPUT);
pinMode(PIN_DB2, OUTPUT);pinMode(PIN_DB3, OUTPUT);
digitalWrite(PIN_WR1, LOW);//digitalWrite(PIN_WR2, LOW);
}
void _write_dbs(byte nibble) { //запись битов младших 4 разрядов
  digitalWrite(PIN_DB0, nibble & 1 ? HIGH : LOW);
  digitalWrite(PIN_DB1, nibble & 2 ? HIGH : LOW);
  digitalWrite(PIN_DB2, nibble & 4 ? HIGH : LOW);
  digitalWrite(PIN_DB3, nibble & 8 ? HIGH : LOW);}
void _write_wr() { // переключиние на запись
  //digitalWrite(PIN_WR2, LOW); 
  digitalWrite(PIN_WR1, HIGH);
  delayMicroseconds(5);digitalWrite(PIN_WR1, LOW);}
void write_lcd_raw(byte addr, byte data) {
  _write_dbs(addr & 15);
  digitalWrite(PIN_A0, LOW);
  _write_wr();  _write_dbs(data & 15);
  digitalWrite(PIN_A0, HIGH);
  _write_wr(); _write_dbs((data >> 4) & 15);
  _write_wr();
  // Unnecessary cleanup
  digitalWrite(PIN_A0, LOW); _write_dbs(0); }
//
void go_display(float number, byte razr) {
byte point=0,zap=0, out;
String stroka=String(number);
char *z= new char[stroka.length()+1];
stroka.toCharArray(z,stroka.length()+1);
for (int i = 0; i<stroka.length(); i++){
int b=z[i]-'0';
out=num[b];
//if (z[i+1]-'0'==-2 && b!=-3) point=16;
if (z[i+1]-'0'==-2) point=16;
if (b==-3) out=1;
write_lcd_raw(razr+i-zap, out+point);
if (point==16) {point=0;i++;zap=1;}
delay(5);
Serial.print(b);
Serial.print('+');
}
delete[] z, stroka;  };
//////////////////
////////////////////////////////
void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Requesting temperatures...");
    sensor_inhouse.requestTemperatures();
    Serial.println(" done");

    Serial.print("Inhouse: ");
    Serial.println(sensor_inhouse.getTempCByIndex(0));
///////// индикатор
for (int i = 0; i < 10; i++) {
    write_lcd_raw(i, 0);
  }
  delay(50);
  for (int i = 0; i < 10; i++) {
    write_lcd_raw(i, SEG_G); delay(50);}
  delay(500);
  for (int i = 0; i < 10; i++) { write_lcd_raw(i, 0); }  
  int dir = 1,offset = 1;
  float temp=5;
  while (1) {
temp-=0.01;
go_display(temp,offset);
Serial.print('=');
Serial.print(temp);
Serial.println();
delay(50);  
  }
}
