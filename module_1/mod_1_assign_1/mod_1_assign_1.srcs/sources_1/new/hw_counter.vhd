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
    
begin

end Behavioral;
