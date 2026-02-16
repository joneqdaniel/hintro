#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>
#include <termios.h>
#include "bass.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

typedef BOOL BASSDEF(EXIT)(void);
typedef BOOL BASSDEF(PLAY)(DWORD handle, BOOL restart);
typedef BOOL (CALLBACK INIT)(int freq);
DWORD CALLBACK cb_audio(HSTREAM handle, void *buffer, DWORD length, void *user);

BOOL sound_init(int freq);

typedef struct sound
{
	EXIT*         exit;
	PLAY*         play;
	INIT*         init;
	STREAMPROC*   sink;
	BASS_INFO     info;
	HSTREAM     stream;
	unsigned int  prng;
} SOUND;
SOUND snd = { .exit = BASS_Free, .play = BASS_ChannelPlay, .init = sound_init, .sink = cb_audio };

void sound_halt()
{
	struct termios info;
	snd.exit();
	tcgetattr(STDIN_FILENO, &info);
	info.c_lflag |= ICANON;
	info.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &info);
}

DWORD CALLBACK cb_audio(HSTREAM handle, void *buffer, DWORD length, void *user)
{
	static int cur = 0;
	memset(buffer, 0, length);
	for(int c = 0; c < length / sizeof(float); c+=2)
	{
		float samples;
		const int freq = (cur + snd.prng + 1) / 10;
		const float phase = cur * freq / (float)snd.info.freq;
		((float*)buffer)[c] = sinf(phase *  2 * (float)M_PI);
		((float*)buffer)[c+1] = cosf(phase * 2 * (float)M_PI);
		cur++;
		cur %= snd.info.freq;
	}
	return length;
}

BOOL sound_init(int freq)
{
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion()) != BASSVERSION) {
		fprintf(stderr,"An incorrect version of BASS was loaded\n");
		return false;
	}

	// initialize default output device
	if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
		fprintf(stderr,"Can't initialize device\n");
		return false;
	}
	atexit(sound_halt);

	BASS_GetInfo(&snd.info);
	snd.sink   = cb_audio;
	snd.stream = BASS_StreamCreate(snd.info.freq, 2, BASS_SAMPLE_FLOAT, snd.sink, 0);
	if(snd.stream == 0)
	{
		fprintf(stderr, "Can't create stream!\n");
		return false;
	}
	BASS_ChannelSetAttribute(snd.stream, BASS_ATTRIB_BUFFER, 0);
	struct termios info;
	tcgetattr(0, &info);
	info.c_lflag &= ~ICANON;
	info.c_lflag &= ~ECHO;
	info.c_cc[VMIN] = 1;
	info.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &info);
	return true;
}
