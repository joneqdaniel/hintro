#include "hintro_bass.h"

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
