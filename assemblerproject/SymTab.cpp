//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"


/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function places a_symbol and its location a_loc in the symbol table.
Returns
    Void return type
*/
/**/

void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*SymbolTable::AddSymbol( const string &a_symbol, int a_loc );*/



/**/

/*
SymbolTable::DisplaySymbolTable()
NAME
    SymbolTable::DisplaySymbolTable - adds a new symbol to the symbol table.
SYNOPSIS
    void SymbolTable::DisplaySymbolTable();
DESCRIPTION

    This function will display all the symbol and their location
Returns
    Void return type
*/
/**/
void SymbolTable::DisplaySymbolTable()
{
    cout << "Symbol Table" << endl;
    map<string, int>::iterator it;

    cout << "Output from test program" << endl;
    cout << "Symbol#\tSymbol\tLocation" << endl;
    int i = 0;
    for (it = m_symbolTable.begin(); it != m_symbolTable.end(); it++)
    {
        cout << i++ << "   \t" << it->first << "   \t" << it->second << endl;
    }
}
/*void SymbolTable::DisplaySymbolTable();*/

/**/
/*
SymbolTable::LookupSymbol()
NAME
    SymbolTable::LookupSymbol - Checks if the symbol exists in the symbol table.
SYNOPSIS
    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);
        a_symbol  ->  name of the symbol to be looked for.
        a_loc     ->  location that will be changed if the symbol is found.
DESCRIPTION
    looks for the symbol a_symbol, if found stores it in a_loc
RETURNS
    Returns bool.
    - true if the symbol is found. 
    - else false

*/
/**/

bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
    // finding 
    if (m_symbolTable.find(a_symbol) != m_symbolTable.end()) {
        //getting the location and status of the symbol
        a_loc = m_symbolTable[a_symbol];
        return true;
    }
    return false;
}
/*bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)*/