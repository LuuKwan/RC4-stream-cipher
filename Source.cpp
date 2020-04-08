#include<iostream>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

void S_permutation(int s[], string key = "", int size = 256);
vector<int> Key_Stream_generator(string Ptext, int s[], int size = 256);
void dec2hex(int dec, string &cp);
bool keyCheck(const string &secretKey);
int main() {
	ifstream inData;
	ofstream outData;

	inData.open("plaintext.txt");
	outData.open("cipher.txt");
	if (!inData) {
		cout << "cant find";
		return 1;
	}

	string plainText;
	getline(inData, plainText);
	string secretKey{""};
	bool isValid = true;
	do
	{
		cout << "Enter secret key with the length of 5 to 32 characters long (whitespace is not accepted): ";
		getline(cin, secretKey);
		isValid = keyCheck(secretKey);
	} while (!isValid);

	int S[256];
	S_permutation(S, secretKey);
	vector<int> Key_stream = Key_Stream_generator(plainText, S);

	/*plaintext XOR key stream to have ciphertext*/
	string cipherText{ "" };
	string outTex{ "" };
	for (int i{ 0 }; i < plainText.length(); ++i) {
		int decimal;
		string hex1{ "" };
		decimal = plainText.at(i) ^ Key_stream.at(i);
		dec2hex(decimal, hex1);
		// ascii use 1byte to for 1 character, if function dec2hex return 4bit, need to add 0 in the pre-fix of the return value
		if (hex1.length() < 2) {
			if (hex1.length() == 0) {
				hex1 = "00";
			}
			else {
				string temp = hex1;
				hex1 = { "0" };
				hex1 += temp;
				
			}
		}
		cipherText += (hex1+ " ");
		outTex += (hex1);
	}
	cout << "The Secretkey (5 byte) is: " << secretKey << endl;
	cout << cipherText << endl;
	outData << "The Secretkey (5 byte) is: " << secretKey << endl;
	outData << "Ciphertext in hex form is:" << endl;
	outData << outTex;
	system("pause");
	inData.close();
	outData.close();
	return 0;
}


void S_permutation(int s[], string key, int size) {
	string T;
	// initialize S vector and Temporary T vector
	for (int i{ 0 }; i < size; ++i) {
		s[i] = i;
	}
	// Use T vector to initial permutation S
	int j = 0;
	for (int i{ 0 }; i < size; ++i) {
		j = (j + s[i] + key.at(i % key.length())) % 256;
		swap(s[i], s[j]);
	}

}
vector<int> Key_Stream_generator(string Ptext, int s[], int size) {
	int i{ 0 }, j{ 0 };
	vector<int> key;
	int value;
	int iteration = Ptext.length() + 3072;
	for (int counter{ 0 }; counter < iteration; ++ counter){
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		swap(s[i], s[j]);
		value = s[((s[i] + s[j]) % 256)];
		// discard the 1st 3072 byte of keystream
		if(counter >3071)
		 key.push_back(value);
	}
	return key;
}
void dec2hex(int dec, string &cp) {
	int r;
	if (dec > 0) {
		dec2hex(dec / 16, cp);
		string temp;
		r = dec % 16;
		char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
		temp = hex[r];
		cp += temp;
	}
}
bool keyCheck(const string &secretKey) {
	int length = secretKey.length();
	if (length >= 5 && length <= 32) {
		for (int i{ 0 }; i < length; ++i) {
			if (secretKey.at(i) == 32) {
				cout << "Secret key can not include whitespace! Please enter secret key again" << endl;
				 return false;
			}
				
		}
		return true;
	}
	else {
		cout << "Secret key need to be from 5 to 32 characters long! Please enter secret key again" << endl;
		return false;
	}
		
}