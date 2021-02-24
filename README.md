# A simple DSP (Digital Audio Processor) for audio treatment based on FPGA

In this project we used the Altera DE1-SoC board to implement a simple DSP to process an audio signal.
The signal is recorded by a microphone connected to the board and can be played back through the audio interface of the DE1-SoC.
Our code performs a DFT and some basic audio processing like:
* speed change
* filtering
* reverberation
* pitch change (not working)
