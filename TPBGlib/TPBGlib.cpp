


#include "TPBGlib.h"

using namespace std;

ifstream fin;//for load passwords in file
ofstream fout;//for save passwords in file

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
		
		//when i<j
		if (k < 0)k += spisokLeng;

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
//#pragma warning (disable:4244)		
		srand(time(0));//in <ctime>
//#pragma warning (once:4244)
		for (int i = 0; i < lengPassword; i++) {
			*(password+i)=spisok[(rand()*rand())%spisokLeng];
		}

		return password;
	}


	bool isSavedPasswords(char** passwords) {

		//add text in the file end/���������� � ����� �����
		fout.open(nameFile, ios_base::app);

		//if file is open/���� ���� ������
		if (fout.is_open()) {
			int i = 0;//number of passwords
			while (*(passwords+i) != '\0') {
				int j = 0;//number of symbols in password
				while (*(*(passwords + i) + j) != '\0') {
					fout.put(*(*(passwords + i) + j));
					j++;
				}
				fout.put(' ');
				i++;
			}
			
			fout.close();
			if (!fout.is_open()) return true;
		};
		return false;
	};

	char** loadPasswords() {
		fin.open(nameFile);

		//if file is open/���� ���� ������
		if (fin.is_open()) {
			size_t stepnumber = 10;
			size_t stepsymbol = 10;
			size_t maxnumberpasswords = stepnumber;
			size_t currentpassword = 0;
			char** passwords = (char**)malloc(maxnumberpasswords+1);
			bool isEnd = false;


			while (!isEnd) {
				/*increase the available memory*/
				if (currentpassword+1 == maxnumberpasswords) {
					maxnumberpasswords += stepnumber;
					passwords = (char**)realloc(passwords,maxnumberpasswords+1);
				}

				size_t maxnumbersymbols = stepsymbol;
				size_t currentsymbol = 0;
				*(passwords+currentpassword) = (char*)malloc(maxnumbersymbols + 1);

				char bufsym;
				fin.get(bufsym);
				if (bufsym != EOF) {
					while (bufsym != ' ') {
						/*increase the available memory*/
						if (currentsymbol + 1 == maxnumbersymbols) {
							maxnumbersymbols += stepsymbol;

							*(passwords + currentpassword) =
								(char*)realloc(
									*(passwords + currentpassword),
									maxnumbersymbols + 1);
						}

						*(*(passwords + currentpassword) + currentsymbol) = bufsym;
						currentsymbol++;

						fin.get(bufsym);//end while if bufsym==' '
					}
					currentpassword++;
				}
				else {
					*(passwords + currentpassword) = '\0';
					isEnd = true;
				}
			}

			fin.close();
			if (!fin.is_open()) return passwords;
		};
		return 0;
	};






}