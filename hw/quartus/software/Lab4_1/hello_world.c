/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <math.h>
#include "system.h"
#include "altera_up_avalon_audio.h"
#include <altera_up_avalon_audio_and_video_config.h>
#include <io.h>
#include <limits.h>




#define SAMPLING_FREQ			48000
#define FREQUENCY				200.
#define AMPLITUDE				1.
#define BUFFER_SIZE				100
#define AUDIO_FILE_SIZE			10000
#define MICROPHONE_BUFFER_SIZE	128
#define SECONDS_RECORDING		5
#define RECORDING_SIZE			SECONDS_RECORDING*SAMPLING_FREQ


void generate_sinusoid(int sampling_freq, float freq, char amplitude, unsigned int* buffer);
void play_file(unsigned int *file, int file_size, alt_up_audio_dev *audio_device);

int main()
{
	int i = 0;
	unsigned int *audio_file_pointer, *end_of_recording;
	unsigned int audio_file[AUDIO_FILE_SIZE], recording[RECORDING_SIZE];
	int microphone_buffer[MICROPHONE_BUFFER_SIZE];
	unsigned int fifospace = 0, leftdata = 0, rightdata = 0;
	int incoming_right = 0, incoming_left = 0, n_in_FIFO;
	alt_up_audio_dev *audio_device;
	alt_up_av_config_dev *audio_config;

	audio_device = alt_up_audio_open_dev(AUDIO_0_NAME);
	audio_config = alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME);


	while(!alt_up_av_config_read_ready(audio_config)){
		printf("Audio CODEC not ready\n");
	}

	if (alt_up_av_config_reset(audio_config))	// reset the audio CODEC
		printf("Error resetting the audio CODEC\n");







	generate_sinusoid(SAMPLING_FREQ, FREQUENCY, AMPLITUDE, audio_file);

	while(1)
	{
		play_file(audio_file, AUDIO_FILE_SIZE, audio_device);

		usleep(1000000);

		audio_file_pointer = recording;
		end_of_recording = &(recording[RECORDING_SIZE]);

		printf("Recording...\n");
		while(audio_file_pointer < end_of_recording )
		{
			incoming_left = alt_up_audio_read_fifo_avail(audio_device, 0);
			alt_up_audio_read_fifo(audio_device, audio_file_pointer, incoming_left, 0);

			//play_file(audio_file_pointer, incoming_left, audio_device);

			audio_file_pointer += incoming_left;
		}
		printf("Finished recording...\n");

		play_file(recording, RECORDING_SIZE, audio_device);
	}


	//while (1)
	//{
		 //usleep(1000000);




		/*incoming_left = alt_up_audio_read_fifo_avail(audio_device, 0);
		 incoming_right = alt_up_audio_read_fifo_avail(audio_device, 1);

		 leftdata = alt_up_audio_read_fifo(audio_device, microphone_buffer, incoming_left, 0);
		 rightdata = alt_up_audio_read_fifo(audio_device, microphone_buffer, incoming_right, 1);

		 for(i=0; i<incoming_left; i++)
		 {
			 printf("%f  \n", (float)microphone_buffer[i]/UINT_MAX);
		 }*/
		 //if (incoming_left > 120)
			 //alt_up_audio_reset_audio_core(audio_device);
		/*fifospace = IORD_32DIRECT(AUDIO_0_BASE, 4);
		incoming_left = fifospace & 0x000000FF;
		incoming_right = (fifospace>>8) & 0x000000FF;
		leftdata = IORD_32DIRECT(AUDIO_0_BASE, 8);
		rightdata = IORD_32DIRECT(AUDIO_0_BASE, 12);
		if(incoming_right <= 100)*/
		//printf("%u %u\n", leftdata, rightdata);

	//}

	return 0;
}


void play_file(unsigned int *file, int file_size, alt_up_audio_dev *audio_device)
{
	unsigned int *file_pointer, i;
	for (i=0; i<file_size; i+=BUFFER_SIZE)
	{
		file_pointer = file + i;
		if (file_size - i > BUFFER_SIZE) {
			alt_up_audio_play_r(audio_device, file_pointer, BUFFER_SIZE);
			alt_up_audio_play_l(audio_device, file_pointer, BUFFER_SIZE);
		} else {
			alt_up_audio_play_r(audio_device, file_pointer, file_size - i);
			alt_up_audio_play_l(audio_device, file_pointer, file_size - i);
		}
		while(alt_up_audio_write_fifo_space(audio_device, 0)<BUFFER_SIZE);
	}
}

void generate_sinusoid(int sampling_freq, float freq, char amplitude, unsigned int* buffer)
{
	int i, n_samples, sample, amplitude_factor;

	n_samples = sampling_freq/freq;
	amplitude_factor = UINT_MAX*(float)amplitude/100.0;
	for(i=0; i<AUDIO_FILE_SIZE; i++)
	{
		sample = amplitude_factor *  sin(2 * M_PI * (float)i / n_samples);
		//buffer[i] = sample;
		IOWR_32DIRECT(buffer, i*4, sample);
	}
	printf("Sinusoid created\n");
}
