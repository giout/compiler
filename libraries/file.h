#include<stdio.h>
#include<iostream>
#include<algorithm>
using namespace std;

#ifndef FILE_H_
#define FILE_H_

// Imprimir en archivo
void print(char output[255], char path[255])
{
	FILE * file = fopen(path, "wt");
	fprintf(file, output);
	fclose(file);
}

// Convertir archivo de programa en cadena
string fileToString(char path[255])
{
	// se lee el archivo
	FILE * code = fopen(path, "r");
	string program;
	char line[255];
	
	// se pasan los datos del archivo a una cadena
	while (fgets(line, sizeof(line),code))
	{
		program += line;
		program += " ";
	}
	
	fclose(code);
	
	// se eliminan de la cadena los saltos de linea y los tabs
	program.erase(remove(program.begin(), program.end(), '\n'), program.end());
	program.erase(remove(program.begin(), program.end(), '\t'), program.end());
	
	return program;
}

#endif