----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/18/2018 05:27:02 PM
-- Design Name: 
-- Module Name: hw_counter - Behavioral
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

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity hw_counter is
    Port ( btns : in STD_LOGIC_VECTOR (3 downto 0);
           clk : in STD_LOGIC;
           leds : out STD_LOGIC_VECTOR (3 downto 0));
end hw_counter;

architecture Behavioral of hw_counter is
    
	signal count : STD_LOGIC_VECTOR (3 downto 0) := "0000";
	signal btn_0, btn_1 : std_logic_vector(1 downto 0) := "00";
	-- t = 1/f = 1/100*10^6 s = 0.01micros
	-- 200 ms / 0.01micros = 200 / 0.01*10^-3 = 20*10^6    
	signal debounce: integer range 0 to 20000000 := 0;
	
begin
    
	process (btns(1 downto 0), clk, debounce) begin
	   if (clk'event and clk='1') then
	       btn_0 <= btn_0(0) & btns(0);
           btn_1 <= btn_1(0) & btns(1);
	              
            -- currently not debouncing
            if (debounce=0) then
                -- detect event on button 0
                if not(btn_0(0) = btn_0(1)) then
                    -- if rising increment
                    if (btns(0)='1') then
                        count <= std_logic_vector(unsigned(count) + 1 );
                    -- start debouncing
                    end if; 
                    debounce <= 1;
                end if;
                -- detect event on button 0
                if not(btn_1(0) = btn_1(1)) then
                    -- if rising increment
                    if (btns(1)='1') then
                        count <= std_logic_vector(unsigned(count) - 1 );
                    -- start debouncing
                    end if; 
                    debounce <= 1;
                end if;
            end if;
		
            if (debounce= 20000000) then
                -- debouncing finished
                debounce <= 0;
            elsif (debounce > 0) then
                debounce <= debounce + 1;
            end if;
        end if;
	end process;
	
	--process (clk) begin
	--	if (clk'event and clk='1') then
	--		if (debounce= 20000000) then
	--			-- debouncing finished
	--			debounce <= 0;
	--		elsif (debounce > 0) then
	--			debounce <= debounce + 1;
	--		end if;
	--	end if;
	--end process;
	
	leds <= count;

end Behavioral;
