#pragma once
#include <iostream>
#include <string>
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
	string extract(string str, char a, char b, int& counter1, int& counter2) {
		//VALI: avand in vedere ca nu avem deloc nevoie sa salvam spatiile dintre cuvinte eu zic sa adaugi in functie o chestie care sterge toate 
		//spatiile pana la urmatorul cuvand, dar doar daca b-ul e spatiu (nuj parerea mea, ma gandesc ca e mai simplu sa facem asta direct in 
		//functie decat sa o faci pt fiecare comanda (CREATE, INSERT, etc)
		string subString;
		int i = 0;
		counter1 = -1;
		counter2 = 0;
		while (i < str.length()) {
			if (str[i] == a && str[i + 1] != a)
			{
				int j;
				if (i == 0) {
					counter1 = 0;
					j = i;
				}
				else {
					counter1 = i + 1;
					j = i + 1;
				}
				while (str[j] != b)
				{
					counter2++;
					j++;
					//VALI: aici cred ca trebuie sa pui o conditie in care sa verifici daca ajungi la sfarsitul string-ului si nu se indeplineste conditia
					//adica daca stringul e CREATETABLE si faci extract de la string[0] pana la 'spatiu' cred o sa ai o bucla infinta ca str[j] nu o sa fie niciodata egal cu 'spatiu'
				}
			}
			if (counter1 != -1 && counter2 != 0)
				i = str.length();
			i++;
		}
		subString = str.substr(counter1, counter2);
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
		string result;
		while (str[i] == ' ')
		{
			i++;
		}
		result = str.erase(0, i);
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
void readCommand(ifstream& file)
{
	char buffer[100];
	//aici depinde mult de cum vrea el sa citeasca comanda, daca da size-ul comenzii etc deci inca nu putem sa il facem? cred
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

//readin a command from the keyboard
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
	UsefulFunctions function;
public:
	DropCommand(Command command)
	{
		checkDrop(command.getName());

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
			cout << "DONE";
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
public:

	DisplayCommand(Command command)
	{
		checkDisplay(command.getName());
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
			cout << "DONE";
		}
	}
};

//3.CREATE
//NU MERGE PENTRU IF NOT EXIST
class CreateCommand
{
	Command command;
	UsefulFunctions function;
public:
	CreateCommand(Command command)
	{
		ValidareSerioasaCreate(command.getName());
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
			// here i dont know for sure what checks needs to be done but the passing to the cValue its made 
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
					cout << "DONE!";
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
					cout << endl << "DONE!";
				}
			}

		}
	}
};


//4. SELECT
class SelectCommand
{
	Command command;
	UsefulFunctions function;
public:
	SelectCommand(Command command)
	{
		checkSelect(command.getName());
	}
private:
	void checkSelect(string commandName)
	{
		int counter1 = 0, counter2 = 0;
		commandName = function.subStringWithoutSpaces(commandName);
		if (commandName[0] != 'A')
		{
			string selectedValues = function.extract(commandName, commandName[0], commandName[commandName.find_last_of(')')], counter1, counter2);

			//check the selected values
			if (selectedValues[0] != '(' || selectedValues[selectedValues.length()] != ')')
			{
				throw new InvalidCommandException("Not a column", 0);
			}
			else
			{
				int i = 0;
				while (i<noColumns)
				int counter11 = 0, counter12 = 0;
				string column = function.extract(selectedValues, selectedValues[1], commandName[commandName.find_first_of(',')], counter11, counter12);
				if (function.findChars(column, function.CAPS) == true || function.findChars(column, function.SIGNS) == true || function.findChars(column, " ") == true)
				{
					throw new InvalidCommandException("Not proper column name", 0);
				}
			}
		}
	}
	//{
	//	int counter1 = 0, counter2 = 0;
	//	if (commandName[0] != 'A')
	//	{
	//		string columnList = function.extract(commandName, '(', ')', counter1, counter2);
	//		int size = columnList.length();
	//		columnList.erase(0, 1);
	//		int i = 0;
	//		int j = noValues(columnList);
	//		string* value = new string[1000];
	//		while (i < j)
	//		{
	//			counter1 = 0; counter2 = 0;
	//			value[i] = function.extract(columnList, columnList[0], ',', counter1, counter2);
	//			if (function.findChars(value[i], function.CAPS) == 1 || function.findChars(value[i], function.SIGNS) == 1)
	//			{
	//				throw new InvalidCommandException("The SELECT command hasn't got the proper column values", 0);
	//			}
	//			columnList.erase(0, value[i].length() + 1);
	//			i++;
	//		}
	//		value[i] = columnList;
	//		if (function.findChars(value[i], function.CAPS) == 1 || function.findChars(value[i], function.SIGNS) == 1 || i == 0)
	//		{
	//			throw new InvalidCommandException("The SELECT command hasn't got the proper column/ALL values", 0);
	//		}
	//		commandName.erase(0, size + 2);
	//		delete[]value;
	//	}
	//	else
	//	{
	//		if (function.extract(commandName, commandName[0], ' ', counter1, counter2) != "ALL")
	//		{
	//			throw new InvalidCommandException("The SELECT command hasn't got the proper column/ALL values", 0);
	//		}
	//		else commandName.erase(0, 4);
	//	}
	//	if (function.extract(commandName, commandName[0], ' ', counter1, counter2) != "FROM")
	//	{
	//		throw new InvalidCommandException("The SELECT command hasn't got the proper FROM keyword", 0);
	//	}
	//	commandName.erase(0, 5);
	//	counter1 = 0, counter2 = 0;
	//	if (function.findChars(commandName, "WHERE") == 1)
	//	{
	//		string tableName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
	//		commandName.erase(0, tableName.length() + 1);
	//		if (function.findChars(tableName, function.CAPS) == 1 || function.findChars(tableName, function.SIGNS) == 1)
	//		{
	//			throw new InvalidCommandException("The SELECT command has the wrong table name", 0);
	//		}
	//		//check WHERE
	//		counter1 = 0, counter2 = 0;
	//		string WHERE = commandName.substr(0, 5);
	//		commandName.erase(0, 6);
	//		if (WHERE != "WHERE")
	//		{
	//			throw new InvalidCommandException("The UPDATE command hasn't got the WHERE keyword!", 0);
	//		}
	//		//check columnName to be changed
	//		counter1 = 0, counter2 = 0;
	//		string columnName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
	//		commandName.erase(0, columnName.length() + 1);
	//		if (function.findChars(columnName, function.CAPS) == 1 || function.findChars(columnName, function.SIGNS) == 1)
	//		{
	//			throw new InvalidCommandException("The UPDATE command hasn't got the proper column name", 0);
	//		}
	//		//check equal AGAIN
	//		counter1 = 0, counter2 = 0;
	//		if (commandName[0] != '=')
	//		{
	//			throw new InvalidCommandException("The UPDATE command hasn't got the EQUAL sign", 0);
	//		}
	//		commandName.erase(0, 2);
	//		//check column value to be updated
	//		counter1 = 0, counter2 = 0;
	//		string columnValue = function.extract(commandName, commandName[0], commandName[commandName.length()], counter1, counter2);
	//		commandName.erase(0, columnValue.length());
	//		if (function.findChars(columnValue, function.SIGNS) == 1)
	//		{
	//			throw new InvalidCommandException("The UPDATE command hasn't got the proper column value sign", 0);
	//		}
	//	}
	//	else
	//	{
	//		string tableName = function.extract(commandName, commandName[0], commandName[commandName.length()], counter1, counter2);
	//		commandName.erase(0, tableName.length());
	//		if (function.findChars(tableName, function.CAPS) == 1 || function.findChars(tableName, function.SIGNS) == 1)
	//		{
	//			throw new InvalidCommandException("The SELECT command has the wrong table name", 0);
	//		}
	//	}
	//	if (commandName == "")
	//		cout << "DONE!";
	//}
	int noValues(string Column)
	{
		int noValues = 0;
		int i = 0;
		while (i < Column.length())
		{
			if (Column[i] == ',')
				noValues++;
			i++;
		}
		return noValues;
	}
};

//5. UPDATE
class UpdateCommand
{
	Command command;
	UsefulFunctions function;
public:
	UpdateCommand(Command command)
	{
		checkUpdate(command.getName());
	}
private:
	void checkUpdate(string commandName)
	{
		//check table name
		int counter1 = 0, counter2 = 0;
		string tableName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(0, tableName.length() + 1);
		if ((tableName.find("(") != tableName.npos) || (tableName.find(" ") != tableName.npos) || (tableName.find(",") != tableName.npos))
		{
			throw new InvalidCommandException("The UPDATE command has the wrong table name", 0);
		}
		//check SET KEYWORD
		counter1 = 0, counter2 = 0;
		string setKeyword = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(0, setKeyword.length() + 1);
		if (setKeyword != "SET")
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the SET keyword", 0);
		}
		//check column name
		counter1 = 0, counter2 = 0;
		string columnName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(0, columnName.length() + 1);
		if (function.findChars(columnName, function.CAPS) == 1 || function.findChars(columnName, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column name", 0);
		}
		//check =
		counter1 = 0, counter2 = 0;
		if (commandName[0] != '=')
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the EQUAL sign", 0);
		}
		//check value to be changed
		counter1 = 0, counter2 = 0;
		commandName.erase(0, 2);
		string columnValue = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(0, columnValue.length() + 1);
		if (function.findChars(columnValue, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column value sign", 0);
		}
		//check WHERE
		counter1 = 0, counter2 = 0;
		string WHERE = commandName.substr(0, 5);
		commandName.erase(0, 6);
		if (WHERE != "WHERE")
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the WHERE keyword!", 0);
		}
		//check columnName to be changed
		counter1 = 0, counter2 = 0;
		columnName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(0, columnName.length() + 1);
		if (function.findChars(columnName, function.CAPS) == 1 || function.findChars(columnName, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column name", 0);
		}
		//check equal AGAIN
		counter1 = 0, counter2 = 0;
		if (commandName[0] != '=')
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the EQUAL sign", 0);
		}
		commandName.erase(0, 2);
		//check column value to be updated
		counter1 = 0, counter2 = 0;
		columnValue = function.extract(commandName, commandName[0], commandName[commandName.length()], counter1, counter2);
		commandName.erase(0, columnValue.length());
		if (function.findChars(columnValue, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The UPDATE command hasn't got the proper column value sign", 0);
		}
		cout << "DONE";
	}
};

//6.DELETE
class DeleteCommand {

	Command command;
	UsefulFunctions function;
public:
	DeleteCommand(Command command)
	{
		checkDelete(command.getName());
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
		commandName.erase(0, 5);
		//check table name
		string tableName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		{
			if (function.findChars(tableName, function.CAPS) == 1 || function.findChars(tableName, function.SIGNS) == 1)
			{
				throw new InvalidCommandException("The DELETE hasn't got the proper table name", 0);
			}
		}
		commandName.erase(0, tableName.length() + 1);
		//set and check keyword WHERE
		counter1 = 0;
		counter2 = 0;
		string keyWord = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(0, keyWord.length() + 1);
		if (keyWord != "WHERE")
		{
			throw new InvalidCommandException("The DELETE hasn't got the KEYWORD", 0);
		}
		//check column name
		counter1 = 0;
		counter2 = 0;
		string columnName = function.extract(commandName, commandName[0], ' ', counter1, counter2);
		commandName.erase(0, columnName.length() + 1);
		if (function.findChars(columnName, function.CAPS) == 1 || function.findChars(columnName, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The DELETE command hasn't got the proper column name", 0);
		}
		//check =
		counter1 = 0, counter2 = 0;
		if (commandName[0] != '=')
		{
			throw new InvalidCommandException("The DELETE command hasn't got the EQUAL sign", 0);
		}
		commandName.erase(0, 2);
		//check value to be changed
		string columnValue = commandName;
		if (function.findChars(columnValue, function.SIGNS) == 1)
		{
			throw new InvalidCommandException("The DELETE command hasn't got the proper column value sign", 0);
		}
		cout << "DONE!";
	}
};

//7.INSERT
class InsertCommand
{
	//o fac eu pe asta
};


//PARSER-UL
class Parser
{
	Command command;
//	UsefulFunctions function;
	string name = command.getName();
	string FirstWord = "";
	string SecondWord = "";
public:

	Parser(Command command)
	{
		this->FirstWord = command.getFirstWord();
		this->SecondWord = command.getSecondWord();
	}
	void Parse(Command command)
	{
		if (FirstWord == "CREATE")
		{
			CreateCommand object(command);
		}
		if (FirstWord == "DROP")
		{
			DropCommand object(command);
		}
		if (FirstWord == "DELETE")
		{
			DeleteCommand object(command);
		}
		if (FirstWord == "DISPLAY")
		{
			DisplayCommand object(command);
		}
		if (FirstWord == "UPDATE")
		{
			UpdateCommand object(command);
		}
		if (FirstWord == "SELECT")
		{
			SelectCommand object(command);
		}
	}

	friend class DisplayCommand;
};


