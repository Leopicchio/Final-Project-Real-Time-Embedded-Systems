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
//https://stackoverflow.com/questions/14536950/applying-kiss-fft-on-audio-samples-and-getting-nan-output
//https://github.com/mborgerding/kissfft
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "kiss_fft.h"
//#include "wawfile.h"

#ifndef M_PI
#define M_PI 3.14159265358979324
#endif

#define N 16

void TestFft(const char* title, const kiss_fft_cpx in[N], kiss_fft_cpx out[N], int inverse)
{
  kiss_fft_cfg cfg;

  printf("%s\n", title);

  if ((cfg = kiss_fft_alloc(N, inverse/*is_inverse_fft*/, NULL, NULL)) != NULL)
  {
    size_t i;

    kiss_fft(cfg, in, out);
    free(cfg);

    for (i = 0; i < N; i++)
      printf(" in[%d] = %+f , %+f    "
             "out[%d] = %+f , %+f\n",
             i, in[i].r, in[i].i,
             i, out[i].r, out[i].i);
  }
  else
  {
    printf("not enough memory?\n");
    exit(-1);
  }
}

void SilentFft(const char* title, const kiss_fft_cpx in[N], kiss_fft_cpx out[N], int inverse)
{
  kiss_fft_cfg cfg;

  //printf("%s\n", title);

  if ((cfg = kiss_fft_alloc(N, inverse/*is_inverse_fft*/, NULL, NULL)) != NULL)
  {
    size_t i;

    kiss_fft(cfg, in, out);
    free(cfg);

//    for (i = 0; i < N; i++)
      //printf(" in[%d] = %+f , %+f    "
      //       "out[%d] = %+f , %+f\n",
      //       i, in[i].r, in[i].i,
      //      i, out[i].r, out[i].i);
  }
  else
  {
    printf("not enough memory?\n");
    exit(-1);
  }
}



int main(void)
{
  printf("Sound treatment processor online !\n");
  kiss_fft_cpx in[N], out[N];
  size_t i;

  for (i = 0; i < N; i++)
    in[i].r = in[i].i = 0;
  TestFft("Zeroes (complex)", in, out, 0);

  for (i = 0; i < N; i++)
    in[i].r = 1, in[i].i = 0;
  TestFft("Ones (complex)", in, out, 0);

  for (i = 0; i < N; i++)
    in[i].r = sin(2 * M_PI * 4 * i / N), in[i].i = 0;
  TestFft("SineWave (complex)", in, out, 0);

//  for (i = 0; i < N; i++)
//      in[i].r = wawsound[i], in[i].i = 0;
//  TestFft("Wawfile (complex)", in, out, 0);
//
//  for (i = 0; i < N; i++)
//        in[i].r = out[i].r, in[i].i = out[i].i;
//    TestFft("Wawfile reverse (complex)", in, out, 1);

    printf("starting UART stream\n");
 	char* msg = "Detected the character 't'.\n";
    FILE* fp;
    char prompt = 0;
    fp = fopen ("/dev/uart_0", "r+"); //Open file for reading and writing
    int datatofft[16];
    if (fp)
    {
		while (prompt != 'v')
		{ // Loop until we receive a 'v'.
			char* msg2 = "n\n";
			fwrite (msg2, strlen (msg2), 1, fp);//ready for next chunk of data

			char messageReceived[10] = "";
			prompt = getc(fp); // Get a character from the UART.
			int j = 0;
			while(prompt != 'x')
			{
				//printf("%c", prompt);
				//strcat(messageReceived, prompt);
				int i = 0;
				while(prompt != 'a')
				{
					messageReceived[i] = prompt;
					i += 1;
					prompt = getc(fp); // Get a character from the UART.
				}
				prompt = getc(fp);
				messageReceived[i] = '\0';
				int value = atoi(messageReceived);
				//printf("%d\n", value);
				datatofft[j] = value-100;
				j+=1;
			}
			//printf("Transmission ends --M.H.\n");
//			for (i = 0; i < N; i++)
//			    in[i].r = datatofft[i], in[i].i = 0;
//			SilentFft("Signal (complex)", in, out, 0);
//			for (i = 0; i < N; i++)
//				printf("%f", out[i].r);
//			printf("\n");
			for (i = 0; i < N; i++)
				printf("%d, ", datatofft[i]);
			printf("\n");

		}
		fprintf(fp, "Closing the UART file.\n");
		fclose (fp);
	}


//     char* msg = "Detected the character 't'.\n";
//     FILE* fp;
//     char prompt = 0;
//     fp = fopen ("/dev/uart_0", "r+"); //Open file for reading and writing
//     if (fp)
//     {
//		 while (prompt != 'v')
//		 { // Loop until we receive a 'v'.
//			 prompt = getc(fp); // Get a character from the UART.
//			 if (prompt == 't')
//			 { // Print a message if character is 't'.
//				 fwrite (msg, strlen (msg), 1, fp);
//			 }
//		 }
//		 fprintf(fp, "Closing the UART file.\n");
//		 fclose (fp);
//     }













  return 0;
}

//
//
//
//
//
//#include <stdio.h>
//#include "wawfile.h"
////#include "kiss_fft.h"
//#include "data_processor.h"
//
//
//static int read_data(int * source, kiss_fft_cpx *cin, int n) {
//	int i = 0;
//	while (i < n) {
//		cin[i].i = 0;
//		cin[i++].r = source[i];
//	}
//	return 0;
//}
//
//
//
//static int write_data(int * save, const kiss_fft_cpx *cout, int n) {
//	while (n-- > 0) {
//		printf("(%12f, %12f)\n", cout->r, cout->i);
//		save[n] = cout->r;
//		cout++;
//	}
//	return 0;
//}
//
//
//
//
//int main()
//{
//
//
//  printf("Hello from Nios II!\n");
//  //int testInteger;
//  //printf("Enter an integer: ");
//  //scanf("%d", &testInteger);
//  //printf("Number = %d",testInteger);
//
////
////  kiss_fft_cfg cfg = kiss_fft_alloc( 1024, 0,0,0 );
////  int cx_out[1024];
////  int cx_in[1024];
//
//  for (int i = 0; i < 2048; i++)
//  {
//	  printf("%d\n",wawsound[i]);
//
//  }
//
////  kiss_fft( cfg , wawsound , cx_out );
////  kiss_fft_free(cfg);
//  float hz = 0;
//  data_processor_t dfft = NULL;
//  dfft = data_processor_init(2048, 214000);
//  if (!dfft) {
//  	printf("data_processor_init error\n");
//  	return -1;
//  }
//  read_data(wawsound, dfft->cin, dfft->nfft);
//
//  int i = 100;
//  while (i-- > 0) {
//  	hz = data_processor_run(dfft);
//  	printf("hz = %12f\n", hz);
//  }
//  int save[2018];
//  write_data(save, dfft->cout, dfft->nfft);
//
//  //data_processor_close(dfft);
//  //dfft = NULL;
//
//
//
////  	//float hz = 0;
////    //data_processor_t dfft = NULL;
////    //dfft = data_processor_init(2048, 214000);
////    if (!dfft) {
////    	printf("data_processor_init error\n");
////    	return -1;
////    }
////    read_data(save, dfft->cin, dfft->nfft);
////
////    hz = data_processor_run(dfft);
////    printf("hz = %12f\n", hz);
////
////    //int save2[2048];
////    write_data(save, dfft->cout, dfft->nfft);
////
////    data_processor_close(dfft);
////    dfft = NULL;
//
//
//
//
//
//
//
//
//
//
//
//
//
//  return 0;
//}
