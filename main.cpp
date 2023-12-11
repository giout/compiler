#include<stdio.h>
#include<iostream>
#include<cstring>
#include<vector>
#include<regex>
#include "./libraries/data.h"
#include "./libraries/file.h"
using namespace std;

// metodos
bool reservedWord(string);
bool identifier(string);
bool number(string);
bool arithmeticOperator(string);
int token(string);
bool lexer(string);
bool parser();
bool semantic();
int getValue(int);

// datos fundamentales del programa
vector<string> lexemes;
vector<string> tokens;

// ejecucion del programa
int main()
{
	string program = fileToString("./files/input.txt");
	
	// Si el analisis lexico tuvo exito, se ejecuta el analisis sintactico
	if (lexer(program))
	{
		if (parser())
		{
			if (semantic())
			{
				
			}
		}
	}
	return 0;
}

// En el analisis lexico, se comprueba que el codigo no contenga ningun token desconocido.
bool lexer(string input)
{		
	string auxiliar = "";

	// los espacios, saltos de linea, asignadores y finalizadores actuan como separadores de tokens. Cada vez que el analizador 
	// se encuentra con uno de estos, analiza el token recopilado hasta ese punto y reinicia la variable auxiliar
	for (int i=0; i<input.length(); i++)
	{
		if (input[i] == ' ')
		{	
			// Se analiza el token, si no existe, se imprime el error y se finaliza el programa
			if (token(auxiliar) == 1)
			{
				print("Error lexico: token inexistente.", "./files/output.txt");
				return false;
			}
			auxiliar = "";
		}
		
		else if (input[i] == '=')
		{
			if (token(auxiliar) == 1)
			{
				print("Error lexico: token inexistente.", "./files/output.txt");
				return false;
			}
			auxiliar = "";
			lexemes.push_back("=");
			tokens.push_back("ASIG");	
		}
		
		else if (input[i] == '.')
		{
			if (token(auxiliar) == 1)
			{
				print("Error lexico: token inexistente.", "./files/output.txt");
				return false;
			}
			auxiliar = "";
			lexemes.push_back(".");	
			tokens.push_back("FINAL");	
		}
		
		else if (input[i] == '(')
		{
			token(auxiliar);
			auxiliar = "";
		}
		
		else if (input[i] == ')')
		{
			token(auxiliar);
			auxiliar = "";
		}
		
		// se agrega cada caracter iterado a la variable auxiliar token para que cuando se encuentr con alguno de los previos simbolos, se pueda analizar
		else auxiliar += input[i];
		
		// se verifica si se ha llegado al final de cadena para analizar el ultimo token
		if (i == input.length() - 1)
		{
			token(auxiliar);
		} 
	}
	
	return true;
}

// analisis general
int token(string input)
{	
	// cuando la variable auxiliar sea "", el analizador omitira el caracter vacio
	if (input == "") return 0;
	
	if (reservedWord(input))
	{
		lexemes.push_back(input);
		tokens.push_back("PR");
		return 0;
	}
	
	if (identifier(input))
	{
		lexemes.push_back(input);
		tokens.push_back("ID");
		return 0;
	}
	
	if (number(input))
	{
		lexemes.push_back(input);
		tokens.push_back("NUM");
		return 0;
	}
	
	if (arithmeticOperator(input))
	{
		lexemes.push_back(input);
		tokens.push_back("OP");
		return 0;
	}
	
	
	return 1;
}

// analisis de cada token
bool reservedWord(string input)
{	
	// se convierte la cadena a minuscula
	string s = lower_case(input);
	
	if (s == "inicio" || s == "fin" || s == "variable")
	{
		return true;
	}
	
	return false;
}

bool identifier(string input)
{	
	// se comprueba que el primer caracter sea letra
	if (isLetter(input[0]))
	{
		for (int i=1; i<input.length(); i++)
		{
			// si algun caracter no es ni letra ni numero, se descarta como identificador valido
			if (!isLetter(input[i]) && !isDigit(input[i]))
			{
				return false;
			}
		}	
		return true;
	}
	return false;
}

bool number(string input)
{
	// si algun caracter no es numero ni coma (separador decimal), se descarta
	for (int i=0; i<input.length(); i++)
	{
		if (!isDigit(input[i]) && input[i] != ',') return false;
	 } 
	 
	 // si existe mas de una coma en el numero, se descarta
	 int count = 0;
	 for (int i=0; i<input.length(); i++)
	 {
	 	if (input[i] == ',') count++;
	 	if (count > 1) return false;
	 }
	 
	 return true;
	 
}

bool arithmeticOperator(string input)
{
	if (input == "+" || input == "-" || input == "^" || input == "/" || input == "*")
	{
		return true;
	}
	
	return false;
}
 
// analisis sintactico
bool parser()
{
	// La lista de tokens se traslada a un string para evaluarla con mas facilidad
	string p = "";
	for (int i=0; i<tokens.size(); i++)
	{
		p += tokens[i] + " ";
	}

	// Se define la expresion regular de acuerdo a la estructura que debe tener el programa.
	regex r("^(PR\\s*(\\s*ID\\s*ASIG\\s*NUM\\s*FINAL)*)*\\s*PR\\s*(\\s*ID\\s*ASIG\\s*(ID|NUM)(\\s*OP\\s*(ID|NUM))*\\s*FINAL)\\s*PR\\s*$");

	bool result = regex_search(p, r);	

	// Si la cadena que representa los tokens en orden del programa cumplen con la expresion regular, quiere decir que no hay errores sintacticos.
	if (result)
	{
		return true;
	}
	else
	{
		print("Error sintactico: estructura de programa u orden de tokens invalido.", "./files/output.txt");
		return false;
	}
}

// El analizador semantico evalua los errores dentro de las expresiones. 
bool semantic()
{
	// Evalua que las palabras reservadas se usen de forma coherente.
	vector<string> reserved; 
	for (int i=0; i<tokens.size(); i++)
	{
		if (tokens[i] == "PR") reserved.push_back(lower_case(lexemes[i]));
	}

	// Se debe cumplir el orden: VARIABLE - INICIO - FIN, siendo variable una palabra opcional.
	if (reserved.size() == 2)
	{
		if (!(reserved[0]=="inicio" && reserved[1]=="fin"))
		{
			print("Error semantico: uso invalido de palabras reservadas.", "./files/output.txt");
			return false;
		}
	}
	else if (reserved.size() == 3)
	{
		if (!(reserved[0]=="variable" && reserved[1]=="inicio" && reserved[2]=="fin"))
		{
			print("Error semantico: uso invalido de palabras reservadas.", "./files/output.txt");
			return false;
		}
	}
	
	// Evalua que todos los identificadores usados en las expresiones esten definidos con un valor coherente. 
	vector<string> defined;
	for (int i=0; i<tokens.size()-1; i++)
	{
		// Si se detecta una asignacion (es decir, si se detecta un token id y consiguiente a ese, un token asignacion), se agrega el valor del id a la lista
		if (tokens[i]=="ID")
		{
			if (tokens[i+1]=="ASIG") defined.push_back(lexemes[i]);
		}
	}

	// Ahora se recopilan absolutamente todos los id encontrados y se verifica que estos se encuentren dentro de la lista de ids definidos.
	vector<string> id;
	for (int i=0; i<tokens.size(); i++)
	{
		if (tokens[i]=="ID")
		{
			id.push_back(lexemes[i]); 
		} 
	}

	id = deleteDuplicates(id);

	// Si no existe el mismo numero de identificadores definidos que de identificadores en general, quiere decir que hay un identificador utilizado cuyo valor no ha sido definido.
	if (defined.size() != id.size())
	{
		print("Error semantico: identificador con valor no definido.", "./files/output.txt");
		return false;
	}

	// Evalua que no se divida un valor entre cero.
	for (int i=0; i<tokens.size()-1; i++)
	{
		if (lexemes[i]=="/")
		{
			if(lexemes[i+1]=="0")
			{
				print("Error semantico: division entre cero.", "./files/output.txt");
				return false;
			}
		}
	}
	
	// Se obtiene el resultado.

	// Se itera a traves de los lexemas hasta encontrar la palabra reservada "INICIO".
	int index; // Se guarda el indice aqui.
	for (int i=0; i<lexemes.size(); i++)
	{
		if (lower_case(lexemes[i]) == "inicio")
		{
			index = i;
		}
	}

	// Ahora se leera la operacion y se definira el resultado.
	index+=3; // Se le suman 3 a index para saltarnos el id y el asignador e ir directo al primer elemento operacion.


	// El valor inicial del resultado es el primer elemento que se encuentra dentro de la operacion, al cual se le procesaran los demas a traves de los operadores aritmeticos.
	int result = getValue(index);

	index++; // Se incrementa de nuevo para continuar.

	while(tokens[index] != "FINAL")
	{
		if (lexemes[index] == "+")
		{
			result += getValue(index+1);
		}
		else if (lexemes[index] == "-")
		{
			result -= getValue(index+1);
		}
		else if (lexemes[index] == "*")
		{
			result *= getValue(index+1);
		}
		else if (lexemes[index] == "/")
		{
			result /= getValue(index+1);
		}

		index += 2;
	}

	// Se imprime el resultado.
	char r[255];
	sprintf(r, "%d", result);
	print(r, "./files/output.txt");

	return true;
}

int getValue(int index)
{
	if (tokens[index] == "ID")
	{
		// Se busca en el programa cual es el valor del identificar.
		for (int i=0; i<tokens.size(); i++)
		{
			if (lexemes[i] == lexemes[index])
			{
				if (tokens[i+1] == "ASIG")
				{
					// Se obtiene finalmente el numero y se convierte a float.
					return atoi(lexemes[i+2].c_str());
				}
			}
		}	
	}
	else if (tokens[index] == "NUM")
	{
		return atoi(lexemes[index].c_str());
	}
}