
module main_system (
	audio_0_external_interface_ADCDAT,
	audio_0_external_interface_ADCLRCK,
	audio_0_external_interface_BCLK,
	audio_0_external_interface_DACDAT,
	audio_0_external_interface_DACLRCK,
	audio_and_video_config_0_external_interface_SDAT,
	audio_and_video_config_0_external_interface_SCLK,
	clk_clk,
	clk_audio_codec_clk,
	clk_sdram_clk,
	pio_buttons_external_connection_export,
	pio_leds_external_connection_export,
	pio_switches_external_connection_export,
	reset_reset_n,
	sdram_controller_wire_addr,
	sdram_controller_wire_ba,
	sdram_controller_wire_cas_n,
	sdram_controller_wire_cke,
	sdram_controller_wire_cs_n,
	sdram_controller_wire_dq,
	sdram_controller_wire_dqm,
	sdram_controller_wire_ras_n,
	sdram_controller_wire_we_n,
	uart_0_external_connection_rxd,
	uart_0_external_connection_txd,
	pio_7_segments_external_connection_export);	

	input		audio_0_external_interface_ADCDAT;
	input		audio_0_external_interface_ADCLRCK;
	input		audio_0_external_interface_BCLK;
	output		audio_0_external_interface_DACDAT;
	input		audio_0_external_interface_DACLRCK;
	inout		audio_and_video_config_0_external_interface_SDAT;
	output		audio_and_video_config_0_external_interface_SCLK;
	input		clk_clk;
	output		clk_audio_codec_clk;
	output		clk_sdram_clk;
	input	[3:0]	pio_buttons_external_connection_export;
	output	[9:0]	pio_leds_external_connection_export;
	input	[9:0]	pio_switches_external_connection_export;
	input		reset_reset_n;
	output	[12:0]	sdram_controller_wire_addr;
	output	[1:0]	sdram_controller_wire_ba;
	output		sdram_controller_wire_cas_n;
	output		sdram_controller_wire_cke;
	output		sdram_controller_wire_cs_n;
	inout	[15:0]	sdram_controller_wire_dq;
	output	[1:0]	sdram_controller_wire_dqm;
	output		sdram_controller_wire_ras_n;
	output		sdram_controller_wire_we_n;
	input		uart_0_external_connection_rxd;
	output		uart_0_external_connection_txd;
	output	[27:0]	pio_7_segments_external_connection_export;
endmodule
