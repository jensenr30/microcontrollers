This was an attempt to create a frequency counter using an ATtiny24A.
The program size kept being compiled too big (over 2Kbytes)
That was very limiting.
I decided to migrate the project to an ATmega328.
Then I migrated the project BACK to the ATtiny24A platform, but used the 84A.
The ATtiny84A is compatible with the 24A.
The difference between the two is that the 84A has 8K memory whereas the 24A only has 2K.

