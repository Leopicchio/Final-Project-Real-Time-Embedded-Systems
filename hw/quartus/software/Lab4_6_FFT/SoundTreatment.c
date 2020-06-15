#include <stdio.h>
#include "altera_avalon_mailbox_simple.h"
#include "altera_avalon_mailbox_simple_regs.h"
#include <altera_avalon_pio_regs.h>
#include "system.h"
#include "sys/alt_irq.h"
#include "kiss_fft.h"
#include <stdbool.h>


// the masks that are used to know which effect is activated
#define EFFECT_REVERSE  		0b0000000001
#define EFFECT_ECHO				0b0000000010
#define EFFECT_REVERBERATION	0b0000000100
#define EFFECT_MOVING_AVERAGE	0b0000001000
#define EFFECT_CHANGE_PITCH		0b0000010000


#define SAMPLING_FREQ_Hz	48000	// sampling frequency used by the audio CODEC (Hertz)
#define SAMPLING_FREQ_kHz	48		// sampling frequency used by the audio CODEC (kHz)

// PARAMETERS OF THE EFFECTS
#define ECHO_DELAY_MS		500		// milliseconds
#define ECHO_AMPLITUDE		0.5		// in range from 0 to 1
#define REVERB_DELAY_MS		200		// milliseconds
#define REVERB_DECAY		0.3		// in range from 0 to 1
#define MOVING_AVERAGE_N	100		// number of samples over which perform the moving average (cuts frequencies above SAMPL_FREQ/N_SAMPLES_AVERAGE)
#define PITCH_CHANGE		1		// value added to the FFT bins to change the pitch
#define FFT_SIZE			16 		// size of the FFT

#define FIFO_SIZE		5

#ifndef M_PI
#define M_PI 3.14159265358979324
#endif


void receiver_mailbox_callback(void* message);
void sender_mailbox_callback(void* report, int status);
void add_echo(unsigned int *file_address, unsigned int file_length, unsigned int delay_ms, float amplitude);
void add_reverberation(unsigned int *file_address, unsigned int file_length, unsigned int delay_ms, float decay);
void reverse(unsigned int *file_address, unsigned int file_length);
void moving_average(unsigned int *file_address, unsigned int file_length, unsigned int n_samples);
void SilentFft(const char* title, const kiss_fft_cpx in[FFT_SIZE], kiss_fft_cpx out[FFT_SIZE], int inverse);
void modify_pitch(unsigned int *file_address, unsigned int file_length, unsigned int pitch_change);


typedef struct AudioFile
{
	unsigned int *start_address;
	unsigned int *file_pointer;
	unsigned int *end_address;
	bool processed;
}AudioFile;

AudioFile audio_file[FIFO_SIZE];
int FIFO_start = 0, FIFO_length = 0;

//arrays for the FFT
kiss_fft_cpx time_domain[FFT_SIZE], freq_domain[FFT_SIZE];

int main()
{
	alt_u32 message[2];
	altera_avalon_mailbox_dev *sender_mailbox, *receiver_mailbox;	// handle to the Mailbox
	int activated_audio_effects = 0, i;	// each bit of this variable represents an effect which can be activated by toggling the switches
	unsigned int* file_address, file_length;

	// initialize the LEDS parallel port
	IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_SWITCHES_BASE, 0x00000000);	// sets all pins as input

	// Open mailbox
	receiver_mailbox = altera_avalon_mailbox_open("/dev/mailbox_to_FFT", NULL, receiver_mailbox_callback);
	if (!receiver_mailbox) {
		printf("FAIL: Unable to open mailbox from other processor\n");
		return 1;
	}
	sender_mailbox = altera_avalon_mailbox_open("/dev/mailbox_to_Sound_Acquisition", sender_mailbox_callback, NULL);
	if (!sender_mailbox) {
		printf("FAIL: Unable to open mailbox to other processor\n");
		return 1;
	}

	printf("Hello, this is FFT processor\n");
	while(1)
	{
		if(FIFO_length > 0)
		{

			file_address = audio_file[FIFO_start].start_address;
			file_length = audio_file[FIFO_start].end_address - audio_file[FIFO_start].start_address;

			FIFO_length--;
			FIFO_start++;
			if(FIFO_start >= FIFO_SIZE)
				FIFO_start = 0;


			printf("\n\n\n\n----------------------------------------------------\n");
			printf("Processing file:\n");
			printf("\tADDRESS:\t0x%x\n\tLENGHT:\t%u samples\n", file_address, file_length);

			// read the switches to see which effects are active
			activated_audio_effects = IORD_ALTERA_AVALON_PIO_DATA(PIO_SWITCHES_BASE);

			// TO DO:    FFT

			// check which effects are active and apply them
			printf("LIST OF AUDIO EFFECTS APPLIED:\n");
			if (activated_audio_effects & EFFECT_REVERSE)	// if this effect is active
			{
				printf("\tReversing audio...\n");
				reverse(file_address, file_length);
				printf("\tAudio reversed\n");
			}
			if (activated_audio_effects & EFFECT_ECHO)
			{
				printf("\tApplying echo...\n");
				add_echo(file_address, file_length, ECHO_DELAY_MS, ECHO_AMPLITUDE);
				printf("\tEcho applied\n");
			}
			if (activated_audio_effects & EFFECT_REVERBERATION)
			{
				printf("\tApplying reverberation...\n");
				add_reverberation(file_address, file_length, REVERB_DELAY_MS, REVERB_DECAY);
				printf("\tReverberation applied\n");
			}
			if (activated_audio_effects & EFFECT_MOVING_AVERAGE)
			{
				printf("\tApplying moving average...\n");
				moving_average(file_address, file_length, MOVING_AVERAGE_N);
				printf("\tMoving average applied\n");
			}
			if (activated_audio_effects & EFFECT_CHANGE_PITCH)
			{
				printf("\tApplying pitch modification...\n");
				modify_pitch(file_address, file_length, PITCH_CHANGE);
				printf("\tPitch change applied\n");
			}
			printf("----------------------------------------------------\n");
			// sends to the other processor the address and the length of the processed file
			printf("Finished processing.\n");
			printf("Sending processed file to the other processor...\n");

			message[0] = file_address;
			message[1] = file_address + file_length;
			altera_avalon_mailbox_send (sender_mailbox, message, 0, ISR);	// sends the info through the mailbox
		}


	}

	altera_avalon_mailbox_close(receiver_mailbox);
	altera_avalon_mailbox_close(sender_mailbox);
	return 0;
}





// Get message read from mailbox
void receiver_mailbox_callback(void* message) {

	alt_u32* data = (alt_u32*)message;	// cast from void* to alt_u32*
	int index;

	if (message != NULL) {
		//printf("Message received from sound acquisition CPU: msg[0]: %lx   msg[1]: %lx\n", data[0], data[1]);

		if(FIFO_length < FIFO_SIZE)
			FIFO_length++;
		else
			printf("Not enough space in FIFO\n");

		index = (FIFO_start + FIFO_length - 1)%FIFO_SIZE;

		audio_file[index].start_address = (unsigned int*)data[0];
		audio_file[index].end_address = (unsigned int*) data[1];
	} else {
		printf("[FFT]Incomplete receive\n");
	}
}





void sender_mailbox_callback(void* report, int status) {
	if (status) {
		printf("Processed file sent with success.\n");
	} else {
		printf("Error in transfer\n");
	}
}





void add_reverberation(unsigned int *file_address, unsigned int file_length, unsigned int delay_ms, float decay)
{
	unsigned int i, samples_delay;
	float sample_amplitude;

	samples_delay = delay_ms*SAMPLING_FREQ_kHz;
	for (i=samples_delay; i<file_length; i++)
	{
		sample_amplitude = IORD_32DIRECT(file_address, (i-samples_delay)*4) * decay;
		sample_amplitude += IORD_32DIRECT(file_address, i*4);
		IOWR_32DIRECT(file_address, i*4, sample_amplitude);
	}
}





void add_echo(unsigned int *file_address, unsigned int file_length, unsigned int delay_ms, float amplitude)
{
	unsigned int i, samples_delay;
	float sample_amplitude;

	samples_delay = delay_ms*SAMPLING_FREQ_kHz;
	for (i=file_length; i>samples_delay; i--)
	{
		sample_amplitude = IORD_32DIRECT(file_address, (i-samples_delay)*4) * amplitude;
		sample_amplitude += IORD_32DIRECT(file_address, i*4);
		IOWR_32DIRECT(file_address, i*4, sample_amplitude);
	}
}


void reverse(unsigned int *file_address, unsigned int file_length)
{
	unsigned int i, last_sample, first_sample;

	for (i=0; i<file_length/2; i++)
	{
		// swaps the first with the last sample (for all samples)
		last_sample = IORD_32DIRECT(file_address, (file_length-i)*4);
		first_sample = IORD_32DIRECT(file_address, i*4);
		IOWR_32DIRECT(file_address, i*4, last_sample);
		IOWR_32DIRECT(file_address, (file_length-i)*4, first_sample);
	}
}


void moving_average(unsigned int *file_address, unsigned int file_length, unsigned int n_samples)
{
	int i, moving_average = 0, sample, last_sample;
	float  coeff = 1./n_samples;	// transform division into multiplication for speed

	moving_average = 0;
	for (i=file_length-n_samples; i<file_length; i++)
	{
		sample = IORD_32DIRECT(file_address, i*4);
		moving_average += sample * coeff;
	}

	for (i=file_length; i>n_samples; i--)
	{
		last_sample = IORD_32DIRECT(file_address, i*4);
		IOWR_32DIRECT(file_address, i*4, moving_average);

		moving_average -= last_sample * coeff;
		moving_average += IORD_32DIRECT(file_address, (i-n_samples)*4) * coeff;
	}
}




void modify_pitch(unsigned int *file_address, unsigned int file_length, unsigned int pitch_change)
{
	unsigned int j, i, n_batches;

	n_batches = file_length/FFT_SIZE;

	for (j=0; j<n_batches; j++)
	{
		for (i = 0; i < FFT_SIZE; i++){
			time_domain[i].r = IORD_32DIRECT(file_address, (j*FFT_SIZE + i)*4);
			time_domain[i].i = 0;
		}
		SilentFft("Signal (complex)", time_domain, freq_domain, 0);	//calculate FFT

		for(i=1; i<FFT_SIZE/2; i++)
		{
			if(i!=FFT_SIZE-1)	// keeps only 24kHz frequencies (at least in theory)
			{
				freq_domain[i].r = 0;
				freq_domain[i].i = 0;
				freq_domain[FFT_SIZE-i].r = 0;
				freq_domain[FFT_SIZE-i].i = 0;
			}
		}


		SilentFft("Signal (complex)", freq_domain, time_domain, 1);	//calculate reverse FFT
		for (i = 0; i < FFT_SIZE; i++)
			IOWR_32DIRECT(file_address, (j*FFT_SIZE + i)*4, time_domain[i].r);
	}
}


void SilentFft(const char* title, const kiss_fft_cpx in[FFT_SIZE], kiss_fft_cpx out[FFT_SIZE], int inverse)
{
  static kiss_fft_cfg cfg = NULL;

  // if first time allocates memory
  if (cfg == NULL)
	  cfg = kiss_fft_alloc(FFT_SIZE, inverse, NULL, NULL);

  // checks if memory was successfully allocated
  if (cfg  != NULL)
  {
    size_t i;

    kiss_fft(cfg, in, out);
    //free(cfg);
  }
  else
  {
    printf("not enough memory?\n");
    exit(-1);
  }
}







