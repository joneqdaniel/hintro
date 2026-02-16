#include "hintro_bass.h"

DWORD CALLBACK sound_proc(HSTREAM handle, void *buffer, DWORD length, void *user)
{
	static int cur = 0;
	memset(buffer, 0, length);
	for(int c = 0; c < length / sizeof(float); c+=2)
	{
		float samples;
		const int        freq = (cur + snd.prng + 1) / 10;
		const float     phase = cur * freq / (float)snd.info.freq;
		((float*)buffer)[c  ] = sinf(phase * 2 * (float)M_PI);
		((float*)buffer)[c+1] = cosf(phase * 2 * (float)M_PI);
		cur++;
		cur %= snd.info.freq;
	}
	return length;
}

int main(int argc, char** argv)
{
	if(!snd.init(44100)) exit(EXIT_FAILURE);

	if(snd.play(snd.stream, FALSE))
	{
		char c = 0;
		while((c = getc(stdin)) >= 0 && c != 27)
			snd.prng = rand();
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}
