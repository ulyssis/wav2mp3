/*
 * -- 4.12.2015
	README: 

		- This code is to realize multithreading on all available CPU cores to covert .wav files to mp3 files	
		
		- This code has been tested on Ubuntu 12.04 OS (linux kernel: 3.13.0-66-generic).
	

	 To run this code:
	 
		1. enter the folder where this file locates
		2. type in the folliwng command in terminal:
				gcc convertWav2Mp3_multithreading.c -o convertWav2Mp3_multithreading -lmp3lame -lpthread
	 	3. type in the adress of folder as the prompt suggests, e.g., /home/li/sounds/wav_files
		
	
	
	*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "lame/lame.h"
#include <time.h>
#include <pthread.h>


// create thread argument struct
typedef struct _thread_data_t {
  char* file;
  char* folder;
} thread_data_t;

// obtain the URL of folder, which is typed in by user
char* receiveInput(){
    printf("\n Please type in the folder you want to look at:\n");
    char *user_input = "/Users/tang/Documents/technical-interview/wav2mp3/funk_samples";
    scanf("%ms", &user_input);
	return user_input;
}

// xxx.wav -> xxx
char* removePostfix(char* mystr) {
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}

// xxx -> xxx.mp3
char* addPostfix(char *fileNameRemove){
	char * postfix = ".mp3" ;
		strcat(fileNameRemove, postfix);
		printf("After adding the postfix: %s\n", fileNameRemove);
		return fileNameRemove;
}



// fileName is the name of .wav file which is to be converted.
// address is the URL of folder to work
void *convertWav2Mp3(void* arg ){
    thread_data_t *input = (thread_data_t*) arg;
	char *fileName = input->file;
	char* address = input-> folder;
    int read, write;

	// obtain the abosolut path of the file to be converted
  	char addr0[255];
  	strcpy(addr0, address);
	strcat(addr0, "/");			
	strcat(addr0, fileName);	

	FILE *pcm = fopen(addr0, "rb");
	if(!pcm){
		printf("%s is not found!\n", addr0);
		//return;
	}
	else{
	    printf("\nstart to convert %s : \n", addr0);
	}
	
	char * fileNameRemove = removePostfix(fileName);    
	char *fileNameNew = addPostfix(fileNameRemove);
	
	// obtain the abosolut path of the file to be created
  	char addr[255];
  	strcpy(addr, address);
	strcat(addr, "/");
	strcat(addr, fileNameNew);
    FILE *mp3 = fopen(addr, "wb");
	/*    printf("The newly generated file is: %s \n", addr);*/

    const int PCM_SIZE = 8192;
    const int MP3_SIZE = 8192;

    short int pcm_buffer[PCM_SIZE*2];
    unsigned char mp3_buffer[MP3_SIZE];

    lame_t lame = lame_init();
	/*    lame_set_num_channels(lame,2);//1 is single channel, 2 is the default setting*/
    lame_set_in_samplerate(lame, 44100); // consumer audio like CDs use 44.1KHz sampling,
    lame_set_VBR(lame, vbr_default);
    
	/*            lame_set_brate(lame,8);*/
	/*            lame_set_mode(lame,3);*/
	/*            lame_set_quality(lame,2); // 2=high 5 = medium  7=low*/
    lame_init_params(lame);
    do {
    	printf("converting %s ...\n", fileNameNew);
        read = fread(pcm_buffer, 2*sizeof(short int), PCM_SIZE, pcm);
        if (read == 0)
            {
            write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
            }
        else{
            write = lame_encode_buffer_interleaved(lame, pcm_buffer, read, mp3_buffer, MP3_SIZE);
            }
        fwrite(mp3_buffer, write, 1, mp3);
        
    } while (read != 0);

    lame_close(lame);
    fclose(mp3);
    fclose(pcm);
    
    pthread_exit(NULL);
}



int main(int argc, char *argv[])
{
	clock_t begin, end;
	double time_spent;
	
//	begin = clock();

	// get the address of folder which is to be searched from user input.
    //char *folderAddr = receiveInput();
	char *folderAddr = argv[1];
    printf("The folder address is %s\n", folderAddr);

	// Obtain the number of wav files
	DIR *dpdf;

	dpdf = opendir(folderAddr);	
	int numberFile =0;
	
	if (dpdf != NULL){
		char *fileName;
		struct dirent *epdf;
		epdf = readdir(dpdf);
		int rc;

	    while (epdf){
	   	   	fileName = epdf->d_name;

			// Iterate all the files in the given folder
			if(strlen(fileName) >= strlen(".wav")){
		        if(!strcmp(fileName + strlen(fileName) - strlen(".wav"), ".wav"))
			        {

					// printf("Find one .wav file\n");
			        numberFile ++;

		    		pthread_t thr;  // array of function identifiers
		    		thread_data_t thr_data;	// array of parameters to be passed into thread

					thr_data.file = malloc(strlen(fileName) + 1);
					strcpy(thr_data.file, fileName);
						
					thr_data.folder = malloc(strlen(folderAddr) + 1);
					strcpy(thr_data.folder, folderAddr);

					if ( (rc = pthread_create(&thr, NULL, convertWav2Mp3, &thr_data)) ) {
						 fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
						 return EXIT_FAILURE;
						}
						else{
							pthread_join(thr, NULL);
						}

						free(thr_data.file);
						free(thr_data.folder);
			        }
			}
			epdf = readdir(dpdf);
	    }

	    printf("The number of files is: %d\n", numberFile);

	   }else{
		printf("The input address can not be found, please check the input.\n");
	}
	
   	closedir(dpdf);
	free(folderAddr);

//	end = clock();
//	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//	printf("The consumed time is: %f\n", time_spent);
    return EXIT_SUCCESS;
}

