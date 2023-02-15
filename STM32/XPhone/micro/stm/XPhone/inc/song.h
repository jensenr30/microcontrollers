#ifndef SONG_H_DEF
	#define SONG_H_DEF
	#include "GPIO.h"
	#include "main.h"
	#include "key.h"

	//=============================================================================
	// definitions for song stuff
	//=============================================================================
	// song timer stuff
	#define SONG_TIM TIM3									// The song timer is driven by timer 3 (16-bit timer)
	#define SONG_TIM_FREQ		((uint16_t)10000)			// 10 kHz input freq
	#define SONG_TIM_PSC		(CPU_FREQ/SONG_TIM_FREQ - 1)// calculate the prescaler
	#define SONG_TIM_ARR		(10 - 1)					// reset to 0 on the 10th value (this will give a period of 1 ms)
	#define SONG_TICKS_PER_SEC	((uint32_t)1000)			// the time resolution of the song is 1 ms. (1000 of them per second)
	#define SongTimeSec(t) 		(t/(float)SONG_TICKS_PER_SEC)// convert key time to seconds
	#define SONG_LATE_MAX_PLAY_MS	(10)					// sometimes the XPhone is running a little bit late.  This specifies how late it can still play a note.  If the XPhone is more than this number of milliseconds late, it will simply forego playing the late note to avoid muddying/confusing the song playback.

	// song variables
	volatile KeyTimeType SongTime;							// this is our current position in the song (ms)
	volatile KeyTimeType SongLength;						// this is how long the song is (ms). Defaults to maximum time (4294967295 milliseconds, or about 1.2 hours)
	volatile uint8_t SongWasReset;							// this indicates that the song was reset and that the main() while() loop must reset the noteToPlay to songCurrent.  This must happen inside main() to prevent the program from becoming unpredictable / or introducing race conditions.
	Note *songCurrent;										// this points to the current song you are playing
	Note *noteToPlay;										// this will point to the next note you are going to play
	uint32_t SongNotesTotal;								// this keeps track of the total number of notes the song has. This isn't truely necessary for making the song work, but it serves as a check when we clear a song to make sure the number of notes cleared equals the number of notes in the song.
	uint16_t SongSkipNextNotes;								// if you set this to 5, it will ignore the next 5 notes. 
	
	// TODO implement a way to limit to total possible notes
	
	//=============================================================================
	// song functions
	//=============================================================================
	
	void song_init();
	void song_set_to_beginning();
	void TIM3_IRQHandler(void);
	void song_clear(Note *song);
	
	
#endif // SONG_H_DEF
