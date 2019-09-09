// Morse code random blocks trainer
// 
// Assumptions:
//  Speaker on D13
//  Speed potentiometer on A0
//
// Copyright (c) 2019 Michael Robinson

#define SPK_PIN 13   // Digital pin for speaker
#define SPD_PIN 0    // Analog pin for potentiometer
#define TONE 700 // Tone of the signal in Hz

#define BLK_MIN 3
#define BLK_MAX 5

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890./=";
#define ALPHABET_LEN 39
const char *code[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", 
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-", "-..-.", "-...-"};

int spd = 125; // Current length of a dot in ms

void string_send( char *string )
{
  int code_start;
  int i, j;

  // Scan over string to transmit
  for( j = 0; string[j] != NULL; j ++ )
  {
    if( string[j] == ' ' )
    {
      delay(spd*3);
    }
    else
    {
      // Look up this character in the lookup table
      for( i = 0; alphabet[i] != NULL; i ++ )
      {
        if( alphabet[i] == string[j] )
        {
          // Found the character
          cw_send( code[i] );
          cw_send( ";" );
          break;
        }
      }
    }
  }
}

// Decode dots '.', dashes '-', and spaces ';' into audio, given a speed.  The speed is the length in ms of a dot
void cw_send( char *string )
{
  int i;

  spd = analogRead(SPD_PIN) / 5;
  for( i = 0; string[i] != NULL && string[i] != ' '; i ++ )
  {
    switch(string[i])
    {
      case '.':
        tone(SPK_PIN,TONE,spd);
        delay(spd*2);
        break;
      case '-':
        tone(SPK_PIN,TONE,spd*3);
        delay(spd*4);
        break;
      default:
        delay(2*spd);
    }
  }
}

void setup()
{
  pinMode( SPK_PIN, OUTPUT );
  randomSeed(analogRead(1));
  //string_send("HELLO WORLD.");
}

void loop()
{
  char str[BLK_MAX];
  int i;
  
  for( i = 0; i < BLK_MAX; i ++ ) 
    str[i] = ' ';

  for( i = 0; i < random(BLK_MIN, BLK_MAX); i ++ )
    str[i] = alphabet[random(0,ALPHABET_LEN)];

  string_send(str);
  cw_send(";;;;;");
}
