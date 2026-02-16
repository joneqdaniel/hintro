# hintro
Random Synth Tone Generator example using BASS library in C

## Depends

- [C23](https://www.open-std.org/JTC1/SC22/WG14/)
- [CMake](https://www.open-std.org/JTC1/SC22/WG14/)
- [BASS Library](https://www.un4seen.com/bass.html)
- a stereo sound card

## Compile

```sh
cmake .
make
./hintro
```

## Usage

- Press any key to switch random tone
- Press ESC to exit

## Synth

```c
DWORD CALLBACK sound_proc(HSTREAM handle, void *buf, DWORD len, void *user)
{
	static int cur = 0;
	memset(buf, 0, len);

	for(int c = 0; c < len / sizeof(float); c+=2)
	{
		float samples;
		const int     freq =     (snd.prng + cur + 1) / 10;
		const float  phase =     (freq     * cur    ) / (float)snd.info.freq;
		((float*)buf)[c  ] = sinf(phase    *       2  * (float)M_PI);
		((float*)buf)[c+1] = cosf(phase    *       2  * (float)M_PI);
		cur++;
		cur %= snd.info.freq;
	}
	return len;
}
```
