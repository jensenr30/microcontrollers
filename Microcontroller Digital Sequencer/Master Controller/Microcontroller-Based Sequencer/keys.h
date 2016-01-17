/* keys.h
 * Ryan Jensen
 * Date:
 * This header file contains definitions for all 88 keys on a traditional piano.
 * The n_A4, n_C2, n_f5, etc... definitions are just for enumeration. They fit into a char data type
 * 
 */


/// KEY ENUMERATION DEFINITIONS
#define n_same 0		// play the same note as before
#define n_none 127		// play no note. play nothing.
#define n_A0  1
#define n_A0s 2
#define n_B0  3
#define n_C1  4
#define n_C1s 5
#define n_D1  6
#define n_D1s 7
#define n_E1  8
#define n_F1  9
#define n_F1s 10
#define n_G1  11
#define n_G1s 12
#define n_A1  13
#define n_A1s 14
#define n_B1  15
#define n_C2  16
#define n_C2s 17
#define n_D2  18
#define n_D2s 19
#define n_E2  20
#define n_F2  21
#define n_F2s 22
#define n_G2  23
#define n_G2s 24
#define n_A2  25
#define n_A2s 26
#define n_B2  27
#define n_C3  28
#define n_C3s 29
#define n_D3  30
#define n_D3s 31
#define n_E3  32
#define n_F3  33
#define n_F3s 34
#define n_G3  35
#define n_G3s 36
#define n_A3  37
#define n_A3s 38
#define n_B3  39
#define n_C4  40
#define n_C4s 41
#define n_D4  42
#define n_D4s 43
#define n_E4  44
#define n_F4  45
#define n_F4s 46
#define n_G4  47
#define n_G4s 48
#define n_A4  49
#define n_A4s 50
#define n_B4  51
#define n_C5  52
#define n_C5s 53
#define n_D5  54
#define n_D5s 55
#define n_E5  56
#define n_F5  57
#define n_F5s 58
#define n_G5  59
#define n_G5s 60
#define n_A5  61
#define n_A5s 62
#define n_B5  63
#define n_C6  64
#define n_C6s 65
#define n_D6  66
#define n_D6s 67
#define n_E6  68
#define n_F6  69
#define n_F6s 70
#define n_G6  71
#define n_G6s 72
#define n_A6  73
#define n_A6s 74
#define n_B6  75
#define n_C7  76
#define n_C7s 77
#define n_D7  78
#define n_D7s 79
#define n_E7  80
#define n_F7  81
#define n_F7s 82
#define n_G7  83
#define n_G7s 84
#define n_A7  85
#define n_A7s 86
#define n_B7  87
#define n_C8  88


/// KEY FREQUENCY DEFINITIONS
#define f_same 123456789.0	// this frequency will never be played. it is simply a flag.
#define f_none 0.0			// this is no frequency. nothing being played.
#define f_A0  27.500000
#define f_A0s 29.135235
#define f_B0  30.867706
#define f_C1  32.703197
#define f_C1s 34.647827
#define f_D1  36.708096
#define f_D1s 38.890873
#define f_E1  41.203445
#define f_F1  43.653530
#define f_F1s 46.249302
#define f_G1  48.999428
#define f_G1s 51.913086
#define f_A1  55.000000
#define f_A1s 58.270470
#define f_B1  61.735413
#define f_C2  65.406395
#define f_C2s 69.295654
#define f_D2  73.416191
#define f_D2s 77.781746
#define f_E2  82.406891
#define f_F2  87.307060
#define f_F2s 92.498604
#define f_G2  97.998856
#define f_G2s 103.826172
#define f_A2  110.000000
#define f_A2s 116.540939
#define f_B2  123.470825
#define f_C3  130.812790
#define f_C3s 138.591309
#define f_D3  146.832382
#define f_D3s 155.563492
#define f_E3  164.813782
#define f_F3  174.614120
#define f_F3s 184.997208
#define f_G3  195.997711
#define f_G3s 207.652344
#define f_A3  220.000000
#define f_A3s 233.081879
#define f_B3  246.941650
#define f_C4  261.625580
#define f_C4s 277.182617
#define f_D4  293.664764
#define f_D4s 311.126984
#define f_E4  329.627563
#define f_F4  349.228241
#define f_F4s 369.994415
#define f_G4  391.995422
#define f_G4s 415.304688
#define f_A4  440.000000
#define f_A4s 466.163757
#define f_B4  493.883301
#define f_C5  523.251160
#define f_C5s 554.365234
#define f_D5  587.329529
#define f_D5s 622.253967
#define f_E5  659.255127
#define f_F5  698.456482
#define f_F5s 739.988831
#define f_G5  783.990845
#define f_G5s 830.609375
#define f_A5  880.000000
#define f_A5s 932.327515
#define f_B5  987.766602
#define f_C6  1046.502319
#define f_C6s 1108.730469
#define f_D6  1174.659058
#define f_D6s 1244.507935
#define f_E6  1318.510254
#define f_F6  1396.912964
#define f_F6s 1479.977661
#define f_G6  1567.981689
#define f_G6s 1661.218750
#define f_A6  1760.000000
#define f_A6s 1864.655029
#define f_B6  1975.533203
#define f_C7  2093.004639
#define f_C7s 2217.460938
#define f_D7  2349.318115
#define f_D7s 2489.015869
#define f_E7  2637.020508
#define f_F7  2793.825928
#define f_F7s 2959.955322
#define f_G7  3135.963379
#define f_G7s 3322.437500
#define f_A7  3520.000000
#define f_A7s 3729.310059
#define f_B7  3951.066406
#define f_C8  4186.009277



float get_freq(unsigned char keyNumber){
	
	switch(keyNumber){
		case n_same: return f_same;
		case n_A0:  return f_A0;
		case n_A0s: return f_A0s;
		case n_B0:  return f_B0;
		case n_C1:  return f_C1;
		case n_C1s: return f_C1s;
		case n_D1:  return f_D1;
		case n_D1s: return f_D1s;
		case n_E1:  return f_E1;
		case n_F1:  return f_F1;
		case n_F1s: return f_F1s;
		case n_G1:  return f_G1;
		case n_G1s: return f_G1s;
		case n_A1:  return f_A1;
		case n_A1s: return f_A1s;
		case n_B1:  return f_B1;
		case n_C2:  return f_C2;
		case n_C2s: return f_C2s;
		case n_D2:  return f_D2;
		case n_D2s: return f_D2s;
		case n_E2:  return f_E2;
		case n_F2:  return f_F2;
		case n_F2s: return f_F2s;
		case n_G2:  return f_G2;
		case n_G2s: return f_G2s;
		case n_A2:  return f_A2;
		case n_A2s: return f_A2s;
		case n_B2:  return f_B2;
		case n_C3:  return f_C3;
		case n_C3s: return f_C3s;
		case n_D3:  return f_D3;
		case n_D3s: return f_D3s;
		case n_E3:  return f_E3;
		case n_F3:  return f_F3;
		case n_F3s: return f_F3s;
		case n_G3:  return f_G3;
		case n_G3s: return f_G3s;
		case n_A3:  return f_A3;
		case n_A3s: return f_A3s;
		case n_B3:  return f_B3;
		case n_C4:  return f_C4;
		case n_C4s: return f_C4s;
		case n_D4:  return f_D4;
		case n_D4s: return f_D4s;
		case n_E4:  return f_E4;
		case n_F4:  return f_F4;
		case n_F4s: return f_F4s;
		case n_G4:  return f_G4;
		case n_G4s: return f_G4s;
		case n_A4:  return f_A4;
		case n_A4s: return f_A4s;
		case n_B4:  return f_B4;
		case n_C5:  return f_C5;
		case n_C5s: return f_C5s;
		case n_D5:  return f_D5;
		case n_D5s: return f_D5s;
		case n_E5:  return f_E5;
		case n_F5:  return f_F5;
		case n_F5s: return f_F5s;
		case n_G5:  return f_G5;
		case n_G5s: return f_G5s;
		case n_A5:  return f_A5;
		case n_A5s: return f_A5s;
		case n_B5:  return f_B5;
		case n_C6:  return f_C6;
		case n_C6s: return f_C6s;
		case n_D6:  return f_D6;
		case n_D6s: return f_D6s;
		case n_E6:  return f_E6;
		case n_F6:  return f_F6;
		case n_F6s: return f_F6s;
		case n_G6:  return f_G6;
		case n_G6s: return f_G6s;
		case n_A6:  return f_A6;
		case n_A6s: return f_A6s;
		case n_B6:  return f_B6;
		case n_C7:  return f_C7;
		case n_C7s: return f_C7s;
		case n_D7:  return f_D7;
		case n_D7s: return f_D7s;
		case n_E7:  return f_E7;
		case n_F7:  return f_F7;
		case n_F7s: return f_F7s;
		case n_G7:  return f_G7;
		case n_G7s: return f_G7s;
		case n_A7:  return f_A7;
		case n_A7s: return f_A7s;
		case n_B7:  return f_B7;
		case n_C8:  return f_C8;
		default:    return f_none;
	}
}
