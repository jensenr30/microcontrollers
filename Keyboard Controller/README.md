Keyboard_Controller
===================

This program will allow an ATmega328P microcontroller to interpret user key events on an electric keyboard.

I will most likely use a DAC to generate analog control voltages from the microcontroller directly.

In addition, there will need to be some gate outputs that go high when a note is held down.



This will be a monophonic CV generator at first.
However, I'm thinking I might want to allow the highest and lowest notes held down to be played through two different CVs.
This would allow the user to play a bassline with one hand and some treble with the other hand.
I may want to add a split feature if I do decide to implement this.
The split point would be able to be manually set by the user.

Also, I need to have octave shift buttons.
There will need to be two octave shift keys: one for the top and one for the bottom.

It would be nice to have some analog portamento implemented with RC circuits as well (with adjustable time constants).
Because the user will have the ability to split the keyboard, there must be two different portamento circuits (Two RC circuits with tunable time constants.)
There will be a potentiometer, but It might be a good idea to have a few caps on switch to allow large changes of RC constant.
