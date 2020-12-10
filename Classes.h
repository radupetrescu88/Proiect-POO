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
};

class Command
{
	string name = "";
	string commandFirst = "";
	string commandSecond = "";
	UsefulFunctions function;
public: //aminteste-ti sa stergi dupa verificare!!!!!!!!!!!!!
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

};
