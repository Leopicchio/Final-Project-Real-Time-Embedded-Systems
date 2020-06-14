

#include <stdio.h>
#include <stdbool.h>
#include <altera_up_avalon_audio.h>
#include <altera_up_avalon_audio_and_video_config.h>
#include <altera_avalon_mailbox_simple.h>
#include <altera_avalon_mailbox_simple_regs.h>
#include <altera_avalon_timer_regs.h>
#include <altera_avalon_pio_regs.h>
#include <system.h>
#include <math.h>
#include <limits.h>
#include <system.h>


#define SAMPLING_FREQ				48000							// sampling frequency used by the audio CODEC (may be changed to 8kHz to improve speed and reduce memory usage)

#define BEEP_FREQUENCY				500.
#define BEEP_AMPLITUDE				1.
#define BEEP_SECONDS				1								// accepts only integers!
#define BEEP_SAMPLES				BEEP_SECONDS * SAMPLING_FREQ	// number of samples in the "beep" file

#define RECORDING_MAX_NUMBER		3
#define RECORDING_TIME				4								// seconds
#define RECORDING_BUFFER_SIZE 		128								// 128 is the size of the FIFO that is used to send/receive data to/from the audio CODEC. A receiving buffer of 128 bytes does just fine
#define PIO_LEDS_NUMBER				10								// number of LEDs on the DE1-Soc board
#define LOUDNESS_THRESHOLD			0.5								// sound loudness threshold that starts the recording (in a range from 0 to 1)
#define BASE_ADDRESS 				SDRAM_CONTROLLER_BASE

typedef struct AudioFile
{
	unsigned int *start_address;
	unsigned int *file_pointer;
	unsigned int *end_address;
	bool processed;
}AudioFile;


void start_recording(AudioFile file, alt_up_audio_dev *audio_device);
AudioFile new_audio_file(unsigned int length);
void generate_sinusoid(int sampling_freq, float freq, char amplitude, AudioFile file);
void play_file(AudioFile file, alt_up_audio_dev *audio_device);
void show_amplitude_on_LEDS(float amplitude);
float average_amplitude(int *data, int length);
void sender_mailbox_callback(void* report, int status);
void receiver_mailbox_callback(void* message);
void show_state_on_7_segments();



AudioFile recording[RECORDING_MAX_NUMBER];
int recording_index = -1;

int main()
{
	alt_up_audio_dev *audio_device;
	alt_up_av_config_dev *audio_config;
	float sound_loudness = 0;
	unsigned int recording_buffer[RECORDING_BUFFER_SIZE], n_samples_in_FIFO, i;
	AudioFile beep_sound;
	char buttons_state;
	altera_avalon_mailbox_dev *sender_mailbox, *receiver_mailbox;	// handle to the Mailboxes
	alt_u32 message[2];



	printf("Hello from Sound Acquisition Processor!\n");

	// Open mailboxes
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

	// gets handles to audio devices
	audio_device = alt_up_audio_open_dev(AUDIO_0_NAME);	// handle to the circuit that interfaces with the audio CODEC
	audio_config = alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME); // handle to the circuit that initializes the audio CODEC

	// waits for the audio CODEC to be ready
	while (!alt_up_av_config_read_ready(audio_config)) {
		printf("Audio CODEC not ready\n");
	}

	// resets the audio CODEC just in case
	if (alt_up_av_config_reset(audio_config))
		printf("Error resetting the audio CODEC\n");

	// initialize parallel ports
	IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_LEDS_BASE, 0xFFFFFFFF);	// sets all pins as output
	IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_BUTTONS_BASE, 0x0);	// sets all pins as input
	IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_7_SEGMENTS_BASE, 0xFFFFFFFF);	// sets all pins as output

	beep_sound = new_audio_file(BEEP_SAMPLES);
	printf("\n\nGenerating sinusoid for beep sound. Please wait...\n");
	generate_sinusoid(SAMPLING_FREQ, BEEP_FREQUENCY, BEEP_AMPLITUDE, beep_sound);
	printf("Sinusoid created\n");

	printf("\n\nAllocating memory for recordings...  %d possible recordings of %d seconds each\n", RECORDING_MAX_NUMBER, RECORDING_TIME);
	for (i=0; i<RECORDING_MAX_NUMBER; i++)
		recording[i] = new_audio_file(RECORDING_TIME * SAMPLING_FREQ);
	printf("Memory allocated with success\n", RECORDING_MAX_NUMBER, RECORDING_TIME);



	printf("\n\nMake a loud noise to start recording. Try snapping your fingers.\n");
	while(1)
	{
		// reads the incoming FIFO from the audio CODEC (audio from the microphone)
		n_samples_in_FIFO = alt_up_audio_read_fifo_avail(audio_device, 0);				// checks how many elements are in the FIFO
		alt_up_audio_read_fifo(audio_device, recording_buffer, n_samples_in_FIFO, 0);	// reads the content of the FIFO

		sound_loudness = average_amplitude(recording_buffer, n_samples_in_FIFO);		// computes the sound loudness
		show_amplitude_on_LEDS(sound_loudness);											// shows the amplitude on the LEDs
		show_state_on_7_segments();

		if (sound_loudness > LOUDNESS_THRESHOLD) {
			if(recording_index < RECORDING_MAX_NUMBER-1)
				recording_index++;
			else
				recording_index = 0;
			recording[recording_index].processed = false;
			show_state_on_7_segments();

			printf("Loud noise detected! Recording starting in:\n3...\n");
			play_file(beep_sound, audio_device);
			usleep(250000);
			printf("2...\n");
			play_file(beep_sound, audio_device);
			usleep(250000);
			printf("1...\n");
			play_file(beep_sound, audio_device);
			usleep(250000);


			printf("Recording audio %d...\n", recording_index + 1);
			start_recording(recording[recording_index], audio_device);
			printf("Finished recording audio %d...\n", recording_index + 1);


			message[0] = (int)recording[recording_index].start_address;			// puts the address of the file containing the recording
			message[1] = (int)recording[recording_index].end_address;	// puts the length of the file
			printf("Sending file information:\tSTART ADDRESS: 0x%lx\tEND: 0x%lx \n", message[0], message[1]);
			altera_avalon_mailbox_send (sender_mailbox, message, 0, ISR);	// sends the info through the mailbox
		}

		buttons_state = IORD_ALTERA_AVALON_PIO_DATA(PIO_BUTTONS_BASE);
		switch (buttons_state)
		{
			case 0b0111:
				if (recording[0].processed == true)
				{
					printf("Playing recording 1...\n");
					play_file(recording[0], audio_device);
				}
				break;
			case 0b1011:
				if (recording[1].processed == true)
				{
					printf("Playing recording 2...\n");
					play_file(recording[1], audio_device);
				}
				break;
			case 0b1101:
				if (recording[2].processed == true)
				{
					printf("Playing recording 3...\n");
					play_file(recording[2], audio_device);
				}
				break;
			default:
				break;
		}
	}
	return 0;
}






// records a sound from the microphone and stores it at location pointed by *file (it should be in the SDRAM)
void start_recording(AudioFile file, alt_up_audio_dev *audio_device)
{
	unsigned int n_samples_in_FIFO = 0, buffer[RECORDING_BUFFER_SIZE], i;
	float percentage;

	file.processed = false;
	file.file_pointer = file.start_address;

	while (file.file_pointer < file.end_address) {
		n_samples_in_FIFO = alt_up_audio_read_fifo_avail(audio_device, 0);
		alt_up_audio_read_fifo(audio_device, buffer, n_samples_in_FIFO, 0);		// reads data from the incoming FIFO (sound from the microphone)

		percentage = (float)(file.file_pointer - file.start_address)/(file.end_address - file.start_address);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_LEDS_BASE, (0x01 << (int)(10 * percentage))-1);
		for(i=0; i<n_samples_in_FIFO; i++)
		{
			IOWR_32DIRECT(file.file_pointer, i*4, buffer[i]);
		}
		file.file_pointer += n_samples_in_FIFO;
	}
}






AudioFile new_audio_file(unsigned int length)
{
	static unsigned int *first_available_address = BASE_ADDRESS;
	AudioFile file;

	file.start_address = first_available_address;
	file.file_pointer = first_available_address;
	file.end_address = first_available_address + length;
	file.processed = false;

	if ((int)file.end_address  < SDRAM_CONTROLLER_BASE + SDRAM_CONTROLLER_SPAN)
	{
		first_available_address = file.end_address + 1;
	}
	else
	{
		printf("ERROR: not enough memory to allocate audio file!\n");
		file.start_address = NULL;
		file.file_pointer = NULL;
		file.end_address = NULL;
	}
	return file;
}






// creates a sinusoidal waveform in the memory (at the location pointed by "unsigned int *file"), n_samples specifies the length
// of the file to create.
void generate_sinusoid(int sampling_freq, float freq, char amplitude, AudioFile file)
{
	int i, amplitude_factor, samples_in_period, n_samples;
	float phase_factor;
	unsigned int sample;

	n_samples = file.end_address - file.start_address;

	phase_factor = 2 * M_PI * freq / sampling_freq;	// I break down the computation because it is too slow
	amplitude_factor = UINT_MAX*(float)amplitude/100.0;

	// generates first period of the wave and then copies it (to improve speed since sin() is very slow)
	samples_in_period = sampling_freq/freq;
	for (i=0; i<samples_in_period; i++)
	{
		file.start_address[i] = amplitude_factor *  sin( phase_factor * i );
	}

	for(i=samples_in_period; i<n_samples; i++)	// copies it all over
	{
		sample = IORD_32DIRECT(file.start_address, (i - samples_in_period)*4);
		IOWR_32DIRECT(file.start_address, i*4, sample);
	}
}




// this function plays an audio file trough the output jack (green). To do so it needs to send bursts of data to the outgoing FIFO.
void play_file(AudioFile file, alt_up_audio_dev *audio_device)
{
	unsigned int *file_pointer, i, file_size;
	const unsigned int BURST_LENGTH = 100;	// the outgoing FIFO is of 128 elements, if we write burst of 100 at the time there is no distortion (this is an empirical value)

	file_size = file.end_address - file.start_address;
	// sends the data, one burst at a time until the end of the file is reached
	for (i=0; i<file_size; i+=BURST_LENGTH)
	{
		file_pointer = file.start_address + i;
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



void sender_mailbox_callback(void* report, int status) {
	if (status) {
		printf("Transfer done\n");
	} else {
		printf("error in transfer\n");
	}
}



void receiver_mailbox_callback(void* message){
	int i;
	alt_u32* data = (alt_u32*)message;	// cast from void* to alt_u32*

	//IORD_ALTERA_AVALON_MAILBOX_CMD(MAILBOX_TO_SOUND_ACQUISITION_BASE);
	if (message != NULL) {
		for (i=0; i<RECORDING_MAX_NUMBER; i++)
			if (recording[i].start_address ==  (unsigned int*)data[0])
				break;
		if (i >= RECORDING_MAX_NUMBER)
			printf("Error receiving file information from other processor\n");
		else
			recording[i].processed = true;

		printf("Received processed file information: START ADDRESS 0x%lx \t END: 0x%lx \n\n", data[0], data[1]);
	} else {
		printf("Incomplete receive\n");
	}
}


void show_state_on_7_segments()
{
	unsigned int output = 0xFFFFFFFF;

	if (recording_index == 0)
		output = output & 0b1111111111111111111111111001;
	else if (recording_index == 1)
		output = output & 0b1111111111111111111110100100;
	else if (recording_index == 2)
		output = output & 0b1111111111111111111110110000;

	if (recording[0].processed)
		output = output & 0b1000000111111111111111111111;
	if (recording[1].processed)
		output = output & 0b1111111100000011111111111111;
	if (recording[2].processed)
		output = output & 0b1111111111111110000001111111;

	IOWR_ALTERA_AVALON_PIO_DATA(PIO_7_SEGMENTS_BASE, output);
}


