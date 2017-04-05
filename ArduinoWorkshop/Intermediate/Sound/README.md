# Arduino Intermediate Workshop - Sound





## Functions

```c++
void playNote(char note, unsigned int mod, unsigned int octave);
```

-**note:** A single character for the note to be played. Available notes are: A, B, C, D, E, F, & G.
-**mod:** Note modifier (Sharp and Flat). 
  +0 - No modifier
  +1 - Make the note sharp (higher pitch)
  +2 - Make the note flat (lower pitch)
-**octave:** The octave the note should be in. Range is 0 - 10. Middle C is C4.

