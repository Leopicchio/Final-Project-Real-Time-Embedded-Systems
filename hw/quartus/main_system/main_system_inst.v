	main_system u0 (
		.audio_0_external_interface_ADCDAT                (<connected-to-audio_0_external_interface_ADCDAT>),                //                  audio_0_external_interface.ADCDAT
		.audio_0_external_interface_ADCLRCK               (<connected-to-audio_0_external_interface_ADCLRCK>),               //                                            .ADCLRCK
		.audio_0_external_interface_BCLK                  (<connected-to-audio_0_external_interface_BCLK>),                  //                                            .BCLK
		.audio_0_external_interface_DACDAT                (<connected-to-audio_0_external_interface_DACDAT>),                //                                            .DACDAT
		.audio_0_external_interface_DACLRCK               (<connected-to-audio_0_external_interface_DACLRCK>),               //                                            .DACLRCK
		.audio_and_video_config_0_external_interface_SDAT (<connected-to-audio_and_video_config_0_external_interface_SDAT>), // audio_and_video_config_0_external_interface.SDAT
		.audio_and_video_config_0_external_interface_SCLK (<connected-to-audio_and_video_config_0_external_interface_SCLK>), //                                            .SCLK
		.clk_clk                                          (<connected-to-clk_clk>),                                          //                                         clk.clk
		.clk_audio_codec_clk                              (<connected-to-clk_audio_codec_clk>),                              //                             clk_audio_codec.clk
		.clk_sdram_clk                                    (<connected-to-clk_sdram_clk>),                                    //                                   clk_sdram.clk
		.pio_buttons_external_connection_export           (<connected-to-pio_buttons_external_connection_export>),           //             pio_buttons_external_connection.export
		.pio_leds_external_connection_export              (<connected-to-pio_leds_external_connection_export>),              //                pio_leds_external_connection.export
		.pio_switches_external_connection_export          (<connected-to-pio_switches_external_connection_export>),          //            pio_switches_external_connection.export
		.reset_reset_n                                    (<connected-to-reset_reset_n>),                                    //                                       reset.reset_n
		.sdram_controller_wire_addr                       (<connected-to-sdram_controller_wire_addr>),                       //                       sdram_controller_wire.addr
		.sdram_controller_wire_ba                         (<connected-to-sdram_controller_wire_ba>),                         //                                            .ba
		.sdram_controller_wire_cas_n                      (<connected-to-sdram_controller_wire_cas_n>),                      //                                            .cas_n
		.sdram_controller_wire_cke                        (<connected-to-sdram_controller_wire_cke>),                        //                                            .cke
		.sdram_controller_wire_cs_n                       (<connected-to-sdram_controller_wire_cs_n>),                       //                                            .cs_n
		.sdram_controller_wire_dq                         (<connected-to-sdram_controller_wire_dq>),                         //                                            .dq
		.sdram_controller_wire_dqm                        (<connected-to-sdram_controller_wire_dqm>),                        //                                            .dqm
		.sdram_controller_wire_ras_n                      (<connected-to-sdram_controller_wire_ras_n>),                      //                                            .ras_n
		.sdram_controller_wire_we_n                       (<connected-to-sdram_controller_wire_we_n>),                       //                                            .we_n
		.uart_0_external_connection_rxd                   (<connected-to-uart_0_external_connection_rxd>),                   //                  uart_0_external_connection.rxd
		.uart_0_external_connection_txd                   (<connected-to-uart_0_external_connection_txd>),                   //                                            .txd
		.pio_7_segments_external_connection_export        (<connected-to-pio_7_segments_external_connection_export>)         //          pio_7_segments_external_connection.export
	);

