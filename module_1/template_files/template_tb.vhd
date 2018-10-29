library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

entity template_tb is

end template_tb;

architecture Behavioral of template_tb is
    
    signal clk : std_logic;
    constant CLK_PERIOD : time := 10 ns;
    signal tb_btns : STD_LOGIC_VECTOR (3 downto 0);
    signal tb_leds : STD_LOGIC_VECTOR (3 downto 0);
    
    component
        hw_counter port(btns: in STD_LOGIC_VECTOR (3 downto 0);
                        clk: in STD_LOGIC; 
                        leds: out STD_LOGIC_VECTOR (3 downto 0));
    end component;
    
begin

    dut_hw_counter: hw_counter port map(btns=>tb_btns, leds=>tb_leds, clk=>clk);

    clk_process : process
    begin
        clk <= '0';
        wait for CLK_PERIOD/2;  --for half of clock period clk stays at '0'.
        clk <= '1';
        wait for CLK_PERIOD/2;  --for next half of clock period clk stays at '1'.
    end process;
    
    stimuli: process
    begin
        tb_btns <= "0001";
        wait for 100 ms;
        tb_btns <= "0001";
        wait for 400 ms;
        tb_btns <= "0000";
        wait for 10 ns;
        tb_btns <= "0001";
        wait for 300 ms;
        tb_btns <= "0010";
        wait for 250 ms;
        tb_btns <= "0010";
        wait for 100 ms;
        wait;
    end process;

end Behavioral;
