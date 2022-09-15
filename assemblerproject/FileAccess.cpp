//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/**/
/*
FileAccess::FileAccess()
NAME
    FileAccess::FileAccess - opening file to read the content
SYNOPSIS
    FileAccess::FileAccess(int argc, char *argv[]);
    argc	->  number of command-line arguments.
    argv	->	array of the name of file(s)
DESCRIPTION
    Constructor for FileAccess class that is used to
    open the file to read the code from.
    It exits with error if the number of command-line
    argument is not equal to 2
    and if the file to be read from does not exist.
Returns
    Constructor so no return

*/
/**/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
/*FileAccess::FileAccess(int argc, char *argv[]);*/

/**/
/*
FileAccess::~FileAccess()
NAME
    FileAccess::FileAccess - used to close the file that we read from
SYNOPSIS
    FileAccess::~FileAccess();

DESCRIPTION
    Destructor to close the file

Returns
    Destructor so no return value

*/
/**/
FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}
/*FileAccess::~FileAccess();*/
/**/

/*
FileAccess::GetNextLine()
NAME
    FileAccess::GetNextLine - takes the next line of string form file
SYNOPSIS
    bool FileAccess::GetNextLine(string &a_line);
    a_line -> contains sring for next line inside file
DESCRIPTION
    Checks for next line.
    if there is next line stores it and returns true else false
Returns
    Returns bool
    - true if next line exists
    - else returns false.

/**/

bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}
/*bool FileAccess::GetNextLine(string &a_line);*/
/**/
/*
FileAccess::rewind()
NAME
    FileAccess::Rewind - points to the begining of the file
SYNOPSIS
    void FileAccess::Rewind();
DESCRIPTION
    file flags are cleaned and goes back to the beginning
    of the file.
Returns
    void

*/
/**/
void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
/*void FileAccess::rewind();*/
    
