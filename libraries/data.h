#include<stdio.h>
#include<iostream>
using namespace std;

#ifndef DATA_H_
#define DATA_H_

// tipos
const string letters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
const string digits = "1234567890";

// metodos para identificar y manipular datos
bool isLetter(char character)
{
	for (int i=0; i<letters.length(); i++)
	{
		if (character == letters[i]) return true;
	}
	
	return false;
}

bool isDigit(char character)
{
	for (int i=0; i<digits.length(); i++)
	{
		if (character == digits[i]) return true;
	}
	
	return false;
}

string lower_case(string input)
{
	string result = "";
	for (int i=0; i<input.length(); i++)
	{
		if (isLetter(input[i]))
		{
			result += tolower(input[i]);	
		}
		else
		{
			result += input[i];
		}
	}
	return result;
}

vector<string> deleteDuplicates(vector<string>& list) 
{
    sort(list.begin(), list.end());
    list.erase(unique(list.begin(), list.end()), list.end());

    return list;
}

#endif