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
#include "system.h"

void receiver_callback(void* message);


int main()
{
	alt_u32 message[2];
	altera_avalon_mailbox_dev* mailbox;	// handle to the Mailbox

	// Open mailbox
	mailbox = altera_avalon_mailbox_open("/dev/mailbox_simple_0", NULL, receiver_callback);
	if (!mailbox) {
		printf("FAIL: Unable to open mailbox\n");
		return 1;
	}

	while(1)
	{
		// do your stuff
	}

	altera_avalon_mailbox_close(mailbox);
	return 0;
}

/* Get message read from mailbox */
void receiver_callback(void* message) {

	alt_u32* data = (alt_u32*)message;	// cast from void* to alt_u32*

	if (message != NULL) {
		printf("Message received with Command %lx and Message %lx\n", data[0], data[1]);
	} else {
		printf("Incomplete receive");
	}
}





