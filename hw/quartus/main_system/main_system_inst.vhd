	component main_system is
		port (
			audio_0_external_interface_ADCDAT                : in    std_logic                     := 'X';             -- ADCDAT
			audio_0_external_interface_ADCLRCK               : in    std_logic                     := 'X';             -- ADCLRCK
			audio_0_external_interface_BCLK                  : in    std_logic                     := 'X';             -- BCLK
			audio_0_external_interface_DACDAT                : out   std_logic;                                        -- DACDAT
			audio_0_external_interface_DACLRCK               : in    std_logic                     := 'X';             -- DACLRCK
			audio_and_video_config_0_external_interface_SDAT : inout std_logic                     := 'X';             -- SDAT
			audio_and_video_config_0_external_interface_SCLK : out   std_logic;                                        -- SCLK
			clk_clk                                          : in    std_logic                     := 'X';             -- clk
			clk_audio_codec_clk                              : out   std_logic;                                        -- clk
			clk_sdram_clk                                    : out   std_logic;                                        -- clk
			pio_buttons_external_connection_export           : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			pio_leds_external_connection_export              : out   std_logic_vector(9 downto 0);                     -- export
			pio_switches_external_connection_export          : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			reset_reset_n                                    : in    std_logic                     := 'X';             -- reset_n
			sdram_controller_wire_addr                       : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_controller_wire_ba                         : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_controller_wire_cas_n                      : out   std_logic;                                        -- cas_n
			sdram_controller_wire_cke                        : out   std_logic;                                        -- cke
			sdram_controller_wire_cs_n                       : out   std_logic;                                        -- cs_n
			sdram_controller_wire_dq                         : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_controller_wire_dqm                        : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_controller_wire_ras_n                      : out   std_logic;                                        -- ras_n
			sdram_controller_wire_we_n                       : out   std_logic;                                        -- we_n
			uart_0_external_connection_rxd                   : in    std_logic                     := 'X';             -- rxd
			uart_0_external_connection_txd                   : out   std_logic                                         -- txd
		);
	end component main_system;

	u0 : component main_system
		port map (
			audio_0_external_interface_ADCDAT                => CONNECTED_TO_audio_0_external_interface_ADCDAT,                --                  audio_0_external_interface.ADCDAT
			audio_0_external_interface_ADCLRCK               => CONNECTED_TO_audio_0_external_interface_ADCLRCK,               --                                            .ADCLRCK
			audio_0_external_interface_BCLK                  => CONNECTED_TO_audio_0_external_interface_BCLK,                  --                                            .BCLK
			audio_0_external_interface_DACDAT                => CONNECTED_TO_audio_0_external_interface_DACDAT,                --                                            .DACDAT
			audio_0_external_interface_DACLRCK               => CONNECTED_TO_audio_0_external_interface_DACLRCK,               --                                            .DACLRCK
			audio_and_video_config_0_external_interface_SDAT => CONNECTED_TO_audio_and_video_config_0_external_interface_SDAT, -- audio_and_video_config_0_external_interface.SDAT
			audio_and_video_config_0_external_interface_SCLK => CONNECTED_TO_audio_and_video_config_0_external_interface_SCLK, --                                            .SCLK
			clk_clk                                          => CONNECTED_TO_clk_clk,                                          --                                         clk.clk
			clk_audio_codec_clk                              => CONNECTED_TO_clk_audio_codec_clk,                              --                             clk_audio_codec.clk
			clk_sdram_clk                                    => CONNECTED_TO_clk_sdram_clk,                                    --                                   clk_sdram.clk
			pio_buttons_external_connection_export           => CONNECTED_TO_pio_buttons_external_connection_export,           --             pio_buttons_external_connection.export
			pio_leds_external_connection_export              => CONNECTED_TO_pio_leds_external_connection_export,              --                pio_leds_external_connection.export
			pio_switches_external_connection_export          => CONNECTED_TO_pio_switches_external_connection_export,          --            pio_switches_external_connection.export
			reset_reset_n                                    => CONNECTED_TO_reset_reset_n,                                    --                                       reset.reset_n
			sdram_controller_wire_addr                       => CONNECTED_TO_sdram_controller_wire_addr,                       --                       sdram_controller_wire.addr
			sdram_controller_wire_ba                         => CONNECTED_TO_sdram_controller_wire_ba,                         --                                            .ba
			sdram_controller_wire_cas_n                      => CONNECTED_TO_sdram_controller_wire_cas_n,                      --                                            .cas_n
			sdram_controller_wire_cke                        => CONNECTED_TO_sdram_controller_wire_cke,                        --                                            .cke
			sdram_controller_wire_cs_n                       => CONNECTED_TO_sdram_controller_wire_cs_n,                       --                                            .cs_n
			sdram_controller_wire_dq                         => CONNECTED_TO_sdram_controller_wire_dq,                         --                                            .dq
			sdram_controller_wire_dqm                        => CONNECTED_TO_sdram_controller_wire_dqm,                        --                                            .dqm
			sdram_controller_wire_ras_n                      => CONNECTED_TO_sdram_controller_wire_ras_n,                      --                                            .ras_n
			sdram_controller_wire_we_n                       => CONNECTED_TO_sdram_controller_wire_we_n,                       --                                            .we_n
			uart_0_external_connection_rxd                   => CONNECTED_TO_uart_0_external_connection_rxd,                   --                  uart_0_external_connection.rxd
			uart_0_external_connection_txd                   => CONNECTED_TO_uart_0_external_connection_txd                    --                                            .txd
		);

