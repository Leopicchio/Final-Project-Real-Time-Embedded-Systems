/*
 * This example uses the Altera IPs "Audio" and "Audio_and_Video_Config" to record sound from a microphone and play it back
 * through an audio jack. To start the recording snap your fingers close to the microphone and follow the instructions.
 */

#include <stdio.h>
#include <math.h>
#include "system.h"
#include "altera_up_avalon_audio.h"
#include <altera_up_avalon_audio_and_video_config.h>
#include "altera_avalon_mailbox_simple.h"
#include "altera_avalon_mailbox_simple_regs.h"
#include <altera_avalon_pio_regs.h>
#include <io.h>
#include <limits.h>


#define SAMPLING_FREQ			48000	// sampling frequency used by the audio CODEC (may be changed to 8kHz to improve speed and reduce memory usage)

// parameters used to generate a sinusoidal beep sound
#define BEEP_FREQUENCY			500.
#define BEEP_AMPLITUDE			1.
#define BEEP_SECONDS			1	// accepts only integers!
#define BEEP_SAMPLES			BEEP_SECONDS * SAMPLING_FREQ	// number of samples in the "beep" file

// parameters used to record a sound from the microphone
#define RECORDING_BUFFER_SIZE	128		// 128 is the size of the FIFO that is used to send/receive data to/from the audio CODEC. A receiving buffer of 128 bytes does just fine
#define RECORDING_SECONDS		2		// NOTE! accepts only integers
#define RECORDING_SAMPLES		RECORDING_SECONDS * SAMPLING_FREQ


#define PIO_LEDS_NUMBER			10	// number of LEDs on the DE1-Soc board

#define LOUDNESS_THRESHOLD		0.5	// sound loudness threshold that starts the recording (in a range from 0 to 1)

//------------------------------------ FUNCTIONS DEFINITION ------------------------------------------
void start_recording(unsigned int *file, unsigned int n_samples, alt_up_audio_dev *audio_device);
float average_amplitude(int *data, int length);
void show_amplitude_on_LEDS(float amplitude);
void generate_sinusoid(int sampling_freq, float freq, char amplitude, unsigned int n_samples, unsigned int *file);
void play_file(unsigned int *file, int file_size, alt_up_audio_dev *audio_device);
void sender_mailbox_callback(void* report, int status);
void receiver_mailbox_callback(void* message);


// information about the file which contains the processed audio (will be received through the mailbox)
unsigned int *processed_file, processed_file_length;
char file_ready_to_play = 0;



//-------------------------------------------- MAIN ------------------------------------------------------
int main()
{
	float sound_loudness = 0;
	unsigned int recording_buffer[RECORDING_BUFFER_SIZE], n_samples_in_FIFO;
	unsigned int beep_sound[BEEP_SAMPLES], recording[RECORDING_SAMPLES];
	alt_up_audio_dev *audio_device;
	alt_up_av_config_dev *audio_config;
	altera_avalon_mailbox_dev *sender_mailbox, *receiver_mailbox;	// handle to the Mailbox
	alt_u32 message[2] =  {0x00001111, 0xaa55aa55};


	/* Open mailbox on sender processor */
	sender_mailbox = altera_avalon_mailbox_open("/dev/mailbox_to_FFT", sender_mailbox_callback, NULL);
	if (!sender_mailbox){
		printf ("FAIL: Unable to open mailbox to other processor\n");
		return 1;
	}
	receiver_mailbox = altera_avalon_mailbox_open("/dev/mailbox_to_Sound_Acquisition", NULL, receiver_mailbox_callback);
	if (!receiver_mailbox){
		printf ("FAIL: Unable to open mailbox from other processor\n");
		return 1;
	}


	audio_device = alt_up_audio_open_dev(AUDIO_0_NAME);		// handle to the circuit that interfaces with the audio CODEC
	audio_config = alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME); // handle to the circuit that initializes the audio CODEC

	// waits for the audio CODEC to be ready
	while(!alt_up_av_config_read_ready(audio_config)){
		printf("Audio CODEC not ready\n");
	}

	// resets the audio CODEC just in case
	if (alt_up_av_config_reset(audio_config))
		printf("Error resetting the audio CODEC\n");


	// initialize the LEDS parallel port
	IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_LEDS_BASE, 0xFFFFFFFF);	// sets all pins as output

	// creates a sinusoid in memory to test the jack output
	printf("Generating sinusoid for beep sound. Please wait...\n");
	generate_sinusoid(SAMPLING_FREQ, BEEP_FREQUENCY, BEEP_AMPLITUDE, BEEP_SAMPLES, beep_sound);
	printf("Sinusoid created\n\n\n");


	// infinite loop that monitors the sound loudness
	printf("Make a loud noise to start recording. Try snapping your fingers.\n");
	while(1)
	{
		// reads the incoming FIFO from the audio CODEC (audio from the microphone)
		n_samples_in_FIFO = alt_up_audio_read_fifo_avail(audio_device, 0);	// checks how many elements are in the FIFO
		alt_up_audio_read_fifo(audio_device, recording_buffer, n_samples_in_FIFO, 0);	// reads the content of the FIFO

		sound_loudness = average_amplitude(recording_buffer, n_samples_in_FIFO);	// computes the sound loudness
		show_amplitude_on_LEDS(sound_loudness);		// shows the amplitude on the LEDs
		if (sound_loudness > LOUDNESS_THRESHOLD)
		{
			printf("Loud noise detected! Recording starting in:\n3...\n");
			play_file(beep_sound, BEEP_SAMPLES, audio_device);
			usleep(500000);
			printf("2...\n");
			play_file(beep_sound, BEEP_SAMPLES, audio_device);
			usleep(500000);
			printf("1...\n");
			play_file(beep_sound, BEEP_SAMPLES, audio_device);
			usleep(500000);

			printf("Recording...\n");
			start_recording(recording, RECORDING_SAMPLES, audio_device);	// starts the recording
			printf("Finished recording...\n");

			message[0] = recording;			// puts the address of the file containing the recording
			message[1] = RECORDING_SAMPLES;	// puts the length of the file
			printf("Sending file information: address: 0x%x \t length: %u \n\n", message[0], message[1]);
			altera_avalon_mailbox_send (sender_mailbox, message, 0, ISR);	// sends the info through the mailbox

			//play_file(recording, RECORDING_SAMPLES, audio_device);			// plays back the recording
		}


		// if the others processor signals that the file is ready: it plays the file
		if (file_ready_to_play)
		{
			play_file(processed_file, processed_file_length, audio_device);			// plays back the recording
			file_ready_to_play = 0;
		}
	}


	altera_avalon_mailbox_close (sender_mailbox); 	// closing the handle to the mailbox
	altera_avalon_mailbox_close (receiver_mailbox); 	// closing the handle to the mailbox
	return 0;
}



// this function plays an audio file trough the output jack (green). To do so it needs to send bursts of data to the outgoing FIFO.
void play_file(unsigned int *file, int file_size, alt_up_audio_dev *audio_device)
{
	unsigned int *file_pointer, i;
	const unsigned int BURST_LENGTH = 100;	// the outgoing FIFO is of 128 elements, if we write burst of 100 at the time there is no distortion (this is an empirical value)

	// sends the data, one burst at a time until the end of the file is reached
	for (i=0; i<file_size; i+=BURST_LENGTH)
	{
		file_pointer = file + i;
		if (file_size - i > BURST_LENGTH) {
			alt_up_audio_play_r(audio_device, file_pointer, BURST_LENGTH);
			alt_up_audio_play_l(audio_device, file_pointer, BURST_LENGTH);
		} else {
			alt_up_audio_play_r(audio_device, file_pointer, file_size - i);
			alt_up_audio_play_l(audio_device, file_pointer, file_size - i);
		}
		while(alt_up_audio_write_fifo_space(audio_device, 0)<BURST_LENGTH);	// waits for the FIFO to get empty
	}
}



/* creates a sinusoidal waveform in the memory (at the location pointed by "unsigned int *file"), n_samples specifies the length*/
// of the file to create.
void generate_sinusoid(int sampling_freq, float freq, char amplitude, unsigned int n_samples, unsigned int *file)
{
	int i, sample_amplitude, amplitude_factor;
	float phase_factor = 2 * M_PI * freq / sampling_freq;	// I break down the computation because it is too slow

	amplitude_factor = UINT_MAX*(float)amplitude/100.0;
	for(i=0; i<n_samples; i++)
	{
		sample_amplitude = amplitude_factor *  sin( phase_factor * i );
		IOWR_32DIRECT(file, i*4, sample_amplitude);
	}
}


// outputs the amplitude on the LEDs
void show_amplitude_on_LEDS(float amplitude)
{
	amplitude = sqrt(amplitude);	// the way we perceive sound is not linear, this is why I take the square root of the amplitude
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_LEDS_BASE, (0x01<<((int)(amplitude*PIO_LEDS_NUMBER)))-1);
}


// computes the average amplitude of an array of sound samples
float average_amplitude(int *data, int length)
{
	int i;
	float average_amplitude = 0;

	// computes the average
	for (i=0; i<length; i++)
	{
		if (data[i] < 0)
			average_amplitude -= data[i];
		else
			average_amplitude += data[i];
	}

	// normalizes to a range from 0 to 1
	average_amplitude /= INT_MAX;
	average_amplitude /= length;
	if (average_amplitude < 0)
		average_amplitude *= -1;

	return average_amplitude;
}



// records a sound from the microphone and stores it in the SDRAM (at location pointed by *file)
void start_recording(unsigned int *file, unsigned int n_samples, alt_up_audio_dev *audio_device)
{
	unsigned int *file_pointer, *end_of_file;
	unsigned int n_samples_in_FIFO = 0;

	if (n_samples > RECORDING_SAMPLES)
	{
		printf("Error, can't record a file so long\n");
		return;
	}
	file_pointer = file;
	end_of_file = &(file[n_samples]);

	while(file_pointer < end_of_file )
	{
		// reads data from the incoming FIFO (sound from the microphone)
		n_samples_in_FIFO = alt_up_audio_read_fifo_avail(audio_device, 0);
		alt_up_audio_read_fifo(audio_device, file_pointer, n_samples_in_FIFO, 0);

		show_amplitude_on_LEDS(average_amplitude(file_pointer, n_samples_in_FIFO));		// shows the amplitude on the LEDs

		file_pointer += n_samples_in_FIFO;
	}
}




void sender_mailbox_callback(void* report, int status) {
	if (status) {
		printf("Transfer done\n");
	} else {
		printf("error in transfer\n");
	}
}


void receiver_mailbox_callback(void* message){
	alt_u32* data = (alt_u32*)message;	// cast from void* to alt_u32*

	if (message != NULL) {
		printf("Message received: msg[0]: 0x%lx   msg[1]: %u\n", data[0], data[1]);
		processed_file = (unsigned int*)data[0];
		processed_file_length = (unsigned int) data[1];
		printf("Received processed file information: address %x \t length: %u \n\n", processed_file, processed_file_length);
		file_ready_to_play = 1;	// raises a flag to start processing the data
	} else {
		printf("Incomplete receive\n");
	}
}

