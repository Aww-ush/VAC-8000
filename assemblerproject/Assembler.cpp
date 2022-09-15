//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler(int argc, char* argv[])
	: m_facc(argc, argv)
{
	// Nothing else to do here at this point.
}
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler()
{
}
/**/
/*
Assembler::PassI()
NAME
	Assembler::PassI - Helps in filling the symbol table.
SYNOPSIS
	Assembler::PassI();
DESCRIPTION
	It establishes the location of the labels.
RETURNS
	void.
*/
/**/
void Assembler::PassI()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.

	// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string line;
		if (!m_facc.GetNextLine(line)) {

			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement and report an error if it isn't.
		if (st == Instruction::InstructionType::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.  So, skip comments.
		if (st == Instruction::InstructionType::ST_Comment)
		{
			continue;
		}
		// Handle the case where there is an error.

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {

			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}
/*

*/
/*Assembler::PassI();*/

/**/
/*
Assembler::PassII()
NAME
	Assembler::PassII - Translates the code and records errors.
SYNOPSIS
	Assembler::PassII();
DESCRIPTION
	This function first goes to the address where the symbols are stores
	passes the symbol table into a funtion called translation, which is in Instruction,
	then this fucntion begins to translate the into the required code
RETURNS
	void.

*/
/**/

void Assembler::PassII()
{
	Errors::InitErrorReporting();

	cout << "Press Enter to continue..." << endl;
	m_facc.rewind();
	int loc = 0;

	cout << "Pass II is starting" << endl;
	cout << "Location" << "\t" << "Contents" << "\t\t" <<"Original Statement" << endl;
	// store the generated code
	int translatedCode = 0;
	// stores the error
	string errorMsg = "";
	int errorCount = 0;

	for (;;)
	{
		errorMsg = "";
		errorCount = 0;
		//Errors::InitErrorReporting();
		vector<string> storage;
		// Read the next line from the source file.
		string line;

		if (!m_facc.GetNextLine(line)) {

			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			// see if there are other lines after end statement
			errorMsg = "Missing an end statement";
			cout << "Missing an end statement" << endl;
			errorCount++;
			Errors::RecordError(errorMsg);

			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(line);
		if (st == Instruction::InstructionType::ST_End)
		{
			cout << "\t\t\tend" << endl;
			if (m_facc.GetNextLine(line)) {
				errorMsg = "End statement not present or End is not the last statement.";
				cout << "End statement not present or End is not the last statement." << endl;
				errorCount++;
				Errors::RecordError(errorMsg);
				return;
			}
			return;
		}

		// Labels can only be on machine language and assembler language
		// instructions.  So, skip comments.
		if (st == Instruction::InstructionType::ST_Comment)
		{
			cout << "\t\t\t\t\t" << line << endl;
			continue;
		}
		// prints the location of the instruciton
		cout << loc;
		// here the translation of the instruction is done

		long long mt = m_inst.translation(line, m_symtab);
		
		// only adds into the memory if it is machine language
		m_emul.insertMemory(loc, mt);
		// Compute the location of the next instruction.
		if (m_inst.LocationNextInstruction(loc) > 999999) {
			errorMsg = "Insufficient Memory for Translation";
			Errors::RecordError(errorMsg);
			Errors::DisplayErrors();
			return;
		}
		loc = m_inst.LocationNextInstruction(loc);
		// formatiing for the output
		cout << "\t\t\t" << line << endl;



	}
}
/*void Assembler::PassII()*/
/**/

/*
Assembler::RunProgramInEmulator()
NAME
    Assembler::RunProgramInEmulator - starts the emulator
SYNOPSIS
    void Assembler::RunProgramInEmulator();
DESCRIPTION
    Checks if there are any errors and if there are, displays them.
    If there are no errors, it runs the emulation.
RETURNS
    void.

*/
/**/
void Assembler::RunProgramInEmulator()
{
	
	if (Errors::NumErrors() == true) {
		cout << "Press Enter to continue..." << endl;
		m_emul.runProgram();
	}
	else
		Errors::DisplayErrors();
}
/* void Assembler::RunProgramInEmulator();*/


