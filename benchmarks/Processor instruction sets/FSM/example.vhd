LIBRARY ieee;
USE ieee.std_logic_1164.all; 

LIBRARY work;

-- Controller modelling two scenarios
ENTITY example_controller IS 
   PORT(
      -- inputs
      GO : IN  STD_LOGIC;
      CODE : IN  STD_LOGIC_VECTOR(0 downto 0);
      ACK_fetch :  IN  STD_LOGIC;
      ACK_decode :  IN  STD_LOGIC;
      ACK_loadA :  IN  STD_LOGIC;
      ACK_loadB :  IN  STD_LOGIC;
      ACK_ALU :  IN  STD_LOGIC;
      ACK_mem :  IN  STD_LOGIC;
      ACK_PC :  IN  STD_LOGIC;
      CLK :  IN  STD_LOGIC;
      RST :  IN  STD_LOGIC;
	  
      -- codes
      b : IN STD_LOGIC_VECTOR(0 downto 0); -- binary code

      -- outputs
      FINE : OUT  STD_LOGIC;
      REQ_fetch : OUT  STD_LOGIC;
      REQ_decode : OUT  STD_LOGIC;
      REQ_loadA : OUT  STD_LOGIC;
      REQ_loadB : OUT  STD_LOGIC;
      REQ_ALU : OUT  STD_LOGIC;
      REQ_mem : OUT  STD_LOGIC;
      REQ_PC : OUT  STD_LOGIC);
END example_controller;

ARCHITECTURE behavioural OF example_controller IS 

type state is (
   s0, -- initial state
   s1, s2, s3, s4, s5, s6, -- Arithmetic instruction, encoded by b = 0
   s7, s8, s9, s10, s11, s12, -- Unconditional branch, encoded by b = 1
   s13); -- final state
signal current_state, next_state: state;

begin

process (CLK,RST)
begin
   if (RST='1') then
      current_state <= s0;
   elsif (CLK'event and CLK='1') then
      current_state <= next_state;
   end if;
end process;

process (current_state, ACK_fetch, ACK_decode, ACK_loadA, ACK_loadB,
         ACK_alu, ACK_mem, ACK_PC, go, code)
begin
   case current_state is
      -- initial state
      when s0 =>
         FINE <= '0';
         REQ_fetch <= '0';
         REQ_decode <= '0';
         REQ_loadA <= '0';
         REQ_loadB <= '0';
         REQ_ALU <= '0';
         REQ_mem <= '0';
         REQ_PC <= '0';
         if ( go = '1') then
		    case b is
		       when '0' => next_state <= s1;
		       when '1' => next_state <= s7;
		    end case;
         end if;

      -- Arithmetic instructions
      when s1 =>
         REQ_fetch <= '1';
         if ( ACK_fetch = '1') then
            next_state <= s2;
         end if;
      when s2 =>
         REQ_decode <= '1';
         if ( ACK_decode = '1') then
            next_state <= s3;
         end if;
      when s3 =>
         REQ_loadA <= '1';
		 REQ_loadB <= '1';
         if ( ACK_loadA = '1' AND ACK_loadB = '1') then
            next_state <= s4;
         end if;
      when s4 =>
         REQ_ALU <= '1';
         if ( ACK_ALU = '1') then
            next_state <= s5;
         end if;
      when s5 =>
         REQ_mem <= '1';
         if ( ACK_mem = '1') then
            next_state <= s6;
         end if;
      when s6 =>
         done <= '1';
         if ( go = '0') then
            next_state <= s13;
         end if;

      -- Unconditional branch
      when s7 =>
         REQ_fetch <= '1';
         if ( ACK_fetch = '1') then
            next_state <= s8;
         end if;
      when s8 =>
         REQ_decode <= '1';
         if ( ACK_decode = '1') then
            next_state <= s9;
         end if;
      when s9 =>
         REQ_loadA <= '1';
         if ( ACK_loadA = '1') then
            next_state <= s10;
         end if;
      when s10 =>
         REQ_ALU <= '1';
         if ( ACK_ALU = '1') then
            next_state <= s11;
         end if;
      when s11 =>
         REQ_PC <= '1';
         if ( ACK_PC = '1') then
            next_state <= s12;
         end if;
      when s12 =>
         done <= '1';
         if ( go = '0') then
            next_state <= s13;
         end if;

      -- final state
      when s13 =>
         REQ_fetch <= '0';
         REQ_decode <= '0';
         REQ_loadA <= '0';
         REQ_loadB <= '0';
         REQ_ALU <= '0';
         REQ_mem <= '0';
         REQ_PC <= '0';
         if ( ACK_fetch = '0' AND
	      ACK_decode = '0' AND
	      ACK_loadA = '0' AND
	      ACK_loadB = '0' AND
	      ACK_ALU = '0' AND
	      ACK_mem = '0' AND
	      ACK_PC = '0') then
            next_state <= s0;
         end if;
		 
   end case;
end process;

END behavioural;
