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
#include "altera_avalon_mailbox_simple.h"
#include "altera_avalon_mailbox_simple_regs.h"
#include <altera_avalon_pio_regs.h>
#include "system.h"
#include "sys/alt_irq.h"

void receiver_mailbox_callback(void* message);
void sender_mailbox_callback(void* report, int status);
void add_echo(unsigned int *file_address, unsigned int file_length, unsigned int delay_ms, float amplitude);

// parameters of the file to be processed (which are received through the mailbox)
unsigned int *file_address, file_length;
char flag_data_ready = 0;

// the masks that are used to know which effect is activated
#define EFFECT_REVERSE  0b0000000001
#define EFFECT_ECHO		0b0000000010

#define SAMPLING_FREQ_Hz	48000	// sampling frequency used by the audio CODEC (Hertz)
#define SAMPLING_FREQ_kHz	48		// sampling frequency used by the audio CODEC (kHz)
#define ECHO_DELAY_MS		1000	// milliseconds
#define ECHO_AMPLITUDE		0.5		// in range from 0 to 1



int main()
{
	alt_u32 message[2];
	altera_avalon_mailbox_dev *sender_mailbox, *receiver_mailbox;	// handle to the Mailbox
	int activated_audio_effects = 0, i=0;	// each bit of this variable represents an effect which can be activated by toggling the switches

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
		if (flag_data_ready)
		{
			alt_irq_context context;
			context = alt_irq_disable_all();

			printf("\n\nProcessing file at address: 0x%x with length: %u \n\n", file_address, file_length);
			// read the switches to see which effects are active
			activated_audio_effects = IORD_ALTERA_AVALON_PIO_DATA(PIO_SWITCHES_BASE);

			// TO DO:    FFT

			// check which effects are active and apply them
			printf("LIST OF AUDIO EFFECTS APPLIED:\n");
			if (activated_audio_effects & EFFECT_REVERSE)	// if this effect is active
			{
				printf("\tReversing audio...\n");
			}
			if (activated_audio_effects & EFFECT_ECHO)
			{
				printf("\tApplying echo...\n");
				add_echo(file_address, file_length, ECHO_DELAY_MS, ECHO_AMPLITUDE);
			}
			printf("----------------------------------\n");
			// sends to the other processor the address and the length of the processed file
			printf("Sending processed file info to the other processor...\n");
			message[0] = file_address;
			message[1] = file_length;
			altera_avalon_mailbox_send (sender_mailbox, message, 0, ISR);	// sends the info through the mailbox

			flag_data_ready = 0;
			alt_irq_enable_all(context);
		}


	}

	altera_avalon_mailbox_close(receiver_mailbox);
	altera_avalon_mailbox_close(sender_mailbox);
	return 0;
}





/* Get message read from mailbox */
void receiver_mailbox_callback(void* message) {

	alt_u32* data = (alt_u32*)message;	// cast from void* to alt_u32*

	if (message != NULL) {
		printf("Message received from sound acquisition CPU: msg[0]: %lx   msg[1]: %lx\n", data[0], data[1]);
		file_address = (unsigned int*)data[0];
		file_length = (unsigned int) data[1];
		flag_data_ready = 1;	// raises a flag to start processing the data
	} else {
		printf("[FFT]Incomplete receive\n");
	}
}





void sender_mailbox_callback(void* report, int status) {
	if (status) {
		printf("Processed file info sent with success.\n");
	} else {
		printf("Error in transfer\n");
	}
}





void add_echo(unsigned int *file_address, unsigned int file_length, unsigned int delay_ms, float amplitude)
{
	unsigned int i, samples_delay;
	float sample_amplitude;

	samples_delay = delay_ms*SAMPLING_FREQ_kHz;
	for (i=samples_delay; i<file_length; i++)
	{
		sample_amplitude = IORD_32DIRECT(file_address, (i-samples_delay)*4) * amplitude;
		sample_amplitude += IORD_32DIRECT(file_address, i*4);
		IOWR_32DIRECT(file_address, i*4, sample_amplitude);
	}
	printf("Echo applied\n");
}








/* CODE FOR INTERRUPTS ON THE SWITCHES, IN CASE IT MAY BE USEFUL

IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SWITCHES_BASE, 0xFFFFFFFF);		// enables interrupts on all switches
alt_ic_isr_register(PIO_SWITCHES_IRQ_INTERRUPT_CONTROLLER_ID, PIO_SWITCHES_IRQ, switches_toggled,NULL, NULL);	// registers interrupt

void switches_toggled()
{
	int edge_capture_register;
	edge_capture_register = IORD_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SWITCHES_BASE);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SWITCHES_BASE, 0x00000000);	// dummy write operation to the register to clear interrupt


}
*/




