----------------------------------------------------------------------------------
// Company: Inipro, Inc.
// Engineer: Polo
-- 
-- Create Date: 2019/03/07 20:01:54
-- Design Name: 
-- Module Name: led_counter - Behavioral
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
use IEEE.STD_LOGIC_unsigned.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity led_counter is
    Port (
        clk : in std_logic;
        pmod_a : out std_logic_vector(7 downto 0)
    );
end led_counter;

architecture Behavioral of led_counter is

    signal cnt : std_logic_vector(27 downto 0);
    
begin

    process(clk)
    begin
        if rising_edge(clk) then
            cnt <= cnt + 1;
        end if;
    end process;
    
    pmod_a <= cnt(27 downto 20);
    
end Behavioral;
