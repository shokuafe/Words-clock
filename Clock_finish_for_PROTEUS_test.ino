#define MINUTES_START 16
#define POS_15        15
#define POS_5         16
#define MIN_WORD      22
#define MAXLED        24  

// 00 | 00 0000 | 0 | 000 | 0000 0000 0000 |
/*
 * x = 123456;
 * y = 127 == 0xAA == 0b10101010;
 * 
 */
int x;

int OE=4;
int CLK=5;
int LA=6;
int DATA=3;

void setup() {

  pinMode(OE, OUTPUT); //OUTPUT ENABLE (отображать цифры) всегда вкл, выходной буфер
  pinMode(CLK, OUTPUT); //CLOCK (SH_CP) (запись данных для вывода сегментных данных)
  pinMode(LA, OUTPUT); //LATCH (ST_CP) (защёлка, для вывода записанных данных в CLOCK)
  pinMode(DATA, OUTPUT);// (DS) сегментные данные, самый первый этап

  digitalWrite(OE, 0);
}

void loop() {
  unsigned long bits;
//  bits = calc(2,45);
  bits = 1;
  for (char i = 0; i < 24; i++) 
  {
    output(bits);
    bits <<= 1;
    delay(500);
  }

}

void output(unsigned long bits)
{
  x = 0;
  do 
  {
    if(bits & 0x01)
    {
      digitalWrite(DATA, 1);
      delay(1);
      digitalWrite(CLK, 1);
      delay(1);
      digitalWrite(CLK, 0);
    }
    else
    {
      digitalWrite(DATA, 0);
      delay(1);
      digitalWrite(CLK, 1);
      delay(1);
      digitalWrite(CLK, 0);
    }
    x++;
    bits >>= 1;
  }
  while (x < MAXLED);
  digitalWrite(LA, 1);
  delay(1);
  digitalWrite(LA, 0);  
}

unsigned long calc(unsigned char hours, unsigned char minutes)
{
  unsigned long out;
  unsigned long hrs_offset = 0;

  out = (1 << (hours - 1));

  if(hours == 1)
  {
    hrs_offset = 1 << 12;
  }

  if(hours > 1)
  {
    hrs_offset = 1 << 13;
  }

  if(hours > 4)
  {
    hrs_offset = 1 << 14;
  }

  if (hours > 12)
  {
    hrs_offset = 1 >> 12;
  }
  
  out |= hrs_offset;
  
  unsigned long mnts_offset;

  if (minutes != 15)
  {
    unsigned char decimals;
    unsigned char singles;

    singles = minutes % 10; //44 % 10 = 4
    if (singles > 4)
    {
      mnts_offset |= 1UL << POS_5; // +5 min
    }
    decimals = minutes / 10; //44 / 10 = 4
    mnts_offset = (unsigned long)(1UL << (decimals + MINUTES_START)); // 1 << 19
  }
  else
  {
    mnts_offset = (unsigned long)(1UL << 15); // 1 << 15
  }

  if (minutes > 0)
  {
    mnts_offset |= (unsigned long)(1UL << MIN_WORD);
  }

  out |= mnts_offset;
  
  return out;
  }

