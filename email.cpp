//Objective: Extract email addresses embedded in the first (input) text file, and copy them to the second (output) text file.
//Name: Carlo Antonio Bilbao
//Course: COMSC-110-8269
//Compiler: TDM MinGW
//Editor: MS NotePad

//libraries
#include <deque>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

#include <ctype.h>

//Programmer defined data types
//NONE

//Special compiler dependent definitions
//NONE

//global constants/variables
//NONE

//Programmer defined functions
void introduction(string obj);
void promptForFiles(string* files);
string extractPossibleEmail(int index, string line);
bool isValidCharacter(char c);
string changeCase(string anEmail);
bool isUniqueEmail(string email, deque<string> emails);
void sortList(deque<string>& emails);

//main program
int main() {
  //data
  string objective="extract email addresses embedded in the first (input) text file, and copy them to the second (output) text file.";
  string files[2]; //array containing input file name and output file name respectively
  string iFileName; // is the input file name
  string oFileName; // is the output file name
  ifstream fin; // input file handle
  ofstream fout; // output file handle
  string lineHolder; // holds line from file
  deque<string> emails; //holds collection of valid emails
  string anEmail; //find string of email to be outputted
  string normalizedEmail; //lowercase version of email

  //introduction
  introduction(objective);

  //prompt for file names
  promptForFiles(files);

  //unpack file names from array
  iFileName = files[0];
  oFileName = files[1];

  //open input file
  fin.open(iFileName.c_str());
  if(!fin.good()) throw "I/O error";

  //iterate through each input file lines
  while(fin.good()) {
    getline(fin, lineHolder);

    //iterate through each character in a line
    for(int i = 0; i < lineHolder.length(); i++) {

      //finds the '@' character
      if(lineHolder[i] == '@') {

        //extract email
	anEmail = extractPossibleEmail(i, lineHolder);

        //check if email is valid
        if(anEmail != "xxx") {

          //normalize string
          normalizedEmail = changeCase(anEmail);

	  //append to unique emails list if email is unique
	  if(isUniqueEmail(normalizedEmail, emails)) {
	    emails.push_back(anEmail);
	  }

        }//end email is valid selection
      }
    }  
  } 

  //close input file
  fin.close();

  //check to see if any emails found
  if(emails.size() != 0) {

    //output unsorted emails in emails collection + label
    cout << endl << "List of Unique Emails(Unsorted):" << endl; //for UI
    for(int i = 0; i < emails.size(); i++) cout << emails[i] << endl;

    //sort emails
    sortList(emails);

    //output sorted emails in emails collection + label
    cout << endl << "List of Unique Emails(Sorted):" << endl; //for UI
    for(int i = 0; i < emails.size(); i++) cout << emails[i] << endl;

    //output emails with proper spacing and save to outfile
    cout << endl << "For easy copy and paste:" << endl; //for UI
    fout.open(oFileName.c_str());
    if(!fout.good()) throw "I/O error";

    for(int i = 0; i < emails.size(); i++) {
      if(i == emails.size() - 1) {
        cout << emails[i] << endl;
        fout << emails[i] << endl;
      } else {
        cout << emails[i] << "; ";
        fout << emails[i] << "; ";
      }
    }

    //close output file
    fout.close();

    //UI
    cout << endl << "Number of valid email addresses is " << emails.size() << ". ";
    cout << "A copy of the emails can be found in " << oFileName << "." << endl << endl;
    cout << "Open the output file and copy/paste its contents into the \"to\", \"cc\", or \"bcc\" field of any email message. It is best to use the \"bcc\" field so that everyone's email address does not appear in the message, to protect their privacy." << endl << endl;

  } else {
    cout << endl << "Sorry, no email addresses were found in the file input.txt" << endl << endl;
  }

}//main

void introduction(string obj) {
  cout << "Objective: This program will "; 
  cout << obj << endl;
  cout << "Programmer: Carlo Antonio Bilbao\n"; 
  cout << "Editor(s) used: Notepad\n"; 
  cout << "Compiler(s) used: TDM MinGW\n"; 
  cout << "File: " << __FILE__ << endl; 
  cout << "Complied: " << __DATE__ << " at " << __TIME__ << endl << endl; 
}//introduction

void promptForFiles(string* files) {
  //local variables
  string iFileName; // is the input file name
  string dFileName = "fileContainingEmails.txt"; // is the default file name
  string oFileName; // is the output file name

  //prompt user for input file, get input
  cout << "Enter input filename [default: " << dFileName << "]: ";
  getline(cin, iFileName);

  //check if anything was inputted
  if(iFileName.length() == 0) {
    iFileName = dFileName;
    dFileName =  "copyPasteMyEmails.txt";
  } else {
    dFileName = iFileName;
  }

  //prompt user for output file
  cout << "Enter output filename [default: " << dFileName << "]: ";
  getline(cin, oFileName);

  //check if anything was inputted
  if(oFileName.length() == 0) {
    oFileName = dFileName;
  }

  //output file checks
  cout << "Input file: " << iFileName << endl;
  cout << "Output file: " << oFileName << endl << endl;
  cout << "Press ENTER key to continue: ";
  cin.ignore(1000, 10);

  //package file names into array for return to main
  files[0] = iFileName;
  files[1] = oFileName;
}

string extractPossibleEmail(int index, string line) {

  //local variables
  int start; //index position of start of email
  int end; //index position of end of email
  string possibleEmail = "xxx"; //find string of email to be outputted, return
  bool hasDot = false; //checks for valid "." occuring in email

  //find the start of the email
  for (int j = index - 1; j > 0; j--) {
    //set start to first index if email is at the start of the line
    start = 0;

    //check location of start of email
    if(!isValidCharacter(line[j])) {
      start = j + 1;
      break;
    }
  } //end start of email loop
 
  //find the end of the email
  for (int h = index + 1; h < line.length(); h++) {
    
    //set end to full length of line if email reaches end of line
    end = line.length();

    //check to see that a dot is found
    if(isValidCharacter(line[h])) {
      if(line[h] == '.' && h != (index + 1)) hasDot = true;
    }

    //break away when invalid character is reached
    if(!isValidCharacter(line[h])) {
      end = h;
      break;
    }
  } //end of end of email selection

  //validate email
  if(hasDot) possibleEmail = line.substr(start, (end - start));

  //return email string 
  return possibleEmail;
} //end extractPossibleEmail

bool isValidCharacter(char c) {
  //local data
  bool isValid = false;
  int converted = c - '0';

  //check character validity
  if (c >= 'A' && c <= 'Z') return isValid = true;
  if (c >= 'a' && c <= 'z') return isValid = true;
  if (converted >= 0 && converted <= 9) return isValid = true;
  if (c == '_' || c == '.' || c == '-' || c == '+') return isValid = true;

  //return false otherwise
  return isValid;
} //end isValidCharacter

string changeCase(string anEmail) {
  //local variables
  string normalizedEmail = "";

  //convert each character to lowercase
  for(int i = 0; i < anEmail.length(); i++) {
    normalizedEmail += tolower(anEmail[i]);
  }

  return normalizedEmail;
} //end changeCase

bool isUniqueEmail(string email, deque<string> emails) {
  //local variables
  bool isUniqueEmail = true;
  string normalizedDequeEmail;

  //check for duplicate emails
  for(int k = 0; k < emails.size(); k++) {
    normalizedDequeEmail = changeCase(emails[k]);
    if(normalizedDequeEmail == email) {
      isUniqueEmail = false;
    }
  }

  return isUniqueEmail;
} // end isUniqueEmail

void sortList(deque<string>& emails){
  //local data
  string holder; //user to hold data while switching

  //sort alphabetically, case independent
  for(int i = 0; i < emails.size(); i++) {
    for(int j = i + 1; j < emails.size(); j++) {
      if (changeCase(emails[j]) < changeCase(emails[i])) {
        holder = emails[i];
        emails[i] = emails[j];
        emails[j] = holder;
      }
    }
  }
}
