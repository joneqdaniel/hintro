#include "hintro_bass.h"

DWORD CALLBACK sound_proc(HSTREAM handle, void *buf, DWORD len, void *user)
{
	const SOUND* cfg = (SOUND*)user;
	static int cur = 0;
	memset(buf, 0, len);

	for(int c = 0; c < len / sizeof(float); c+=2)
	{
		float samples;
		const int     freq =     (cfg->prng + cur + 1) / 10;
		const float  phase =     (freq      * cur    ) / (float)cfg->info.freq;
		((float*)buf)[c  ] = sinf(phase     *       2  * (float)M_PI);
		((float*)buf)[c+1] = cosf(phase     *       2  * (float)M_PI);
		cur++;
		cur %= cfg->info.freq;
	}
	return len;
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
