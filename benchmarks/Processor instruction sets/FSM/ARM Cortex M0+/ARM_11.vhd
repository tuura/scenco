LIBRARY ieee;
USE ieee.std_logic_1164.all; 

LIBRARY work;

ENTITY ARM_controller IS 
   PORT(
      -- inputs
      GO : IN  STD_LOGIC;
      CODE : IN  STD_LOGIC_VECTOR(3 downto 0);
      ACK_pciu :  IN  STD_LOGIC;
      ACK_ifu :  IN  STD_LOGIC;
      ACK_alu :  IN  STD_LOGIC;
      ACK_mau :  IN  STD_LOGIC;
      ACK_pciu2 :  IN  STD_LOGIC;
      ACK_ifu2 :  IN  STD_LOGIC;
      CLK :  IN  STD_LOGIC;
      RST :  IN  STD_LOGIC;

      -- outputs
      FINE : OUT  STD_LOGIC;
      REQ_pciu : OUT  STD_LOGIC;
      REQ_ifu : OUT  STD_LOGIC;
      REQ_alu : OUT  STD_LOGIC;
      REQ_mau : OUT  STD_LOGIC;
      REQ_pciu2 : OUT  STD_LOGIC;
      REQ_ifu2 : OUT  STD_LOGIC);
END ARM_controller;

ARCHITECTURE behavioural OF ARM_controller IS 

type state is (start,
		s1_1, s1_2, s1_3, s1_4, s1_5, s1_6,
		s2_1, s2_2, s2_3,
		s3_1, s3_2, s3_3,
		s4_1, s4_2,
		s5_1, s5_2,
		s6_1, s6_2, s6_3, s6_4,
		s7_1, s7_2,
		s8_1, s8_2, s8_3, s8_4,
		s9_1, s9_2,
		s10_1, s10_2, s10_3, s10_4, s10_5,
		s11_1, s11_2,
		done, done2);
signal cs, nx: state;

begin

process (CLK,RST)
begin
   if (RST='1') then      cs <= start;
   elsif (CLK'event and CLK='1') then
      cs <= nx;
   end if;
end process;

process (cs, ACK_pciu, ACK_ifu, ACK_alu, ACK_mau,
         ACK_pciu2, ACK_ifu2, go, code)
begin
	case cs is
		when start =>
			FINE <= '0';
			REQ_pciu <= '0';
			REQ_ifu <= '0';
			REQ_alu <= '0';
			REQ_mau <= '0';
			REQ_pciu2 <= '0';
			REQ_ifu2 <= '0';
			if (go = '1') then
				case code is
					when "0000" =>
						nx <= s1_1;
					when "0001" =>
						nx <= s2_1;
					when "0010" =>
						nx <= s3_1;
					when "0011" =>
						nx <= s4_1;
					when "0100" =>
						nx <= s5_1;
					when "0101" =>
						nx <= s6_1;
					when "0110" =>
						nx <= s7_1;
					when "0111" =>
						nx <= s8_1;
					when "1000" =>
						nx <= s9_1;
					when "1001" =>
						nx <= s10_1;
					when "1010" =>
						nx <= s11_1;
				end case;
			end if;

		-- LDR_STRIMM
		when s1_1 =>
			REQ_pciu <= '1';
			if (ACK_pciu = '1') then
				nx <= s1_2;
			end if;
		when s1_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= s1_3;
			end if;
		when s1_3 =>
			REQ_alu <= '1';
			if (ACK_alu = '1') then
				nx <= s1_4;
			end if;
		when s1_4 =>
			REQ_mau <= '1';
			if (ACK_mau = '1') then
				nx <= s1_5;
			end if;
		when s1_5 =>
			REQ_pciu2 <= '1';
			if (ACK_pciu2 = '1') then
				nx <= s1_6;
			end if;
		when s1_6 =>
			REQ_ifu2 <= '1';
			if (ACK_ifu2 = '1') then
				nx <= done;
			end if;

		-- LDR_REG_PC
		when s2_1 =>
			REQ_alu <= '1';
			if (ACK_alu = '1') then
				nx <= s2_2;
			end if;
		when s2_2 =>
			REQ_mau <= '1';
			if (ACK_mau = '1') then
				nx <= s2_3;
			end if;
		when s2_3 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= done;
			end if;

		-- NOP
		when s3_1 =>
			REQ_pciu <= '1';
			if (ACK_pciu = '1') then
				nx <= s3_2;
			end if;
		when s3_2 =>
			REQ_pciu2 <= '1';
			if (ACK_pciu2 = '1') then
				nx <= s3_3;
			end if;
		when s3_3 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= done;
			end if;

		-- STR_LDRREG
		when s4_1 =>
			REQ_pciu <= '1';
			REQ_alu <= '1';
			if (ACK_pciu = '1' AND ACK_alu = '1') then
				nx <= s4_2;
			end if;
		when s4_2 =>
			REQ_ifu <= '1';
			REQ_mau <= '1';
			if (ACK_ifu = '1' AND ACK_mau = '1') then
				nx <= done;
			end if;

		-- POP_PC
		when s5_1 =>
			REQ_mau <= '1';
			if (ACK_mau = '1') then
				nx <= s5_2;
			end if;
		when s5_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= done;
			end if;

		-- 123 TO PC BRANCH
		when s6_1 =>
			REQ_pciu <= '1';
			if (ACK_pciu = '1') then
				nx <= s6_2;
			end if;
		when s6_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= s6_3;
			end if;
		when s6_3 =>
			REQ_alu <= '1';
			if (ACK_alu = '1') then
				nx <= s6_4;
			end if;
		when s6_4 =>
			REQ_ifu2 <= '1';
			if (ACK_ifu2 = '1') then
				nx <= done;
			end if;

		-- LDM STM
		when s7_1 =>
			REQ_pciu <= '1';
			REQ_mau <= '1';
			if (ACK_pciu = '1' AND ACK_mau = '1') then
				nx <= s7_2;
			end if;
		when s7_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= done;
			end if;

		-- 123 TO RN
		when s8_1 =>
			REQ_pciu <= '1';
			if (ACK_pciu = '1') then
				nx <= s8_2;
			end if;
		when s8_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= s8_3;
			end if;
		when s8_3 =>
			REQ_alu <= '1';
			REQ_pciu2 <= '1';
			if (ACK_alu = '1' AND ACK_pciu2 = '1') then
				nx <= s8_4;
			end if;
		when s8_4 =>
			REQ_ifu2 <= '1';
			if (ACK_ifu2 = '1') then
				nx <= done;
			end if;

		-- RN TO RN
		when s9_1 =>
			REQ_pciu <= '1';
			REQ_alu <= '1';
			if (ACK_pciu = '1' AND ACK_alu = '1') then
				nx <= s9_2;
			end if;
		when s9_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= done;
			end if;

		-- LDR IMM PC
		when s10_1 =>
			REQ_pciu <= '1';
			if (ACK_pciu = '1') then
				nx <= s10_2;
			end if;
		when s10_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= s10_3;
			end if;
		when s10_3 =>
			REQ_alu <= '1';
			if (ACK_alu = '1') then
				nx <= s10_4;
			end if;
		when s10_4 =>
			REQ_mau <= '1';
			if (ACK_mau = '1') then
				nx <= s10_5;
			end if;
		when s10_5 =>
			REQ_ifu2 <= '1';
			if (ACK_ifu2 = '1') then
				nx <= done;
			end if;

		-- RN TO PC
		when s11_1 =>
			REQ_alu <= '1';
			if (ACK_alu = '1') then
				nx <= s11_2;
			end if;
		when s11_2 =>
			REQ_ifu <= '1';
			if (ACK_ifu = '1') then
				nx <= done;
			end if;

		when done =>
			FINE <= '1';
			if (go = '0') then
				nx <= done2;
			end if;

		when done2 =>
			REQ_pciu <= '0';
			REQ_ifu <= '0';
			REQ_alu <= '0';
			REQ_mau <= '0';
			REQ_pciu2 <= '0';
			REQ_ifu2 <= '0';
			if (ACK_pciu = '0' and
			    ACK_ifu = '0' and
			    ACK_alu = '0' and
			    ACK_mau = '0' and
			    ACK_pciu2 = '0' and
			    ACK_ifu2 = '0') then
				nx <= start;
			end if;
end case;
end process;

END behavioural;
