// MathFuncsLib.cpp
// compile with: cl /c /EHsc MathFuncsLib.cpp
// post-build command: lib MathFuncsLib.obj

#include "TPBGlib.h"
#include <ctime> //for srand(time(0)) in generatePassword()
#include <fstream>
#include <stdexcept>

using namespace std;

ifstream fin;//for load passwords in file
ofstream fout;//for save passwords in file
static char nameFile[] = "logs.tpbg";

namespace TPBG
{
	__int64 countLeng(char* ST) {
		char* UM = ST;
		while (*UM != '\0') ++UM;
		_int64 n = (UM - ST);
		return n;
	}

	char findSimbolVigener(char symbolText, char symbolKey, bool isEncode) {
		int i = 0;
		int j = 0;
		int k = 0;

		while (symbolText != *(spisok + i))i++;
		while (symbolKey != *(spisok + j))j++;

		if (isEncode) k = (i + j) % spisokLeng;

		else k = (i - j) % spisokLeng;

		return spisok[k];
	}


	char* encoderVigener(char* textPlain, //Plain text/��������������� �����
		char* key) { //key/����

		_int64 textLeng = countLeng(textPlain);//text's length/����� ������  
		_int64 keyLeng = countLeng(key);//key's length/����� �����
		char* textChiper = (char*)malloc(textLeng + 1);// encrypted text/������������� �����
		char* keykey = (char*)malloc(textLeng + 1); //extended key/����������� ����

		/*length(keykey)==textLeng/����� ����� ����� ����� ���������*/
		for (__int64 i = 0; i < textLeng; i++) {
			*(keykey + i) = *(key + i%keyLeng);
		}

		for (__int64 i = 0; i < textLeng; i++) {
			*(textChiper + i) = findSimbolVigener(*(textPlain + i), *(keykey + i), 1);
		}

		free(keykey);//clear memory
		keykey = 0; //NULL pointer

		*(textChiper + textLeng) = '\0';
		return textChiper;// return pointer on encrypted text
	}


	char* decoderVigener(char* textChiper, //encrypted text/������������� �����
		char* key) {

		_int64 textLeng = countLeng(textChiper);//text's length/����� ������  
		_int64 keyLeng = countLeng(key);//key's length/����� �����
		char* textPlain = (char*)malloc(textLeng + 1);// encrypted text/������������� �����
		char* keykey = (char*)malloc(textLeng + 1); //extended key/����������� ����

		/*length(keykey)==textLeng/����� ����� ����� ����� ���������*/
		for (__int64 i = 0; i < textLeng; i++) {
			*(keykey + i) = *(key + i%keyLeng);
		}

		for (__int64 i = 0; i < textLeng; i++) {
			*(textPlain + i) = findSimbolVigener(*(textChiper + i), *(keykey + i), 0);
		}

		free(keykey);//clear memory
		keykey = 0; //NULL pointer


		*(textPlain + textLeng) = '\0';
		return textPlain;//return pointer on decrypted text
	}

	char* encoderCaesar(char* textPlain, //Plain text
		int key) {

		int turnKey = key % 26;
		_int64 textLeng = countLeng(textPlain);//text's length
		char* textChiper = (char*)malloc(textLeng + 1);// memory for  encrypted text

		for (unsigned int i = 0; i < textLeng; i++) {
			*(textChiper + i) = *(textPlain + i) + turnKey;
			if (*(textChiper + i) > 'Z') {
				*(textChiper + i) -= 26;
			}
		}
		*(textChiper + textLeng) = '\0';
		return textChiper;// return pointer on encrypted text
	}

	char* decoderCaesar(char* textChiper, //encrypted text
		int key) {

		int turnKey = key % 26;
		_int64 textLeng = countLeng(textChiper);//text's length
		char* textPlain = (char*)malloc(textLeng + 1);// memory for decrypted text

		for (unsigned int i = 0; i < textLeng; i++) {
			*(textPlain + i) = *(textChiper + i) - turnKey;
			if (*(textPlain + i) < 'A') {
				*(textPlain + i) += 26;
			}
		}

		*(textPlain + textLeng) = '\0';
		return textPlain;//return pointer on decrypted text
	}


	char* generatePassword(__int64 lengPassword) {

		char* password = (char*)malloc(lengPassword + 1);
		*(password + lengPassword) = '\0';
		
		srand(time(0));//in <ctime>

		for (int i = 0; i < lengPassword; i++) {
			*(password+i)=spisok[rand()%spisokLeng];
		}

		return password;
	}

	/*
	UK save password in file
	RU ���������� ������� � ���� 
	*/
	int savePasswords(char** passwords) {

		//add text in the file end/���������� � ����� �����
		fout.open(nameFile, ios_base::app);

		if (fout.is_open()) {//���� ���� ������
			int i = 0;//number of passwords
			while (*(passwords+i) != '\0') {
				int j = 0;//number of symbols in passwords
				while (*(*(passwords + i) + j) != '\0') {
					fout << *(*(passwords + i)+j);
					j++;
				}
				fout << ' ';
				i++;
			}
			

			fout.close();
			if (!fout.is_open) return 0;
			else return 2;
		}
		else return 1;
	};

	/*
	UK load passwords from file
	RU �������� ������� �� ����� 
	*/
	char** loadPasswords() {
		
		char** inStr;

		fin.open(nameFile);
		if (fin.is_open()) {//���� ���� ������
			
			/*while (!fin.eof()) {
				fin >> **inStr;
			};*/

			fin.close();
			if (!fin.is_open()) return inStr;
			else return 0;
		}
		else return 0;
	};






}