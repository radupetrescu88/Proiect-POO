//!!!!!!!!THE FILES CONTAINING THE TABLES ARE BINARY AND THEY HAVE THE FOLLOWING NAME FORMAT: table_name.bin (ex:Studenti.bin)

#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//Exceptions:
class InvalidCommandException
{
	string description = "";
	int errCode = 0;
public:
	InvalidCommandException(string Description, int ErrCode)
	{
		this->description = Description;
		this->errCode = ErrCode;
	}

	InvalidCommandException()
	{

	}

	string getDescription()
	{
		return this->description;
	}

	int getErrCode()
	{
		return this->errCode;
	}
};


class UsefulFunctions
{
public:
	const char* CAPS = "ABCDEFGHIKLMNOPRSTUVXYZ";
	const char* SIGNS = "(), ";
	const char* NoCAPS = "abcdefghijklmnoprstuvxyz";
	const char* NUMBERS = "0123456789";

	void copyFile(fstream& inFile, fstream& outFile)
	{
		string line;
		if (inFile && outFile)
		{
			while (getline(inFile, line))
			{
				outFile << line << "\n";
			}
		}
	}

	string extract(string str, char a, char b, int& counter1, int& counter2) {
		
		string subString;
		counter1 = str.find_first_of(a);
		counter2 = str.find_first_of(b);
		subString = str.substr(counter1, counter2 - counter1);
		return subString;
	}

	bool findChars(string command, const char* chars) {
		int i = 0;
		int counter = 0;
		while (i < strlen(chars)) {
			for (int j = 0; j < command.length(); j++) {
				if (chars[i] == command[j])
					counter++;
			}
			i++;
		}
		if (counter)
			return true;
		else return false;
	}

	bool checkAsciiValue(string str, char a, char b) {
		int counter = 0;
		for (int i = 0; i < str.length(); i++)
			if (str[i]<a || str[i]>b)
				counter++;
		if (counter)
			return true;
		else return false;
	}

	int nrChars(string str, char a, int& counter) {
		counter = 0;
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == a)
				counter++;
		}
		return counter;
	}

	string stringWithoutSpaces(string str) {
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		return str;
	}

	string subStringWithoutSpaces(string str)
	{
		int i = 0;
		string result=str;
		if (str[i] == ' ')
		{
			while (str[i] == ' ')
			{
				i++;
			}
			str.erase(0, i);
		}
		result = str;
		return result;
	}

	string stringWithoutCommasOrSpaces(string str) {
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		int i = 0;
		while (i < str.length()) {
			if (str[i] == ',' && str[i + 1] == '(' && str[i - 1] == ')') {
				str.erase(i, 1);
				i--;
			}
			i++;
		}
		return str;
	}

	static void writeStringToBin(ofstream& binFile, string str)
	{
		int dim=str.size()+1;
		binFile.write((char*)&dim, sizeof(int));
		binFile.write(str.c_str(), dim * sizeof(char));
	}


	static void readStringFromBin(ifstream& binFile, string str)
	{
		int dim;
		binFile.read((char*)dim, sizeof(int));
		char buffer[1000];
		binFile.read(buffer, dim * sizeof(char));
		str = (string)buffer;
	}

	bool isTabelInFile(fstream& textFile, string tableName)
	{
		//we consider the file to be open
		string table=".";
		while (getline(textFile, table))
		{
			if (table == tableName)
			{
				return true;
			}
		}
		if (table == ".")
			return false;
	}


};

class Command
{
	string name = "";
	string commandFirst = "";
	string commandSecond = "";
	UsefulFunctions function;

private:
	void setWords(string& name, string& commandFirst, string& commandSecond)
	{
		int counter1 = 0;
		int counter2 = 0;
		commandFirst = function.extract(name, name[0], ' ', counter1, counter2);
		name.erase(0, commandFirst.length() + 1);
		if (commandFirst == "CREATE" || commandFirst == "DROP" || commandFirst == "DISPLAY" || commandFirst == "INSERT")
		{
			commandSecond = function.extract(name, name[0], ' ', counter1, counter2);
			name.erase(0, commandSecond.length() + 1);
		}
	}

	void checkCommand(string commandFirst, string commandSecond)
	{
		if (commandSecond == "TABLE")
		{
			if (commandFirst != "CREATE" && commandFirst != "DROP" && commandFirst != "DISPLAY")
			{
				throw new InvalidCommandException("No such command!", 0);
			}
		}
		else {
			if (commandSecond == "INDEX")
			{
				if (commandFirst != "CREATE" && commandFirst != "DROP")
				{
					throw new InvalidCommandException("No such command!", 0);
				}
			}
			else {
				if (commandSecond == "")
				{
					if (commandFirst != "UPDATE" && commandFirst != "SELECT" && commandFirst != "DELETE")
					{
						throw new InvalidCommandException("No such command!", 0);
					}
				}
				else
				{
					if (commandSecond == "INTO")
					{
						if (commandFirst != "INSERT")
							throw new InvalidCommandException("No such command!", 0);
					}
					else throw new InvalidCommandException("No such command!", 0);
				}

			}
		}
	}

public:
	//setters & getters
	void setName(string commandName)
	{
		this->name = commandName;
		setWords(this->name, commandFirst, commandSecond);
		checkCommand(commandFirst, commandSecond);
	}
	string getName()
	{
		return this->name;
	}
	string getFirstWord()
	{
		return this->commandFirst;
	}
	string getSecondWord()
	{
		return this->commandSecond;
	}

	//operators
	//operators
	string operator -(string substring)
	{
		string copy = this->name;
		copy = this->name.erase(0, substring.length());
		return copy;
	}
	Command operator=(Command command)
	{
		Command copy;
		copy.setName(command.name);
		string word1;
		string word2;
		copy.setWords(command.name, word1, word2);
		return copy;
	}
	Command operator--(int) {
		Command command;
		command = *this;
		this->name.erase(1, 6);
		return command;
	}
	Command operator--() {
		this->name.erase(0, 6);//variabile??);
		return *this;
	}
	bool operator ==(Command command) {
		if (this->getName() == command.getName()) {
			return true;
		}
		else {
			return false;
		}
	}

	void writeCommandToBin(ofstream& binFile)
	{
		UsefulFunctions::writeStringToBin(binFile, this->commandFirst);
		if (commandSecond != "")
		{
			UsefulFunctions::writeStringToBin(binFile, this->commandSecond);
		}
		UsefulFunctions::writeStringToBin(binFile, this->name);
	}

	friend void operator <<(ostream& out, Command command);
	friend void operator >>(istream& input, Command& command);
	friend string operator -(string string1, string string2);

};

//substracting a string from another string
string operator -(string string1, string string2)
{
	string result;
	result = string1.erase(string1[0], string2.length());
	return result;
}

//reading a command from a file
void operator >>(ifstream& file, Command &command)
{
	string name;
	getline(file, name);
	command.setName(name);
}


//writing a command on the keyboard
void operator >>(istream& input, Command& command)
{
	cout << endl << "--------------------------------------";
	cout << endl << "Please enter command: ";
	string name;
	getline(input, name);
	command.setName(name);
}

//reading a command from the keyboard
void operator <<(ostream& out, Command command) {
	out << endl << "--------------------------------------";
	out << endl << "Operator Command:" << command.getName();
}



//THE COMMANDS

//1. DROP
//MERGE PENTRU TOATE EXEMPLELE
class DropCommand
{
	Command command;
	string tableName="";
	UsefulFunctions function;
	fstream theDatabase;
public:
	DropCommand(Command command, fstream &theDatabase)
	{
		checkDrop(command.getName());
		theDatabase.open("copyDatabase.txt", ios::in | ios::out);
		function.copyFile(theDatabase, this->theDatabase);
		doDrop(command.getName());
	}
private:
	void checkDrop(string commandName)
	{
		commandName = function.subStringWithoutSpaces(commandName);
		if (function.findChars(commandName, function.CAPS) == 1 || function.findChars(commandName, function.SIGNS) == 1 || function.findChars(commandName, function.NoCAPS) == 0 || function.findChars(commandName, " ")==true)
		{
			throw new InvalidCommandException("The DROP command has the wrong table name", 0);
		}
		else
		{
			this->tableName = commandName;
			cout << "The command is correct. We are now executing it: ";
		}
	}

	void doDrop(string tableName)
	{
		string str="nu e bine";
		getline(this->theDatabase, str);
		cout << str;
		if (function.isTabelInFile(this->theDatabase, tableName) == false)
		{
			cout << endl << "esti aici";
			throw new InvalidCommandException("There is no tabel with this name to be dropped!", 0);
		}
		else
		{

		}
	}
};

//2. DISPLAY
//MERGE PENTRU TOATE EXEMPLELE
class DisplayCommand
{
	Command command;
	string table_name = " ";
	UsefulFunctions function;
	fstream theDatabase;
public:

	DisplayCommand(Command command, fstream &theDatabase)
	{
		checkDisplay(command.getName());
		function.copyFile(theDatabase, this->theDatabase);
	}
private:
	void checkDisplay(string commandName)
	{
		commandName = function.subStringWithoutSpaces(commandName);
		if (function.findChars(commandName, function.CAPS) == 1 || function.findChars(commandName, function.SIGNS) == 1 || function.findChars(commandName, function.NoCAPS) == 0 || function.findChars(commandName, " ")==true)
		{
			throw new InvalidCommandException("The DISPLAY command has the wrong table name", 0);
		}
		else
		{
			cout << "The command is correct. We are now executing it: ";
		}
	}
};

//3.CREATE
//in constructor, dupa validare, adaugi o functie doCreate, care primeste parametrii pe care i-a scos Validare din comanda si construieste
//un fisier binar, cu titlul numeTabel.bin, care contine in ordine coloanele 
//also, doCreate adauga numele tabelului pe care il construiesti pe o linie noua (fara spatiu dupa) in fisierul txt theDatabase (care e deschis si cu in si cu out deci poti sa si citesti sa si scrii in el)
class CreateCommand
{
	Command command;
	UsefulFunctions function;
	fstream theDatabase;
public:
	CreateCommand(Command command, fstream &theDatabase)
	{
		ValidareSerioasaCreate(command.getName());
		function.copyFile(theDatabase, this->theDatabase);
	}
private:

	void createParamVandPars(string command, string& cName, string& cType, int& cDim, string& defaultVal) {
		// we strip the first column details by   ' ( '   ' ) ' 
		string editable = command.substr(command.find_first_not_of('('), command.find_last_of(')') - 1);
		string parametriiTabel;
		int counter;
		int contorEditable = 0;
		int limitEditable = function.nrChars(editable, ',', counter) + 1;
		if (function.nrChars(editable, ',', counter) != 3) {
			throw new InvalidCommandException("Too few terms inside the brackets", 0);
		}
		// we make sure we get rid of the spaces too 
		editable = function.stringWithoutSpaces(editable);
		// we  will now take every parameter to check if its the right data type 
		while (contorEditable < limitEditable) {
			parametriiTabel = editable.substr(0, editable.find_first_of(','));
			// for the first param we need a name so only text and after that we pass it to the cName
			if (contorEditable == 0) {
				if (function.checkAsciiValue(parametriiTabel, 'a', 'z') != 0) {
					throw new InvalidCommandException("Is not a text", 0);
				}
				else {
					cName = parametriiTabel;
					cout << endl;
					cout << endl << "** Primul parametrul este nume *" << cName << "* si este valid ";
				}
			}
			//for the 2nd param we need a text but this time denoting one of the types, check type and pass it to the cType 
			if (contorEditable == 1) {
				if (parametriiTabel == "text" || parametriiTabel == "integer" || parametriiTabel == "float") {
					cType = parametriiTabel;
					cout << endl;
					cout << endl << "** Parametrul al doilea este type*" << cType << "* si este valid ";
				}
				else {
					throw new InvalidCommandException("Second parameter has an error", 0);
				}

			}
			//for the 3rd param we need an integer, we check to see if only digits and convert string to int 
			if (contorEditable == 2) {
				if (function.checkAsciiValue(parametriiTabel, '0', '9') == 0) {
					cDim = stoi(parametriiTabel);
					cout << endl;
					cout << endl << "** Parametrul al treilea este int*" << cDim << "* si este valid ";
				}
				else {
					throw new InvalidCommandException("Wrong third parameter", 0);
				}
			}
			// here i dont know for sure what checks needs to be The command is correct. We are now executing it:  but the passing to the cValue its made 
			if (contorEditable == 3) {
				if (parametriiTabel.find_first_of('’')) {
					parametriiTabel.erase(remove(parametriiTabel.begin(), parametriiTabel.end(), '’'), parametriiTabel.end());
				}
				defaultVal = parametriiTabel;
				cout << endl;
				cout << endl << "** Parametrul al 4 lea este default(string) *" << parametriiTabel << "* si este valid";
			}
			editable.erase(0, parametriiTabel.length() + 1);
			contorEditable++;
		}
	}

	void ValidareSerioasaCreate(string commanda) {// string& name, string& type, int& dim, string& value) {
		string newCommand = commanda;
		string editable;
		string parametriiTabel;
		string copyEditable;
		// parametrii ce vor putea fi dati si de sus cand doriti 
		string name;
		string type;
		int dim;
		string value;
		// next
		// in order to check everything first we strip the table name until ( by any spaces in order to check it lexically 
		editable = newCommand.substr(0, newCommand.find_first_of('('));
		copyEditable = function.stringWithoutSpaces(editable);
		if (function.nrChars(copyEditable, 'I', dim) == 2)
		{
			int noLettersTableName = 0;
			noLettersTableName = copyEditable.find_first_of('I');
			if (copyEditable.compare(copyEditable.find_first_of('I'), 11, "IFNOTEXISTS") == 0 && copyEditable.length() == (noLettersTableName + 11)) {
				//here we should also check the str.substr(0,noLettersTable) with a table name to see if the table exists or not 
				copyEditable = copyEditable.erase(copyEditable.find_first_of('I'), 11);
				cout << endl << "We create new table " << copyEditable;
			}
			else
				throw new InvalidCommandException("wrong if not exists", 0);
		}
			
		if (function.checkAsciiValue(copyEditable, 'a', 'z') != 0) {
			throw new InvalidCommandException("Wrong table name", 0);
		}
		else {
			int counter;
			newCommand.erase(0, editable.length());
			if ((function.nrChars(newCommand, '(', counter) != function.nrChars(newCommand, ')', counter)) || ((function.nrChars(newCommand, '(', counter) + (function.nrChars(newCommand, ')', counter))) % 2 != 0)) {
				throw new InvalidCommandException("Wrong number of parantesis!", 0);
			}
			//after it was checked, if we have only one column we call the function once 
			else {
				if (function.nrChars(newCommand, '(', counter) == 1) {
					createParamVandPars(newCommand, name, type, dim, value);
					cout << "The command is correct. We are now executing it: !";
				}
				else   // else we will strip the columns by the commas and spaces dividing them and then call the functions one by one while also deleting from the command string
				{
					newCommand = newCommand.substr(newCommand.find_first_of('(') + 1, newCommand.find_last_of(')') - 1);
					newCommand = function.stringWithoutCommasOrSpaces(newCommand);
					while (newCommand.length()) {

						parametriiTabel = newCommand.substr(0, newCommand.find_first_of(')') + 1);
						createParamVandPars(parametriiTabel, name, type, dim, value);
						newCommand.erase(0, parametriiTabel.length());
					}
					cout << endl << "The command is correct. We are now executing it: !";
				}
			}

		}
	}
};


//4. SELECT
//MERGE PENTRU TOATE EXEMPLELE
class SelectCommand
{
	Command command;
	UsefulFunctions function;
	fstream theDatabase;
public:
	SelectCommand(Command command, fstream &theDatabase)
	{
		checkSelect(command.getName());
		function.copyFile(theDatabase, this->theDatabase);
	}
private:
	void checkSelect(string commandName)
	{
		int counter1 = 0, counter2 = 0;
		commandName = function.subStringWithoutSpaces(commandName);
		if (commandName[0] != 'A')
		{
			string selectedValues = function.extract(commandName, commandName[0], commandName[commandName.find_last_of(')') + 1], counter1, counter2);
			commandName.erase(0, selectedValues.length());
			//check the selected values
			if (selectedValues[0] != '(' || selectedValues[selectedValues.length() - 1] != ')' || selectedValues=="()")
			{
				throw new InvalidCommandException("Not a column", 0);
			}
			else
			{
				while (selectedValues != "(")
				{

					//check every column between ()
					int counter11 = 0, counter12 = 0;
					string column;
					if (selectedValues.find(',') != string::npos)
					{
						column = function.extract(selectedValues, selectedValues[1], selectedValues[selectedValues.find_first_of(',')], counter11, counter12);
					}
					else
					{
						column = function.extract(selectedValues, selectedValues[1], selectedValues[selectedValues.find_first_of(')')], counter11, counter12);
					}
					if (function.findChars(column, function.CAPS) == true || function.findChars(column, function.SIGNS) == true || function.findChars(column, " ") == true)
					{
						throw new InvalidCommandException("Not proper column name", 0);
					}
					selectedValues.erase(counter11, counter12);
					selectedValues = function.subStringWithoutSpaces(selectedValues);
				}
				commandName = function.subStringWithoutSpaces(commandName);
			}
		}
		else
		{
			counter1 = counter2 = 0;
			if (function.extract(commandName, commandName[0], commandName[3], counter1, counter2) != "ALL")
			{
				throw new InvalidCommandException("HASN't GOT FROM KEYWORD", 0);
			}
			commandName.erase(0, 3);
			commandName = function.subStringWithoutSpaces(commandName);
		}
		counter1 = counter2 = 0;
		if (function.extract(commandName, commandName[0], commandName[4], counter1, counter2) != "FROM")
		{
			throw new InvalidCommandException("HASN't GOT FROM KEYWORD", 0);
		}
		commandName.erase(0, 4);
		commandName = function.subStringWithoutSpaces(commandName);
		counter1 = counter2 = 0;
		string tableName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		if (function.findChars(tableName, function.CAPS) == true || function.findChars(tableName, function.SIGNS) == true || function.findChars(tableName, " ") == true)
		{
			throw new InvalidCommandException("Not proper table name", 0);
		}
		commandName.erase(counter1, counter2);
		commandName = function.subStringWithoutSpaces(commandName);
		if (commandName != "")
		{
			counter1 = counter2 = 0;
			if (function.extract(commandName, commandName[0], commandName[5], counter1, counter2) != "WHERE")
			{
				throw new InvalidCommandException("HASN't GOT WHERE KEYWORD", 0);
			}
			commandName.erase(counter1, counter2);
			commandName = function.subStringWithoutSpaces(commandName);
			counter1 = counter2 = 0;
			string column = function.extract(commandName, commandName[0], ' ', counter1, counter2);
			if (function.findChars(column, function.CAPS) == true || function.findChars(column, function.SIGNS) == true || function.findChars(column, " ") == true)
			{
				throw new InvalidCommandException("Not proper column name", 0);
			}
			commandName.erase(counter1, counter2);
			commandName = function.subStringWithoutSpaces(commandName);
			counter1 = counter2 = 0;
			if (function.extract(commandName, commandName[0], commandName[1], counter1, counter2) != "=")
			{
				throw new InvalidCommandException("HASN't GOT = SIGN", 0);
			}
			commandName.erase(counter1, counter2);
			commandName = function.subStringWithoutSpaces(commandName);
			counter1 = counter2 = 0;
			if (function.findChars(commandName, function.CAPS) == true || function.findChars(commandName, function.SIGNS) == true || function.findChars(commandName, " ") == true)
			{
				throw new InvalidCommandException("Not proper value name OR multiple conditions", 0);
			}
		}
		cout << "The command is correct. We are now executing it: !";
	}
};

//5. UPDATE
//MERGE PENTRU TOATE EXEMPLELE
class UpdateCommand
{
	Command command;
	UsefulFunctions function;
	fstream theDatabase;
public:
	UpdateCommand(Command command, fstream &theDatabase)
	{
		checkUpdate(command.getName());
		function.copyFile(theDatabase, this->theDatabase);
	}
private:
	void checkUpdate(string commandName)
	{
		//check table name
		int counter1 = 0, counter2 = 0;
		string tableName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		if ((tableName.find("(") != tableName.npos) || (tableName.find(" ") != tableName.npos) || (tableName.find(",") != tableName.npos))
		{
			throw new InvalidCommandException("The UPDATE command has the wrong table name", 0);
		}
		//check SET KEYWORD
		counter1 = 0, counter2 = 0;
		string setKeyword = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		if (setKeyword != "SET")
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the SET keyword", 0);
		}
		//check column name
		counter1 = 0, counter2 = 0;
		string columnName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		if (function.findChars(columnName, function.CAPS) == 1 || function.findChars(columnName, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column name", 0);
		}
		//check =
		counter1 = 0, counter2 = 1;
		if (commandName[0] != '=')
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the EQUAL sign", 0);
		}
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		//check value to be changed
		counter1 = 0, counter2 = 0;
		string columnValue = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		if (function.findChars(columnValue, function.SIGNS) == true)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column value sign", 0);
		}
		//check WHERE
		counter1 = 0, counter2 = 5;
		string WHERE = commandName.substr(0, 5);
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		if (WHERE != "WHERE")
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the WHERE keyword!", 0);
		}
		//check columnName to be changed
		counter1 = 0, counter2 = 0;
		columnName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		if (function.findChars(columnName, function.CAPS) == 1 || function.findChars(columnName, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column name", 0);
		}
		//check equal AGAIN
		counter1 = 0, counter2 = 1;
		if (commandName[0] != '=')
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the EQUAL sign", 0);
		}
		commandName.erase(counter1, counter2);
		commandName=function.subStringWithoutSpaces(commandName);
		//check column value to be updated
		counter1 = 0, counter2 = 0;
		columnValue = function.extract(commandName, commandName[0], commandName[commandName.length()], counter1, counter2);
		commandName.erase(counter1, counter2);
		commandName = function.subStringWithoutSpaces(commandName);
		if (function.findChars(columnValue, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column value sign", 0);
		}
		cout << "The command is correct. We are now executing it: !";
	}
};

//6.DELETE
//MERGE PENTRU TOATE EXEMPLELE
class DeleteCommand {

	Command command;
	UsefulFunctions function;
	fstream theDatabase;
public:
	DeleteCommand(Command command, fstream &theDatabase)
	{
		checkDelete(command.getName());
		function.copyFile(theDatabase, this->theDatabase);
	}
private:
	void checkDelete(string commandName)  // "table_name WHERE column_name = value"
	{
		int counter1 = 0, counter2 = 0;
		//check FROM keyword
		if (function.extract(commandName, commandName[0], ' ', counter1, counter2) != "FROM")
		{
			throw new InvalidCommandException("The DELETE hasn't got the KEYWORD FROM", 0);
		}
		commandName.erase(counter1, counter2);
		commandName = function.subStringWithoutSpaces(commandName);
		//check table name
		counter1 = counter2 = 0;
		string tableName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		{
			if (function.findChars(tableName, function.CAPS) == 1 || function.findChars(tableName, function.SIGNS) == 1)
			{
				throw new InvalidCommandException("The DELETE hasn't got the proper table name", 0);
			}
		}
		commandName.erase(counter1, counter2);
		commandName = function.subStringWithoutSpaces(commandName);
		//set and check keyword WHERE
		counter1 = 0;
		counter2 = 0;
		string keyWord = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(counter1, counter2);
		commandName = function.subStringWithoutSpaces(commandName);
		if (keyWord != "WHERE")
		{
			throw new InvalidCommandException("The DELETE hasn't got the KEYWORD", 0);
		}
		//check column name
		counter1 = 0;
		counter2 = 0;
		string columnName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		if (function.findChars(columnName, function.CAPS) == 1 || function.findChars(columnName, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The DELETE command hasn't got the proper column name", 0);
		}
		commandName.erase(counter1, counter2);
		commandName = function.subStringWithoutSpaces(commandName);
		//check =
		counter1 = 0, counter2 = 1;
		if (commandName[0] != '=')
		{
			throw new InvalidCommandException("The DELETE command hasn't got the EQUAL sign", 0);
		}
		commandName.erase(counter1, counter2);
		commandName = function.subStringWithoutSpaces(commandName);
		//check value to be changed
		string columnValue = commandName;
		if (function.findChars(columnValue, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The DELETE command hasn't got the proper column value sign", 0);
		}
		cout << "The command is correct. We are now executing it: !";
	}
};

//7.INSERT
//7.INSERT
class InsertCommand
{
	Command command;
	UsefulFunctions function;
	fstream theDatabase;
public:
	InsertCommand(Command command, fstream &theDatabase) {
		insertValidation(command.getName());
		function.copyFile(theDatabase, this->theDatabase);
	}
private:
	void insertValidation(string Command) {
		string newCommand = Command;
		string editable;
		string parametriiTabel;
		string copyEditable;

		int noParam = 0;
		int startPoint = 0;
		int numberOfParam = 4; // ATENTIE !! ACEST NUMAR SE VA PRIMI DIN CREATE TABLE la APELARE CUMVA

		//validare paranteze 
		if (function.nrChars(newCommand, '(', noParam) != 1 && function.nrChars(newCommand, ')', noParam) != 1) {
			throw new InvalidCommandException("paranteze gresite ", 0);
		}

		editable = newCommand.substr(0, newCommand.find_first_of('('));
		copyEditable = function.stringWithoutSpaces(editable);

		//aici in parametrii avem numele tabelului: studenti
		parametriiTabel = copyEditable.substr(0, copyEditable.find_first_of('V'));
		if (parametriiTabel.length() == 0)
			throw new InvalidCommandException("nu exista nume tabe ", 0);
		else
			cout << endl << "Numele tabelului in care se vor face modificari este " << parametriiTabel;

		//aici putem face o validare daca exista tabelul sau nu  , nu voi mai face validare pt corectitudinea tabelului pt ca ar trb deja sa fie creat deci sa existe deci ajunge doar o comparatie simpla
		//
		//acum facem o validare lexicografica pt VALUES -

		if (copyEditable.compare(copyEditable.find_first_of('V'), 6, "VALUES") == 0 && copyEditable.length() == (parametriiTabel.length() + 6)) {
			cout << endl << "Validare reusita pana la paranteza";
		}
		else {
			throw new InvalidCommandException("wrong command ", 0);
		}

		newCommand.erase(0, editable.length()); // newCommand devine (1,"John","1001")
		editable = newCommand.substr(newCommand.find_first_not_of('('), newCommand.find_last_of(')') - 1); //editable devine 1,"John","10001"
		editable = function.stringWithoutSpaces(editable);  // se elimina spatiile desi in cerinta reiese ca nu ar exista spatii 
		noParam = function.nrChars(editable, ',', noParam) + 1;  // in functie de nr de virgule primim nr parametrii  no=3

		// AICI MAI ESTE NEVOIE DE O VALIDARE CARE SA VERIFICE DACA NUMARUL COLOANELOR RESPECTA NUMARUL VIRGULELOR + 1 CARE SE VA AFLA IN FUNCTIE DE VALOAREA DIN CREATE A TABELULUI 
		// INSERT INTO studenti VALUES (1,”John” ”1001”)  -- exemplul ruleaza desi e gresit.

		// now we go parameter by parameter
		while (startPoint < noParam) {
			parametriiTabel = editable.substr(0, editable.find_first_of(','));
			int temp = 0;
			if (function.nrChars(parametriiTabel, '"', temp)) {
				string temp = parametriiTabel.substr(1, parametriiTabel.find_last_of('"') - 1);
				cout << endl << "Parametrul tabel la iteratie " << startPoint << " este " << temp << " si este tip string";
			}
			else {
				float value;
				string temporary = parametriiTabel;
				if (function.nrChars(temporary, '.', temp) > 1)
					throw new InvalidCommandException("too many points", 0);
				else if (function.nrChars(temporary, '.', temp) == 1) {
					temporary = temporary.erase(temporary.find_first_of('.'), 1);
				}
				if (function.checkAsciiValue(temporary, '0', '9')) {
					throw new InvalidCommandException("nu este string dar are litere", 0);
				}
				else {
					value = stof(parametriiTabel);
					cout << endl << "Parametrul tabel la iteratie " << startPoint << " este floatul " << value;
				}
			}
			editable.erase(0, parametriiTabel.length() + 1);
			startPoint++;
		}

	}
};



//PARSER-UL
class Parser
{
	Command command;
	UsefulFunctions function;
	string name = command.getName();
	string FirstWord = "";
	string SecondWord = "";
	fstream theDatabase;
public:

	Parser(Command command, string TheDatabase)
	{
		this->FirstWord = command.getFirstWord();
		this->SecondWord = command.getSecondWord();
		theDatabase.open(TheDatabase, ios::in | ios::out);
		//function.copyFile(theDatabase, this->theDatabase);
	}
	void Parse(Command command)
	{
		if (FirstWord == "CREATE")
		{
			CreateCommand object(command, this->theDatabase);
		}
		if (FirstWord == "DROP")
		{
			DropCommand object(command, this->theDatabase);
		}
		if (FirstWord == "DELETE")
		{
			DeleteCommand object(command, this->theDatabase);
		}
		if (FirstWord == "DISPLAY")
		{
			DisplayCommand object(command, this->theDatabase);
		}
		if (FirstWord == "UPDATE")
		{
			UpdateCommand object(command, this->theDatabase);
		}
		if (FirstWord == "SELECT")
		{
			SelectCommand object(command, this->theDatabase);
		}
		if (FirstWord == "INSERT")
		{
			InsertCommand object(command, this->theDatabase);
		}
	}

	friend class DisplayCommand;
};


// The classes for data

class UtilTable {
public:
	static void setTable() {

	}
};


//DECI am introdus enum asta ca sa putem sa salvam type-ul atributului. ca sa nu ma mai bag eu peste codul tau ca pierdem timp sa stau sa 
//inteleg ce ai facut tu acolo si de ce, eu cred ca cea mai buna varianta e sa chemi constructorul asta cu (string) inauntru, nu ala default
//in TableColumn. Practic comenzile noastre o sa scoata un atribut, "Alice", sa zicem, si o sa se cheme constructorul ColumnAttribute("Alice")
//care ii atribuie lui attribute valoarea "Alice" (mrg fara "") si lui type valoarea STRING. 
//ALSO chestia asta putea fi facuta pt toata coloana si nu ar mai fi fost nevoie sa o facem in atribut, pt ca toata coloana e de acelasi type
//ALSO eu cred ca nu are rost sa salvam valoarea in sine a atributului cu tipul care e. Gen nu cred ca are rost sa declaram "int valoare"
//pt ca daca vrem sa comparam doua atribute, attribute1.value="12" si attribute2.value="23" sa zicem, putem sa comparam direct stringurile
//"12" cu "23" si attribute1.type cu attribute2.type (putem sa facem asta intr-un operator ==). 
//Practic daca au acelasi type si aceeasi valoare sunt egale, nu trebuie sa comparam in sine 12 cu 23
//pwp
enum AttributeType { INTEGER, FLOAT, STRING };

class ColumnAttribute {
	string value;
	AttributeType type;
	UsefulFunctions function;
public:

	ColumnAttribute(string Attribute)
	{
		this->value = Attribute;
		if (this->value.find(function.NUMBERS) != string::npos)
		{
			//means the attribute is a number. finding what type of number:
			if (this->value.find(",") != string::npos || this->value.find(".") != string::npos)
			{
				//means it's float
				this->type = FLOAT;
			}
			else
			{
				//means it's an integer
				this->type = INTEGER;
			}
		}
		else
		{
			//means it's a string
			this->type = STRING;
		}
	}

	ColumnAttribute()
	{

	}
	
	void writeAttirbuteToBin(ofstream&File)
	{
		//write the value (string)
		function.writeStringToBin(File, this->value);

		//write the type:
		//here, the type will not appear in the bin as enum (idk how to do this) => it will appear as a string.
		//the reading process will transform the string red from the file to an enum, so the type will never explicitly be other than enum to the user 
		if (this->type == FLOAT)
		{
			function.writeStringToBin(File, "FLOAT");
		}
		if (this->type == INTEGER)
		{
			function.writeStringToBin(File, "INTEGER");
		}
		if (this->type == STRING)
		{
			function.writeStringToBin(File, "STRING");
		}
	}

	void readAttributeFromBin(ifstream& File)
	{
		//reading the value (string)
		function.readStringFromBin(File, this->value);


		//reading the type (enum)
		//we read a string object from the bin file, as this is the way in which it has been written, and we transform it into an enum type
		//we need a buffer in which to read:
		string buffer;
		function.readStringFromBin(File, buffer);
		if (buffer == "FLOAT")
		{
			this->type = FLOAT;
		}
		if (buffer == "INTEGER")
		{
			this->type = INTEGER;
		}
		if (buffer == "STRING")
		{
			this->type = STRING;
		}
	}
};

class TableColumn {
	char columnName[100] = "";
	string columnType = "";
	int dimension = 0;
	string defaultValue = "";
	int noAttributes=0;
	ColumnAttribute* attributes = nullptr;

public:
	TableColumn(const char* name, string type, int dimension, string defValue, int NoAttributes) {
		strcpy_s(this->columnName, 100, name);
		this->columnName[99] = '\0';
		this->columnType = type;
		this->dimension = dimension;
		this->defaultValue = defValue;
//VALI: am adaugat nr de atribute pt ca nu putem sa avem un vector in binar fara sa stim exact dimensiunea (sau cel putin asa a zis Boja)
		//deci, in create o sa avem o functie care numara cate atribute are coloana 
		//aslo, de fiecare data cand facem un insert trebuie sa creasca nr de atribute
		this->noAttributes = NoAttributes;
	}
private:
	TableColumn() {

	}
public:
	~TableColumn() {
		if (attributes != nullptr)
			delete[] attributes;
	}
	TableColumn(const TableColumn& newColumn) {
		strcpy_s(this->columnName, 100, newColumn.columnName);
		this->columnName[99] = '\0';
		this->defaultValue = newColumn.defaultValue;
		this->attributes = new ColumnAttribute[newColumn.dimension];
		for (int i = 0; i < newColumn.dimension; i++) {
			this->attributes[i] = newColumn.attributes[i];
		}
		this->dimension = newColumn.dimension;
	}
	void operator=(const TableColumn& newColumn) {
		if (this != &newColumn) {
			if (this->attributes) {
				delete[] this->attributes;
			}
			strcpy_s(this->columnName, 100, newColumn.columnName);
			this->columnName[99] = '\0';
			this->defaultValue = newColumn.defaultValue;
			this->attributes = new ColumnAttribute[newColumn.dimension];
			for (int i = 0; i < newColumn.dimension; i++) {
				this->attributes[i] = newColumn.attributes[i];
			}
			this->dimension = newColumn.dimension;
		}
	}

	void operator<<(ColumnAttribute attribute) {
		ColumnAttribute* newAttributes = new ColumnAttribute[this->dimension + 1];
		for (int i = 0; i < this->dimension; i++) {
			newAttributes[i] = this->attributes[i];
		}
//		newAttributes[this->dimension] = dimension;  //VALEE aici nuj ce ai vrut sa faci dar imi da eroare pt ca e un int atribuit la un obiect
		this->dimension += 1;
		if (this->attributes) {
			delete[] this->attributes;
		}
		this->attributes = newAttributes;
	}

	void operator+=(ColumnAttribute column) {
		*this << column;
	}

	void writeColumnToBin(ofstream& File)
	{
		//write the name(char):
		File.write(this->columnName, sizeof(TableColumn::columnName) * sizeof(char));

		//write the columnType (aici o sa fac dupa ce rezolvi tu Vali cu constructorul in functie de tipul atributului (vezi mai sus))

		//write the dimension(int):
		File.write((char*)this->dimension, sizeof(int));

		//write the default value (string):
		UsefulFunctions::writeStringToBin(File, this->defaultValue);

		//write the array of attributes (the pointer to it):
		File.write((char*)this->noAttributes, sizeof(int));
		for (int i = 0; i < noAttributes; i++)
		{
			attributes[i].writeAttirbuteToBin(File);
		}
	}

	void readColumnFromBin(ifstream& File)
	{
		//read the name(char):
		File.read(this->columnName, sizeof(TableColumn::columnName) * sizeof(char));

		//read the columnType (aici o sa fac dupa ce rezolvi tu Vali cu constructorul in functie de tipul atributului (vezi mai sus))

		//write the dimension(int):
		File.read((char*)this->dimension, sizeof(int));

		//write the default value (string):
		UsefulFunctions::readStringFromBin(File, this->defaultValue);

		//write the array of attributes (the pointer to it):
		File.read((char*)this->noAttributes, sizeof(int));
		for (int i = 0; i < noAttributes; i++)
		{
			attributes[i].readAttributeFromBin(File);
		}
	}

	friend class Table;
};


class Table {
	char tableName[100] = ""; 
	int noColumns = 0;
	TableColumn* columns = nullptr;

public:
	Table(const char* tableName) {
		//i think here we might some sort of validation ??? 
		strcpy_s(this->tableName, 100, tableName);
	}

	~Table() {
		if (columns != nullptr)
			delete[] columns;
	}

	Table(const Table& table) {
		strcpy_s(this->tableName, 100, table.tableName);
		this->tableName[99] = '\0';
		this->columns = new TableColumn[table.noColumns];
		for (int i = 0; i < table.noColumns; i++) {
			this->columns[i] = table.columns[i];
		}
		this->noColumns = table.noColumns;
	}

	void operator=(const Table& table) {
		if (this != &table) {
			if (this->columns) {
				delete[] this->columns;
			}
			strcpy_s(this->tableName, 100, table.tableName);
			this->tableName[99] = '\0';
			this->columns = new TableColumn[table.noColumns];
			for (int i = 0; i < table.noColumns; i++) {
				this->columns[i] = table.columns[i];
			}
			this->noColumns = table.noColumns;
		}
	}
	void operator<<(TableColumn column) {
		TableColumn* newColumn = new TableColumn[this->noColumns + 1];
		for (int i = 0; i < this->noColumns; i++) {
			newColumn[i] = this->columns[i];
		}
		newColumn[this->noColumns] = column;
		this->noColumns += 1;
		if (this->columns) {
			delete[] this->columns;
		}
		this->columns = newColumn;
	}
	void operator+=(TableColumn column) {
		*this << column;
	}

	void writeTableToBin(string tableName)
	{
		ofstream File(tableName, ios::out | ios::binary | ios::trunc);
		if (File.is_open())
		{
			//write the table name (char):
			File.write(this->tableName, sizeof(Table::tableName) * sizeof(char));

			//write the columns:
			File.write((char*)this->noColumns, sizeof(int));
			for (int i = 0; i < this->noColumns; i++)
			{
				columns[i].writeColumnToBin(File);
			}

			File.close();
		}
		else
		{
			cout << "THE FILE IS NOT OPEN SO YOU CAN'T WRITE A TABLE IN IT! OPEN THE FILE!";
		}
		
	}

	void readTableFromBin(string fileName)
	{
		ifstream File(fileName, ios::in | ios::binary);
		if (File.is_open())
		{
			//write the table name (char):
			File.read(this->tableName, sizeof(Table::tableName) * sizeof(char));

			//write the columns:
			File.read((char*)this->noColumns, sizeof(int));
			for (int i = 0; i < this->noColumns; i++)
			{
				columns[i].readColumnFromBin(File);
			}

			File.close();
		}
		else
		{
			cout << "THE FILE IS NOT OPEN SO YOU CAN'T READ A TABLE FROM IT! OPEN THE FILE!";
		}
	}

	friend class CreateCommand;
};
