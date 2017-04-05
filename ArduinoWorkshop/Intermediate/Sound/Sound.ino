
#define SPEAKER_PIN 9

unsigned int duty = 50;

void setup() {
  // initialize speaker pin as an output.
  pinMode(SPEAKER_PIN, OUTPUT);
  // start serial monitor.
  Serial.begin(9600);

  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 41;                // 50KHz Freq.
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);    // 8 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts(); // enable interrupts.
  Serial.println("Setup complete.");
}

// the loop function runs over and over again forever
void loop() {
  // playNote('B', 0, 5); // play a musical note.
  // delay(1000);
  // setFreq(500); // set frequency manually.
  // delay(1000);
  // turnOff(); // stop playing sounds.
  // delay(1000);
  // playNoteTime('C', 1, 4, 1000); // play note for duration.
  // delay(1000);
  // playTheme1();
  // delay(1000);
  readNote(); // read notes from serial monitor.
}

bool pinState = LOW;
int freqCounter = 0;
unsigned int onTime, offTime = 0;

ISR(TIMER1_COMPA_vect)
{
  // turn it off.
  if (onTime == 0) {
      digitalWrite(SPEAKER_PIN, LOW);
  }
  // start next cycle.
  else if (freqCounter <= 0) {
    pinState = !pinState;
    digitalWrite(SPEAKER_PIN, pinState);
    if (pinState) {
      freqCounter = onTime;
    }
    else {
      freqCounter = offTime;
    }
  }
  // count down to 0.
  else {
    freqCounter--;
  }
} 

// set the timings to a given frequency.
void setFreq(float freq) {
  if (freq == 0) {
    onTime = 0;
    offTime = 0;
  }
  else {
    onTime = (50000 / freq) * (100 / duty);
    offTime = (50000 / freq) * (100 / (100 - duty));
  }
}

// turn off sound.
void turnOff() {
  setFreq(0);
}


/////////// notes defined //////////
const float C = 16.35;
const float Cs = 17.32;
const float Db = Cs;
const float D = 18.35;
const float Ds = 19.45;
const float Eb = Ds;
const float E = 20.60;
const float Es = 21.83;
const float Fb = E;
const float F = Es;
const float Fs = 23.12;
const float Gb = Fs;
const float G = 24.50;
const float Gs = 25.96;
const float Ab = Gs;
const float A = 27.50;
const float As = 29.14;
const float Bb = As;
const float B = 30.87;
const float Bs = C * 2;
const float Cb = B;
/////////// end notes //////////

void playNote(char note, unsigned int mod, unsigned int octave) {
  float ret;
  if (octave > 10) {
      Serial.print("Octave ");
      note = 0;
      octave = 0;
  }
  switch(note) {
    case 'A':
    case 'a':
      switch (mod) {
        case 0:
          ret = A;
          break;
        case 1:
          ret = As;
          break;
        case 2:
          ret = Ab;
          break;
      }
      break;
    case 'B':
    case 'b':
      switch (mod) {
        case 0:
          ret = B;
          break;
        case 1:
          ret = Bs;
          break;
        case 2:
          ret = Bb;
          break;
      }
      break;
    case 'C':
    case 'c':
      switch (mod) {
        case 0:
          ret = C;
          break;
        case 1:
          ret = Cs;
          break;
        case 2:
          ret = Cb;
          break;
      }
      break;
    case 'D':
    case 'd':
      switch (mod) {
        case 0:
          ret = D;
          break;
        case 1:
          ret = Ds;
          break;
        case 2:
          ret = Db;
          break;
      }
      break;
    case 'E':
    case 'e':
      switch (mod) {
        case 0:
          ret = E;
          break;
        case 1:
          ret = Es;
          break;
        case 2:
          ret = Eb;
          break;
      }
      break;
    case 'F':
    case 'f':
      switch (mod) {
        case 0:
          ret = F;
          break;
        case 1:
          ret = Fs;
          break;
        case 2:
          ret = Fb;
          break;
      }
      break;
    case 'G':
    case 'g':
      switch (mod) {
        case 0:
          ret = G;
          break;
        case 1:
          ret = Gs;
          break;
        case 2:
          ret = Gb;
          break;
      }
      break;
    case '0':
      ret = 0;
      Serial.println("Turning Off.");
      break;
    default:
      Serial.println("Input Error.");
      ret = 0;
  }
  if (ret != 0) {
    Serial.print("Note Received: ");
    Serial.print(note);
    if (mod == 1) {
      Serial.print("s");
    }
    else if (mod == 2) {
      Serial.print("b");
    }
    Serial.println(octave);
  }
  setFreq(ret * pow(2, octave));
}

void playNoteTime(char note, unsigned int mod, unsigned int octave, unsigned int durationMS) {
  playNote(note, mod, octave);
  delay(durationMS);
  turnOff();
}

unsigned int readNote() {
  char note;
  unsigned int modIn, mod, octave = 0;
  while (Serial.available() < 2);
  note = Serial.read();
  modIn = Serial.read();
  if (modIn == 115 || modIn == 98) {
    if (modIn == 115) {
      mod = 1;
    }
    else {
      mod = 2;
    }
    while(Serial.available() < 1);
    octave = Serial.read() - 48;
  }
  else {
    octave = modIn - 48;
    mod = 0;
  }
  // Flush remaining characters.
  while (Serial.available() > 0) {
    Serial.read();
  }
  playNote(note, mod, octave);
}

// Imperial March from http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0017607
void playTheme1() {
  int i = 0;
  // phrase 1
  for (i = 0; i < 2; i++) {
    playNoteTime('G', 0, 4, 490);
    delay(10);
  }
  for (i = 0; i < 2; i++) {
    playNoteTime('G', 0, 4, 490);
    delay(10);
    playNoteTime('E', 2, 4, 365);
    delay(10);
    playNoteTime('B', 2, 5, 115);
    delay(10);
  }
  playNoteTime('G', 0, 4, 990);
  delay(10);

  // phrase 2
  for (i = 0; i < 3; i++) {
    playNoteTime('D', 0, 5, 490);
    delay(10);
  }
  playNoteTime('E', 2, 5, 365);
  delay(10);
  playNoteTime('B', 2, 5, 115);
  delay(10);
  playNoteTime('G', 2, 4, 490);
  delay(10);
  playNoteTime('E', 2, 4, 365);
  delay(10);
  playNoteTime('B', 2, 5, 115);
  delay(10);
  playNoteTime('G', 2, 4, 990);
  delay(10);
}

