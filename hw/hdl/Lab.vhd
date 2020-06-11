-- #############################################################################
-- DE1_SoC_top_level.vhd
--
-- BOARD         : DE1-SoC from Terasic
-- Author        : Sahand Kashani-Akhavan from Terasic documentation
-- Revision      : 1.2
-- Creation date : 04/02/2015
-- Modifications : 06/04/2017 RB to be coorectly interpreted for bit index
--                            GPIO_0 --> GPIO_0_D
--                            GPIO_1 --> GPIO_1_D

-- Syntax Rule : GROUP_NAME_N[bit]
--
-- GROUP  : specify a particular interface (ex: SDR_)
-- NAME   : signal name (ex: CONFIG, D, ...)
-- bit    : signal index
-- _N     : to specify an active-low signal
-- #############################################################################

library ieee;
use ieee.std_logic_1164.all;

entity Lab is
    port(
        -- ADC
        --ADC_CS_n         : out   std_logic;
        --ADC_DIN          : out   std_logic;
        --ADC_DOUT         : in    std_logic;
        --ADC_SCLK         : out   std_logic;

        -- Audio
			AUD_ADCDAT : in std_logic;
			AUD_ADCLRCK : inout std_logic;
			AUD_BCLK : inout std_logic;
			AUD_DACDAT : out std_logic;
			AUD_DACLRCK : inout std_logic;
			AUD_XCK : out std_logic;


        -- CLOCK
        CLOCK_50         : in    std_logic;
        --CLOCK2_50        : in    std_logic;
        --CLOCK3_50        : in    std_logic;
        --CLOCK4_50        : in    std_logic;

        -- SDRAM
        DRAM_ADDR        : out   std_logic_vector(12 downto 0);
        DRAM_BA          : out   std_logic_vector(1 downto 0);
        DRAM_CAS_N       : out   std_logic;
        DRAM_CKE         : out   std_logic;
        DRAM_CLK         : out   std_logic;
        DRAM_CS_N        : out   std_logic;
        DRAM_DQ          : inout std_logic_vector(15 downto 0);
        DRAM_LDQM        : out   std_logic;
        DRAM_RAS_N       : out   std_logic;
        DRAM_UDQM        : out   std_logic;
        DRAM_WE_N        : out   std_logic;

        -- I2C for Audio and Video-In
        FPGA_I2C_SCLK    : out   std_logic;
        FPGA_I2C_SDAT    : inout std_logic;

        -- SEG7
        --HEX0_N           : out   std_logic_vector(6 downto 0);
        --HEX1_N           : out   std_logic_vector(6 downto 0);
        --HEX2_N           : out   std_logic_vector(6 downto 0);
        --HEX3_N           : out   std_logic_vector(6 downto 0);
        --HEX4_N           : out   std_logic_vector(6 downto 0);
        --HEX5_N           : out   std_logic_vector(6 downto 0);

        -- IR
        --IRDA_RXD         : in    std_logic;
        --IRDA_TXD         : out   std_logic;

        -- KEY_n
        KEY_N            : in    std_logic_vector(3 downto 0);

        -- LED
        LEDR             : out   std_logic_vector(9 downto 0);

        -- PS2
        --PS2_CLK          : inout std_logic;
        --PS2_CLK2         : inout std_logic;
        --PS2_DAT          : inout std_logic;
        --PS2_DAT2         : inout std_logic;

        -- SW
        SW               : in    std_logic_vector(9 downto 0);

        -- Video-In
        --TD_CLK27         : inout std_logic;
        --TD_DATA          : out   std_logic_vector(7 downto 0);
        --TD_HS            : out   std_logic;
        --TD_RESET_N       : out   std_logic;
        --TD_VS            : out   std_logic;

        -- VGA
        --VGA_B            : out   std_logic_vector(7 downto 0);
        --VGA_BLANK_N      : out   std_logic;
        --VGA_CLK          : out   std_logic;
        --VGA_G            : out   std_logic_vector(7 downto 0);
        --VGA_HS           : out   std_logic;
        --VGA_R            : out   std_logic_vector(7 downto 0);
        --VGA_SYNC_N       : out   std_logic;
        --VGA_VS           : out   std_logic;

        -- GPIO_0
        GPIO_0_D           : inout std_logic_vector(35 downto 0)

        -- GPIO_1
        --GPIO_1_D           : inout std_logic_vector(35 downto 0);

        -- HPS
        --HPS_CONV_USB_N   : inout std_logic;
        --HPS_DDR3_ADDR    : out   std_logic_vector(14 downto 0);
        --HPS_DDR3_BA      : out   std_logic_vector(2 downto 0);
        --HPS_DDR3_CAS_N   : out   std_logic;
        --HPS_DDR3_CK_N    : out   std_logic;
        --HPS_DDR3_CK_P    : out   std_logic;
        --HPS_DDR3_CKE     : out   std_logic;
        --HPS_DDR3_CS_N    : out   std_logic;
        --HPS_DDR3_DM      : out   std_logic_vector(3 downto 0);
        --HPS_DDR3_DQ      : inout std_logic_vector(31 downto 0);
        --HPS_DDR3_DQS_N   : inout std_logic_vector(3 downto 0);
        --HPS_DDR3_DQS_P   : inout std_logic_vector(3 downto 0);
        --HPS_DDR3_ODT     : out   std_logic;
        --HPS_DDR3_RAS_N   : out   std_logic;
        --HPS_DDR3_RESET_N : out   std_logic;
        --HPS_DDR3_RZQ     : in    std_logic;
        --HPS_DDR3_WE_N    : out   std_logic;
        --HPS_ENET_GTX_CLK : out   std_logic;
        --HPS_ENET_INT_N   : inout std_logic;
        --HPS_ENET_MDC     : out   std_logic;
        --HPS_ENET_MDIO    : inout std_logic;
        --HPS_ENET_RX_CLK  : in    std_logic;
        --HPS_ENET_RX_DATA : in    std_logic_vector(3 downto 0);
        --HPS_ENET_RX_DV   : in    std_logic;
        --HPS_ENET_TX_DATA : out   std_logic_vector(3 downto 0);
        --HPS_ENET_TX_EN   : out   std_logic;
        --HPS_FLASH_DATA   : inout std_logic_vector(3 downto 0);
       -- HPS_FLASH_DCLK   : out   std_logic;
        ----HPS_FLASH_NCSO   : out   std_logic;
        --HPS_GPIO         : inout std_logic_vector(1 downto 0);
        --HPS_GSENSOR_INT  : inout std_logic;
        --HPS_I2C_CONTROL  : inout std_logic;
        --HPS_I2C1_SCLK    : inout std_logic;
        --HPS_I2C1_SDAT    : inout std_logic;
        --HPS_I2C2_SCLK    : inout std_logic;
        --HPS_I2C2_SDAT    : inout std_logic;
        --HPS_KEY          : inout std_logic;
        --HPS_LED          : inout std_logic;
        --HPS_SD_CLK       : out   std_logic;
        --HPS_SD_CMD       : inout std_logic;
        --HPS_SD_DATA      : inout std_logic_vector(3 downto 0);
        --HPS_SPIM_CLK     : out   std_logic;
        --HPS_SPIM_MISO    : in    std_logic;
        --HPS_SPIM_MOSI    : out   std_logic;
        --HPS_SPIM_SS      : inout std_logic;
        --HPS_UART_RX      : in    std_logic;
        --HPS_UART_TX      : out   std_logic;
        --HPS_USB_CLKOUT   : in    std_logic;
        --HPS_USB_DATA     : inout std_logic_vector(7 downto 0);
        --HPS_USB_DIR      : in    std_logic;
        --HPS_USB_NXT      : in    std_logic;
        --HPS_USB_STP      : out   std_logic
    );
end entity Lab;




architecture rtl of Lab is

component main_system is
        port (
            clk_clk                     : in    std_logic                     := 'X';             -- clk
            reset_reset_n               : in    std_logic                     := 'X';             -- reset_n
            clk_sdram_clk               : out   std_logic;                                        -- clk
            sdram_controller_wire_addr  : out   std_logic_vector(12 downto 0);                    -- addr
            sdram_controller_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
            sdram_controller_wire_cas_n : out   std_logic;                                        -- cas_n
            sdram_controller_wire_cke   : out   std_logic;                                        -- cke
            sdram_controller_wire_cs_n  : out   std_logic;                                        -- cs_n
            sdram_controller_wire_dq    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
            sdram_controller_wire_dqm   : out   std_logic_vector(1 downto 0);                     -- dqm
            sdram_controller_wire_ras_n : out   std_logic;                                        -- ras_n
            sdram_controller_wire_we_n  : out   std_logic;                                         -- we_n
				audio_0_external_interface_ADCDAT                : in    std_logic                     := 'X';             -- ADCDAT
            audio_0_external_interface_ADCLRCK               : in    std_logic                     := 'X';             -- ADCLRCK
            audio_0_external_interface_BCLK                  : in    std_logic                     := 'X';             -- BCLK
            audio_0_external_interface_DACDAT                : out   std_logic;                                        -- DACDAT
            audio_0_external_interface_DACLRCK               : in    std_logic                     := 'X';             -- DACLRCK
            audio_and_video_config_0_external_interface_SDAT : inout std_logic                     := 'X';             -- SDAT
            audio_and_video_config_0_external_interface_SCLK : out   std_logic;                                         -- SCLK
				clk_audio_codec_clk                              : out   std_logic;                                         -- clk
				pio_buttons_external_connection_export           : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
            pio_leds_external_connection_export              : out   std_logic_vector(9 downto 0);                      -- export
				pio_switches_external_connection_export          : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
            uart_0_external_connection_rxd                   : in    std_logic                     := 'X';             -- rxd
            uart_0_external_connection_txd                   : out   std_logic        
		  );
    end component main_system;
	 
	 

	 
	 
	 
begin



	
	u0 : component main_system
        port map (
            clk_clk                     	=> CLOCK_50,                     --                   clk.clk
            clk_sdram_clk            		=> DRAM_CLK,           --         pll_clk_sdram.clk
            reset_reset_n               	=> KEY_N(0),               --                 reset.reset_n
            sdram_controller_wire_addr  	=> DRAM_ADDR,  -- sdram_controller_wire.addr
            sdram_controller_wire_ba    	=> DRAM_BA,    --                      .ba
            sdram_controller_wire_cas_n 	=> DRAM_CAS_N, --                      .cas_n
            sdram_controller_wire_cke   	=> DRAM_CKE,   --                      .cke
            sdram_controller_wire_cs_n  	=> DRAM_CS_N,  --                      .cs_n
            sdram_controller_wire_dq    	=> DRAM_DQ,    --                      .dq
            sdram_controller_wire_dqm(1)  => DRAM_UDQM,   --                      .dqm
            sdram_controller_wire_dqm(0)  => DRAM_LDQM,
				sdram_controller_wire_ras_n 	=> DRAM_RAS_N, --                      .ras_n
            sdram_controller_wire_we_n  	=> DRAM_WE_N,   --                      .we_n
				audio_0_external_interface_ADCDAT                => AUD_ADCDAT,                --                  audio_0_external_interface.ADCDAT
            audio_0_external_interface_ADCLRCK               => AUD_ADCLRCK,               --                                            .ADCLRCK
            audio_0_external_interface_BCLK                  => AUD_BCLK,                  --                                            .BCLK
            audio_0_external_interface_DACDAT                => AUD_DACDAT,                --                                            .DACDAT
            audio_0_external_interface_DACLRCK               => AUD_DACLRCK,               --                                            .DACLRCK
            audio_and_video_config_0_external_interface_SDAT => FPGA_I2C_SDAT, -- audio_and_video_config_0_external_interface.SDAT
            audio_and_video_config_0_external_interface_SCLK => FPGA_I2C_SCLK,  --                                            .SCLK
				clk_audio_codec_clk                              => AUD_XCK,
				pio_buttons_external_connection_export           => KEY_N,
            pio_leds_external_connection_export              => LEDR,
				pio_switches_external_connection_export          => SW,          --            pio_switches_external_connection.export
            uart_0_external_connection_rxd                   => GPIO_0_D(0),                   --                  uart_0_external_connection.rxd
            uart_0_external_connection_txd                   => GPIO_0_D(1)                    --                                            .txd				
		  );

end;








