# wav2mp3

A small tool, which converts all the wav files in a GIVEN folder to mp3 files, and the generated mp3 files are in the same folder.

This code realizes multithreading on all available CPU cores to covert .wav files to mp3 files. This code has been tested on Ubuntu 12.04 OS (linux kernel: 3.13.0-66-generic).
	
	 To run this code:
	 
		1. enter the folder where this file locates
		2. type in the folliwng command in terminal:
				gcc convertWav2Mp3_multithreading.c -o convertWav2Mp3_multithreading -lmp3lame -lpthread
	 	3. type in the adress of folder as the prompt suggests, e.g., /home/li/sounds/wav_files

