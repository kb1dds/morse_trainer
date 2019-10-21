// Morse code random blocks trainer
// If an SD card is present with a text file "WORDS.TXT" containing one word per line (all upper case text), it send random words instead of random blocks
// 
// Assumptions:
//  Speaker : D5
//  Speed potentiometer : A0
// If desired, an SD card attached to SPI bus as follows:
//  MOSI :  D11
//  MISO :  D12
//  CLK  :  D13
//  CS   :  D4
//
// Copyright (c) 2019 Michael Robinson

#include <SPI.h>
#include <SD.h>

#define SPK_PIN 5   // Digital pin for speaker
#define SPD_PIN 0   // Analog pin for potentiometer
#define SD_CS_PIN 4 // Chip select for SD card
#define TONE 700    // Tone of the signal in Hz

#define BLK_MIN 3  // Minimum random block length
#define BLK_MAX 5  // Maximum random block length

#define WORD_MAX 15 // Word buffer length, must be longer than BLX_MAX

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890./=";
#define ALPHABET_LEN 39
const char *code[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", 
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", ".-.-.-", "-..-.", "-...-"};

int spd = 125; // Current length of a dot in ms

// SD card details for "words.txt"
unsigned int total_words=0; // Number of words in the SD card file "words.txt".  If zero, then just send random strings of characters
File fp;

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
  Serial.begin(9600);

  // Set up the words.txt file, if it exists
  if (!SD.begin(SD_CS_PIN)) {
    string_send("NO SD.");
    Serial.println("NO SD.");
  }
  else {
    fp = SD.open("words.txt");
    if( fp ){
      // Successfully opened the dictionary.  Count words in the file
      while( fp.available() ){
        if(fp.read() == '\n' )
          total_words ++;
      }
    }
    else{
      string_send("NO WORDS.TXT.");
      Serial.println("NO WORDS.TXT.");
    }
  }
}

void loop()
{
  char str[WORD_MAX], ch;
  int i, wordnum;

  if( total_words == 0 ){  // No words file
    for( i = 0; i < BLK_MAX; i ++ ) 
      str[i] = ' ';

    for( i = 0; i < random(BLK_MIN, BLK_MAX); i ++ )
      str[i] = alphabet[random(0,ALPHABET_LEN)];

    string_send(str);
    Serial.println(str);
    cw_send(";;;;;");
  }
  else {  // Use words file
    fp.seek(0);
    wordnum = random(0,total_words-1);
    for ( i = 0; i < wordnum; i ++ ){ 
      while( fp.read() != '\n' );
    }
    for( i = 0; (i < WORD_MAX) && ((ch=fp.read()) != '\n'); i ++ )
      str[i]=ch;
    str[i]=0;  
    string_send(str);
    Serial.println(str);
    cw_send(";;;;;");
  }
}
