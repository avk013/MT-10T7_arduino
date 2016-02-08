#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_1 2
#define ONE_WIRE_BUS_2 4
//////////////////////
#define PIN_WR1 3
//#define PIN_WR2 2 //просто соединил с землей и pin 2 освободил
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
  SEG_F | SEG_E | SEG_D | SEG_A | SEG_B | SEG_C, //0
  SEG_B | SEG_C ,//1
  SEG_E | SEG_D | SEG_A | SEG_B | SEG_G,//2
  SEG_D | SEG_A | SEG_B | SEG_C | SEG_G,//3
  SEG_F | SEG_B | SEG_C | SEG_G,//4
  SEG_F | SEG_D | SEG_A | SEG_C | SEG_G,//5
  SEG_F | SEG_E | SEG_D | SEG_A | SEG_C | SEG_G,//6
  SEG_A | SEG_B | SEG_C ,//7
  SEG_F | SEG_E | SEG_D | SEG_A | SEG_B | SEG_C | SEG_G,//8
  SEG_F | SEG_D | SEG_A | SEG_B | SEG_C | SEG_G};//9 
//////////////////////////////
OneWire oneWire_in(ONE_WIRE_BUS_1);
OneWire oneWire_out(ONE_WIRE_BUS_2);
DallasTemperature sensor_inhouse(&oneWire_in);
DallasTemperature sensor_outhouse(&oneWire_out);
/////////////////////////////
void setup() {
sensor_inhouse.begin();
sensor_outhouse.begin();
pinMode(PIN_A0, OUTPUT); pinMode(PIN_WR1, OUTPUT);//pinMode(PIN_WR2, OUTPUT);
pinMode(PIN_DB0, OUTPUT);pinMode(PIN_DB1, OUTPUT);
pinMode(PIN_DB2, OUTPUT);pinMode(PIN_DB3, OUTPUT);
delay(100);
digitalWrite(PIN_WR1, LOW);//digitalWrite(PIN_WR2, LOW);
//обнуление
for (int i = 0; i < 10; i++) {write_lcd_raw(i, 255);delay(150);}
for (int i = 0; i < 10; i++) {write_lcd_raw(i, 0);delay(150);}
for (int i = 0; i < 10; i++) {write_lcd_raw(i, SEG_G); delay(150);}
for (int i = 0; i < 10; i++) {write_lcd_raw(i, 0);delay(50);}
}
/////////////////////
void _write_dbs(byte nibble) { //запись битов младших 4 разрядов
  digitalWrite(PIN_DB0, nibble & 1 ? HIGH : LOW);
  digitalWrite(PIN_DB1, nibble & 2 ? HIGH : LOW);
  digitalWrite(PIN_DB2, nibble & 4 ? HIGH : LOW);
  digitalWrite(PIN_DB3, nibble & 8 ? HIGH : LOW);}
/////////////////////
void _write_wr() { // переключиние на запись
  //digitalWrite(PIN_WR2, LOW); 
  digitalWrite(PIN_WR1, HIGH);
  delayMicroseconds(7);digitalWrite(PIN_WR1, LOW);}
/////////////////////
void write_lcd_raw(byte addr, byte data) {
  _write_dbs(addr & 15);
  digitalWrite(PIN_A0, LOW);
  _write_wr();  _write_dbs(data & 15);
  digitalWrite(PIN_A0, HIGH);
  _write_wr(); _write_dbs((data >> 4) & 15);
  _write_wr();
  // Unnecessary cleanup
  digitalWrite(PIN_A0, LOW); _write_dbs(0); }
/////
// вывод отрицательн.и положит.веществ.чисел
// obrez число обрезаемых знаков с конца просто указать 
// то что нужно и убрать из переменных
void go_display(float number, byte razr) {
byte point=0,zap=0, out, obrez=1;
String stroka=String(number);
char *z= new char[stroka.length()+1];
stroka.toCharArray(z,stroka.length()+1);
razr=razr-stroka.length()+2;
for (int i = 0; i<stroka.length()-obrez; i++){
int b=z[i]-'0';
out=num[b];
if (z[i+1]-'0'==-2) point=16;
if (b==-3) out=1;
write_lcd_raw(razr+i-zap, out+point);
if (point==16) {point=0;i++;zap=1;}
delay(5);} delete[] z, stroka;  };

////////////////////////////////
void loop() {
///////// индикатор temp
int g = 0,offset = 4, offset2=9;
float temp, temp2;
  while (1) {
sensor_inhouse.requestTemperatures();  
sensor_outhouse.requestTemperatures();  
temp=sensor_inhouse.getTempCByIndex(0);
temp2=sensor_outhouse.getTempCByIndex(0);
for (int i = 0; i < 10; i++) {write_lcd_raw(i, 0);}
go_display(temp,offset); //offset последний разряд после числа
go_display(temp2,offset2);
write_lcd_raw(offset2,g);
if (g==137) g=169; else g=137; //показ значка "о" OR "с"
write_lcd_raw(offset,g);
delay(100); 
}}
