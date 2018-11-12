----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/16/2017 05:16:59 PM
-- Design Name: 
-- Module Name: imageFilter - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;
use IEEE.math_real.all;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity imageFilter is
  Generic (
      ADDR_WIDTH : integer := 14;
      DATA_WIDTH : integer := 8;
      
      N_COLS_INT : integer := 10;
      N_ROWS_INT : integer := 5
  );
  Port ( 
      clk : in std_logic;
        
      start : in    std_logic;
      idle  : out   std_logic;
      done  : out   std_logic;
    
      bram_frame_in_waddr         : in   std_logic_vector(ADDR_WIDTH-1 downto 0);
      bram_frame_in_din           : in   std_logic_vector(DATA_WIDTH-1 downto 0);
      bram_frame_in_we            : in   std_logic;
      
      bram_frame_out_raddr        : in    std_logic_vector(ADDR_WIDTH-1 downto 0);
      bram_frame_out_dout         : out   std_logic_vector(DATA_WIDTH-1 downto 0)
  );
end imageFilter;

architecture Behavioral of imageFilter is

  -- Constants
  constant N_COLS : unsigned(31 downto 0) := to_unsigned(N_COLS_INT,32);
  constant N_COLS_2 : unsigned(31 downto 0) := to_unsigned(2*N_COLS_INT,32);
  constant N_ROWS : unsigned(31 downto 0) := to_unsigned(N_ROWS_INT,32);
  constant NBR_PIXELS : unsigned(31 downto 0) := to_unsigned( N_ROWS_INT * N_COLS_INT, 32);
  
  -- BRAM for image
  -- For simulation
  -- Uncomment the following line if you are simulating the IP
  type ram_t is array ((2**ADDR_WIDTH)-1 downto 0) of std_logic_vector (DATA_WIDTH-1 downto 0);
  -- For synthesis
  -- Uncomment the following line if you are synthesizing the IP
  --type ram_t is array (to_integer(NBR_PIXELS)-1 downto 0) of std_logic_vector (DATA_WIDTH-1 downto 0);
  signal bram_frame_in : ram_t := ((others=> (others=>'1')));
  signal bram_frame_out : ram_t := ((others=> (others=>'0')));

  -- Signal declaration
  type state_type is (
            STATE_INIT, STATE_PROCESSING 
            );
            
  signal state        : state_type := STATE_INIT;
  
  -- count number of times a pixel was processed
    signal n_pixel: integer range 0 to 10799 := 0;  -- 120*90
    signal threshold: unsigned(7 downto 0) := (others=>'0');
    signal pixel: unsigned(7 downto 0) := (others=>'0');
  
  -- Read port 0 of BRAM frame_in
  -- Address port 0
  signal bram_frame_in_raddr_0          : std_logic_vector(ADDR_WIDTH-1 downto 0) := (others=>'0');
  -- Data output from read port 0
  signal bram_frame_in_dout_0           : std_logic_vector(DATA_WIDTH-1 downto 0) := (others=>'0');
  -- Read port 1 of BRAM frame_in
  -- Address port 1
  signal bram_frame_in_raddr_1          : std_logic_vector(ADDR_WIDTH-1 downto 0) := (others=>'0');
  -- Data output from read port 1
  signal bram_frame_in_dout_1           : std_logic_vector(DATA_WIDTH-1 downto 0) := (others=>'0');
  -- Read port 2 of BRAM frame_in
  -- Address port 2
  signal bram_frame_in_raddr_2          : std_logic_vector(ADDR_WIDTH-1 downto 0) := (others=>'0');
  -- Data output from read port 2
  signal bram_frame_in_dout_2           : std_logic_vector(DATA_WIDTH-1 downto 0) := (others=>'0');

  -- Write port of BRAM frame_out
  -- Address port
  signal bram_frame_out_waddr           : std_logic_vector(ADDR_WIDTH-1 downto 0) := (others=>'0');
  -- Data input for write port
  signal bram_frame_out_din             : std_logic_vector(DATA_WIDTH-1 downto 0) := (others=>'0');
  -- Write-enable signal
  signal bram_frame_out_we              : std_logic := '0';
  
begin
  
  -- BRAM frame_in for input frame
  -- The BRAM has 3 parallel read ports
  process( clk )
  begin
    if rising_edge(clk) then
      if (bram_frame_in_we = '1') then
      -- Write data
        bram_frame_in( to_integer(unsigned( bram_frame_in_waddr )) ) <= bram_frame_in_din;
      else
        -- Read data
        bram_frame_in_dout_0 <= bram_frame_in( to_integer(unsigned( bram_frame_in_raddr_0 )) );
        bram_frame_in_dout_1 <= bram_frame_in( to_integer(unsigned( bram_frame_in_raddr_1 )) );
        bram_frame_in_dout_2 <= bram_frame_in( to_integer(unsigned( bram_frame_in_raddr_2 )) );
      end if;
    end if;
  end process;

  -- BRAM frame_out for output frame
  process( clk )
  begin
    if rising_edge(clk) then
      if (bram_frame_out_we = '1') then
        -- Write data
        bram_frame_out( to_integer(unsigned( bram_frame_out_waddr )) ) <= bram_frame_out_din;
      else
        -- Read data
        bram_frame_out_dout <= bram_frame_out( to_integer(unsigned( bram_frame_out_raddr )) );
      end if;
    end if;
  end process;
  
 
  -- Filter process
    image_transfer_proc : process( clk )
    begin
      if rising_edge( clk ) then
        case state is
          when STATE_INIT =>
              idle <= '1';
              -- check for start of processing
              if (start = '1') then
                  idle <= '0';
                  state <= STATE_PROCESSING;
                  bram_frame_in_raddr_0 <= (others=>'0');
                  
                -- queue first pixel (1 cycle latency)
                pixel <= unsigned(bram_frame_in_dout_0);
                state <= STATE_PROCESSING;
              end if;
              -- just set done to zero (might be 1 if we just came from PROCESSING)
              done <= '0';
            bram_frame_out_we <= '0';
          --when STATE_STARTING =>
              -- queue first pixel (1 cycle latency)
           --   pixel <= unsigned(bram_frame_in_dout_0);
             -- state <= STATE_PROCESSING;
          when STATE_PROCESSING =>
              -- first pixel is processed, use as threshold
              if (n_pixel = 0) then
                  threshold <= pixel;
                bram_frame_out_waddr <= (others=>'0');
              else
              -- update to new addres for next pixel
                bram_frame_out_waddr <= std_logic_vector(unsigned(bram_frame_out_waddr) + 1); 
              end if;
              
              -- process each pixel
              if (pixel >= threshold) then
                  bram_frame_out_din <= std_logic_vector(pixel);
              else
                  bram_frame_out_din <= (others=>'0');
              end if;
              
              -- write the new pixel out
              bram_frame_out_we <= '1';
              
              -- last pixel was processed
              if (n_pixel = NBR_PIXELS-1) then
                  n_pixel <= 0;
                  done <= '1';
                  idle <= '1';
                  state <= STATE_INIT;
              else
                -- read next pixel
                bram_frame_in_raddr_0 <= std_logic_vector(unsigned(bram_frame_in_raddr_0) + 1); 
                pixel <= unsigned(bram_frame_in_dout_0);
              n_pixel <= n_pixel + 1;
              end if; 
        end case;
      end if;
    end process image_transfer_proc;
end Behavioral;
