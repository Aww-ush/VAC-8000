#include "stdafx.h"
#include "SymTab.h"
#include "Instruction.h"
#include "Emulator.h"
#include "Errors.h"
#include <string>

using namespace std;
// parse an instruction and return its type.  Yes, I want a call by value.
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
	// Record the original statement.  This will be needed in the sceond pass.
	m_instruction = a_line;

	// Delete any comment from the line.
	DeleteComment(a_line);

	// Record label, opcode, and operands.  Up to you to deal with formatting errors.
	bool isFormatError = RecordFields(a_line);

	// Check if this is a comment.
	if (m_Label.empty() && m_OpCode.empty())
	{
		return InstructionType::ST_Comment;
	}
	// Return the instruction type.
	return m_type;
}
/**/
/*
Instruction::RecordFields()
NAME
	Instruction::RecordFields - Record the fields that make up the instructions.
SYNOPSIS
	bool Instruction::RecordFields( const string &a_line );
	a_line -> a string that consists of a line in the file to be parsed.
DESCRIPTION

	The function calls another function to parse the line into label, opcode and operands of instruction
	then it determines the of opcode: instruction or numeric
	records error if too many operands given

Returns

	Returns bool value
	- true if parsing the line into fields(label, opcode, operand1, operand2) was successful 
	- false otherwise
*/
/**/
bool Instruction::RecordFields(const string& a_line)
{
	// Get the fields that make up the instruction.
	// CHANGED THE CODE HERE
	bool isFormatError = !ParseLineIntoFields(a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);
	if (isFormatError)
	{
		m_errorMsg = "Too many operands";
		cout << "Too many operands" << endl;
		Errors::RecordError(m_errorMsg);
	}
	// if code was a comment, there is nothing to do.
	if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

	// Record whether the operands are numeric and their value if they are.
	m_IsNumericOperand1 = isStrNumber(m_Operand1);
	if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

	m_IsNumericOperand2 = isStrNumber(m_Operand2);
	if (m_IsNumericOperand2) m_Operand1NumericValue = stoi(m_Operand2);

	// For the sake of comparing, convert the op code to upper case.
	for (char& c : m_OpCode)
	{
		c = toupper(c);
	}
	// I leave to the class the tasks:
	// - Determine and record the instruction type from the op code.
	// there are 5 types of opcode
	// seperating each and checking what matched with what
	// - Recording the numberic Op code for machine lanuage equivalents.

	string MCODE[] = { "ADD", "SUB", "MULT", "DIV", "LOAD", "STORE","ADDR",
						"SUBR", "MULTR", "DIVR", "READ", "WRITE", "B", "BM", "BZ", "BP", "HALT" };
	if (!m_OpCode.empty())
	{

		if (m_OpCode == "END")
		{
			m_type = InstructionType::ST_End;
			
		}
		else if (m_OpCode == "DC" || m_OpCode == "DS" || m_OpCode == "ORG")
		{
			m_type = InstructionType::ST_AssemblerInstr;
		}
		else
		{
			for (int i = 0; i < 17; i++)
			{
				if (MCODE[i] == m_OpCode)
				{
					m_type = InstructionType::ST_MachineLanguage;
					m_NumOpCode = i + 1;
				}
			}
		}
		return true;

	}
	else
	{
		cout << "there is an error in OP Code" << endl;
		m_type = InstructionType::ST_Error;
	}
	return true;
}
/*bool Instruction::RecordFields( const string &a_line );*/
/**/

/**/
/*
Instruction::ParseLineIntoFields()
NAME
	Instruction::ParseLineIntoFields - Extracts the fields from instruction.
SYNOPSIS
	bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
	string& a_Operand1, string& a_Operand2);
	a_line -> string of a line to be parsed.
	a_label -> string that stores parsed label
	a_OpCode -> string that stores parsed OpCode
	a_Operand1 -> string that stores operand1
	a_Operand2 -> string that stores operand2
DESCRIPTION
	The function parses the line into
	label, operation code, and operands of instruction
	from the given line.

Returns
	Returns a boolean
	- true if there were no extra operands
	- false otherwise.
*/
/**/

bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
	string& a_Operand1, string& a_Operand2)
{
	// Get rid of any commas from the line.
	replace(a_line.begin(), a_line.end(), ',', ' ');

	// Get the elements of the line.  That is the label, op code, operand1, and operand2.
	string endStr;
	a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
	istringstream ins(a_line);
	if (a_line[0] == ' ' || a_line[0] == '\t')
	{
		a_label = "";
		ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
	}
	else
	{

		ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
	}
	// If there is extra data, return false.
	return endStr.empty() ? true : false;
}
/*bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,*/
/**/
/*
Instruction::isStrNumber()
NAME
	 Instruction::isStrNumber - finds if the string in the parameter is a number.
SYNOPSIS
	bool Instruction::isStrNumber(const string& a_str);
	a_str -> contains the string that has to be chaeckeds

DESCRIPTION
	The function checks whether the string is a number

Returns
	Returns Bool
	- true if a_str is a number
	- false otherwise.
*/
/**/
bool Instruction::isStrNumber(const string& a_str)
{
	if (a_str.empty()) return false;

	// If there is a - or a plus, make sure there are more characters.
	int ichar = 0;
	if (a_str[0] == '-' || a_str[0] == '+')
	{
		ichar++;
		if (a_str.length() < 2) return false;
	}
	// Make sure that the remaining characters are all digits
	for (; ichar < a_str.length(); ichar++)
	{
		if (!isdigit(a_str[ichar])) return false;
	}
	return true;
}
/*bool Instruction::isStrNumber(const string& a_str);*/
/**/
/*
	long long Instruction::translation()
NAME
	long long Instruction::translation - converts the symbols into opcode 
SYNOPSIS
	long long Instruction::translation(const string& a_line, SymbolTable m_symtab);
	m_symtab -> symbol table passed as reference

DESCRIPTION
	Translates the instruction into content 
	First it checks if the type is assembly or machine
	Then converts into required content
	Also checks for error
	Also prints the content on the screen
RETURNS

		long long value which is the translated instruction

AUTHOR

		Aayush

DATE

		6:27pm 5/1/2022

*/
/**/
long long Instruction::translation(const string& a_line, SymbolTable m_symtab)
{
	long long value = 0;
	int tmpLoc = 0;
	//throw error if symbolic operands are not defined
	ErrorCheck(m_symtab);
	if (m_type == Instruction::InstructionType::ST_AssemblerInstr) {
		
		if (m_OpCode == "DC" || m_OpCode == "DS") {
			if (!Instruction::isLabel()) {
				m_errorMsg = "Missing Label";
				cout << "Missing Label" << endl;
				Errors::RecordError(m_errorMsg);
				// determine content

			}
			if (stol(m_Operand1) > 999999)
			{
				if (m_OpCode == "DC")
				{
					m_errorMsg = "Too large Operand Value";
					cout << "Too large Operand Value" << endl;
					Errors::RecordError(m_errorMsg);
				}
				else
				{
					m_errorMsg = "Insuffient Memory";
					cout << "Insuffient Memory" << endl;
					Errors::RecordError(m_errorMsg);
				}
			}
		}
		if (m_OpCode == "DC") {

			if (!m_IsNumericOperand1)
			{
				m_errorMsg = "Syntax error in construction of Operand";
				Errors::RecordError(m_errorMsg);
			}
			if (m_Operand2 != "")
			{
				m_errorMsg = "too many operands";
				Errors::RecordError(m_errorMsg);
			}
			value = long(m_NumOpCode) * 10000000;
			int tmpSize = 9 - m_Operand1.length();
			value = stol(m_Operand1);
			cout << "          ";
			for (int i = 0; i < tmpSize; i++)
			{
				cout << "0";
			}
			cout << value;

			return value;
		}
		else
		{
			if (m_OpCode == "DS")// for formatting
				cout << "        ";
			cout << "           ";
			return 0;
		}
	}
	else
	{
		// there is only operand one then
		// read write halt and branch
		if (m_NumOpCode == 11 || m_NumOpCode == 12 || m_NumOpCode == 13 || m_NumOpCode == 17)
		{
			if (m_NumOpCode != 17)
			{
				if (m_IsNumericOperand1 && m_IsNumericOperand2)
				{
					cout << "here \n" << endl;
					m_errorMsg = "One operand must be symbolic";
					cout << "One operand must be symbolic" << endl;
					Errors::RecordError(m_errorMsg);
				}
			}

			value = long(m_NumOpCode) * 10000000;
			// checking if they ahve register value or not
			// if they ahve register value then adding those
			// else adding 9
			if (m_IsNumericOperand1) {
				value += stol(m_Operand1) * 1000000;
				m_symtab.LookupSymbol(m_Operand2, tmpLoc);
				value += tmpLoc;
			}
			// since there are no register adding 9
			else {
				
				value += 9 * 1000000;
				// this must mean that it is a address so
				m_symtab.LookupSymbol(m_Operand1, tmpLoc);
				value += tmpLoc;
			}
			cout << "          " << value;
			// adding the translated code to value
			return value;
		}
		// there is second operand present
		else
		{
			value = long(m_NumOpCode) * 10000000;
			twoOperands(value, m_symtab);
			return value;

		}
		
	}
	cout << "Cound not translate" << endl;
	return -1;
}
/*long long Instruction::translation(const string& a_line, SymbolTable m_symtab)*/
/**/
/*
	void Instruction::twoOperands()
NAME
	void Instruction::twoOperands - converts the symbols into opcode which needs two operands
SYNOPSIS
	void Instruction::twoOperands(long long& value, SymbolTable& m_symtab);
	m_symtab -> symbol table passed as reference
	value -> 

DESCRIPTION
	Converts the symbols into instruction for the emulator
RETURNS

		void return type

AUTHOR

		Aayush

DATE

		6:27pm 5/1/2022

*/
/**/
void Instruction::twoOperands(long long& value, SymbolTable& m_symtab)
{
	int tmpLoc;
	// since there are two operands
	// we have to see if the operands are actually necessary or not

	if (m_Operand1.empty() || m_Operand2.empty())
	{
		m_errorMsg = "Requires two operands";
		cout << "Error: Required two operands" << endl;
		Errors::RecordError(m_errorMsg);
	}
	if (m_NumOpCode > 6 && m_NumOpCode < 11 ) {
		//illegal opcode 
		//check if the opcode is suitable	
		// for read, write branch
		if (!m_IsNumericOperand1 || !m_IsNumericOperand2)
		{
			m_errorMsg = "Illegal opCode. Use this for operation between Register and Register";
			cout << "Illegal opCode. Use this for operation between Register and Register" << endl;
			Errors::RecordError(m_errorMsg);

		}
		// this means that we do not need to add the address 
		// just display two registers to perform the oprations on
		value += stol(m_Operand1) * 1000000;
		value += stol(m_Operand2) * 100000;
		cout << "           " << value;

		return;
	}
	else {
		if (!m_IsNumericOperand1)
		{
			m_errorMsg = "Illegal opCode. Use this for operation between Register and Memory";
			Errors::RecordError(m_errorMsg);

		}
		m_symtab.LookupSymbol(m_Operand2, tmpLoc);
		value += tmpLoc;
		// adding the register
		// formatting
		value += stol(m_Operand1) * 1000000;
		if ((value / 10000000) < 10)
			cout << "           0" << value;
		else
			cout << "           " << value;

		return;
	}
}
/*void Instruction::twoOperands(long long& value, SymbolTable& m_symtab)*/
/**/
/*
	Instruction::ErrorCheck(()
NAME
	Instruction::ErrorCheck - Finds and Records errors which is common in both Machine instruction and assembly instruction
SYNOPSIS
	void Instruction::ErrorCheck(SymbolTable& m_symtab);
	m_symtab -> symbol table passed as reference

DESCRIPTION
	Checks for errors that can occur in both machine and assembly instruction
RETURNS

		void return type

AUTHOR

		Aayush

DATE

		6:27pm 5/1/2022

*/
/**/
void Instruction::ErrorCheck(SymbolTable& m_symtab) {
	int tmpLoc = 0;
	// need an operand
	if (m_NumOpCode != 17)
	{
		if (m_Operand1.empty() && m_Operand2.empty()) {
			m_errorMsg = "No operand found";
			cout << "No operand found" << endl;
			Errors::RecordError(m_errorMsg);
		}
	}

	//mUltiply defined label check
	if (Instruction::isLabel()) {
		if (!isalpha(m_Label[0])) {
			m_errorMsg = "Invalid Label";
			Errors::RecordError(m_errorMsg);
		}
		int tempLoc = 0;
		m_symtab.LookupSymbol(Instruction::GetLabel(), tempLoc);
		if (tempLoc == m_symtab.multiplyDefinedSymbol) {
			m_errorMsg = "Symbol aready defined";
			cout << "Symbol aready defined" << endl;
			Errors::RecordError(m_errorMsg);
		}
	}
	// symbolic not found
	if (!m_IsNumericOperand1 && !m_Operand1.empty()) {

		m_symtab.LookupSymbol(m_Operand1, tmpLoc);;
		if (tmpLoc == 0) {
			cout << m_Label << " " << m_OpCode << endl;
			m_errorMsg = "Symbolic Operand Not Defined";
			Errors::RecordError(m_errorMsg);
		}

	}
	//symbolic not found
	if (!m_IsNumericOperand2 && !m_Operand2.empty()) {

		m_symtab.LookupSymbol(m_Operand2, tmpLoc);;
		if (tmpLoc == 0) {

			m_errorMsg = "Symbolic Operand Not Defined";
			Errors::RecordError(m_errorMsg);
		}

	}
	//registers cannot be greater than 9
	if (m_IsNumericOperand1 || m_IsNumericOperand2){
		if (m_IsNumericOperand1 < 0 || m_IsNumericOperand1 > 9)
		{
			m_errorMsg = "Invalid Register Number. It should be 0 < resisterNum < 9";
			Errors::RecordError(m_errorMsg);
		}
		if (m_IsNumericOperand2 < 0 || m_IsNumericOperand2 > 9)
		{
			m_errorMsg = "Invalid Register Number. It should be 0 < resisterNum < 9";
			Errors::RecordError(m_errorMsg);
		}

	}


}
/*void Instruction::ErrorCheck(SymbolTable& m_symtab); */