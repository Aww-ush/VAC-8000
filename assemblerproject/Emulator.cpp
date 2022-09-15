#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "Emulator.h"
/**/
/*
Emulator::insertMemory()
NAME
	Emulator::insertMemory - stores content on memory location
SYNOPSIS
	bool Emulator::insertMemory(int a_location, long long a_contents);
	a_location -> location for storeing transnlated code
	a_contents -> content is to be stored.
DESCRIPTION
	Stores the translated code in the specified memory location.
RETURNS
	Returns true if the a_contents was stored in the location a_location, else returns false.

*/
/**/
bool emulator::insertMemory(int a_location, long long a_contents)
{
	if (a_location >= 0 && a_location < MEMSZ)
	{
		m_memory[a_location] = a_contents;
		return true;
	}
	return false;
}
/*bool Emulator::insertMemory(int a_location, long long a_contents);*/
/**/
/*
emulator::runProgram()
void emulator::runProgram()
NAME
	emulator::runProgram() - runs the emulator
SYNOPSIS
	void emulator::runProgram();
DESCRIPTION
	This function executes commands according to the content
Returns
	nothing
*/
/**/
// Runs the program recorded in memory.
void emulator::runProgram()
{
	int inpt = 0;	
	int opCode = 0;			
	int register1 = 0;	
	int register2 = 0;
	int address = 0;		
	int location = 100;		

	cout << "Emulator" << endl;
	for (int loc = 0; loc < MEMSZ; loc++)
	{
		int content = m_memory[loc];
		opCode = content / 10000000;
		register1 = (content / 1000000) % 10;
		register2 = (content / 100000) % 10;
		address = content % 10000;
		if (content != 0)
		{
			
			switch (opCode) {

			case 1:
				// add reg <-- c(Reg) + c(ADDR)
				// adding the content of mem location and register
				m_reg[register1] += m_memory[address];
				break;


			case 2:
				// substract reg <-- c(Reg) - c(ADDR)
				// adding the content of mem location and register
				m_reg[register1] -= m_memory[address];
				break;

			case 3:
				// multiply reg <-- c(Reg) * c(ADDR)
				// adding the content of mem location and register
				m_reg[register1] *= m_memory[address];
				break;


			case 4:
				// divide reg <-- c(Reg) / c(ADDR)
				// divide the content of mem location and register
				m_reg[register1] /= m_memory[address];
				break;


			case 5:
				// loads the content of the address into the register
				// ADDR <- c(REG)
				m_reg[register1] = m_memory[address];
				break;

			case 6:
				// ADDR <- c(Reg)
				// the content are stored inside the specified memory location
				m_memory[address] = m_reg[register1];
				break;

			case 7:
				// adding two registers
				// REG1 <--c(REG1) + c(REG2) 
				m_reg[register1] += m_reg[register2];
				break;

			case 8:
				// adding two registers
				// REG1 <--c(REG1) - c(REG2) 
				m_reg[register1] -= m_reg[register2];
				break;

			case 9:
				// multplying two registers
				// REG1 <--c(REG1) * c(REG2) 
				m_reg[register1] *= m_reg[register2];
				break;

			case 10:
				// dividing two registers
				// REG1 <--c(REG1) / c(REG2) 
				m_reg[register1] /= m_reg[register2];
				break;

			case 11:
				// A line is read in and the number found there is recorded in the specified 
				//memory address.  The register value is ignored.
				cout << "?";
				cin >> inpt;
				m_memory[address] = inpt;
				break;

			case 12:
				//  c(ADDR) is displayed  The register value is ignored.
				cout << m_memory[address] << endl;
				break;
			case 13:
				// goes to ADDR for next instruction
				// the register value is ignored
				loc = address;
				break;

			case 14:
				// ge to ADDR  if c(Reg) < 0
				if (m_reg[register1] < 0)
					loc = address - 1;
				break;

			case 15:
				// ge to ADDR  if c(Reg) = 0
				if(m_reg[register1] == 0)
					loc = address - 1;
				break;


			case 16:
				// ge to ADDR  if c(Reg) > 0
				if (m_reg[register1] > 0)
					loc = address - 1;
				break;

			case 17:
				// terminate instruction
				loc = 9999999;
				break;
			}
		}
	}
}
/*void emulator::runProgram();*/
/**/

