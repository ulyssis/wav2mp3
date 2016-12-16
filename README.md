# wav2mp3
# author: DI LI
# Dec. 2016

This program converts all the wav files in a GIVEN folder to mp3 files, and the generated mp3 files are in the same folder.

This code realizes multithreading on all available CPU cores to covert .wav files to mp3 files. This code has been tested on Ubuntu 12.04 OS (linux kernel: 3.13.0-66-generic) and mac OS.
	
	 To run this code:
	 
		1. enter the folder where this file locates
		2. type in the folliwng command in terminal:

			gcc -I/usr/local/include/ -c convertWav2Mp3_multithreading.c
			and		
			gcc -o wav2mp3 convertWav2Mp3_multithreading.o -L/usr/local/lib -lpthread -lmp3lame
				
	 	3. type in the adress of folder as the prompt suggests, example command is:
			./wav2mp3 /sounds/wav_files

