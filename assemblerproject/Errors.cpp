#include "stdafx.h"

#include "Errors.h"

vector<string> Errors::m_ErrorMsgs = {};

// display
// clear
// record 
//no error
// Initializes error reports.
void Errors::InitErrorReporting()
{
    m_ErrorMsgs.clear();
}

// Records an error message.
void Errors::RecordError(string a_emsg)
{
    m_ErrorMsgs.push_back(a_emsg);

}
// Displays the collected error message.
void Errors::DisplayErrors()
{
    for (int i=0; i < m_ErrorMsgs.size(); i++)
    {
        cout << "The errors in the program are: " << endl;
        cout << m_ErrorMsgs[i] << endl;
    }

}