#include<iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;


void S_permutation(int s[], string key = "", int size = 256);
vector<int> Key_Stream_generator(string Ptext, int s[], int size = 256);
int hex2dec(const string &hex);
int pow(int a, int b);
bool keyCheck(const string &secretKey);
int main() {
	ifstream inData;
	ofstream outData;

	inData.open("ciphertext.txt");
	outData.open("plaintex.txt");
	if (!inData) {
		cout << "cant find";
		return 1;
	}

	string cipherText;
	getline(inData, cipherText);
	string secretKey{ "" };
	bool isValid = true;
	do
	{
		cout << "Enter secret key with the length of 5 to 32 characters long (whitespace is not accepted): ";
		getline(cin, secretKey);
		isValid = keyCheck(secretKey);
	} while (!isValid);
	outData << "Decryption with secrete key: " << secretKey << endl;

	int S[256];
	S_permutation(S, secretKey);
	vector<int> Key_stream = Key_Stream_generator(cipherText, S);

	/*plaintext XOR key stream to have ciphertext*/
	outData << "Plain-text in decimal form: " << endl;
	cout << "Plain-text in decimal form:	" << endl;
	string plaintext{ "" };
	
	for (unsigned i{ 0 }; i < cipherText.length(); i= i+2) {
		// break cipherText into substring of 2
		// convert the substring to dec
		// xor the dec-substring with key-stream
		// convert the result into string value;
		string sub;
		static int Keyindex{ 0 };
		sub = cipherText.substr(i,2);
		int decimal = hex2dec(sub);
		decimal = decimal ^ Key_stream.at(Keyindex);
		sub = decimal;
		plaintext += sub;

		outData << decimal << " ";
		cout << decimal << " ";
		++Keyindex;
	}
	outData << endl;
	cout << endl;
	cout << "Plain-text in alphabet form:" << endl;
	cout << plaintext << endl;
	
	outData << "Plain-text in alphabet form: " << endl;
	outData << plaintext << endl;
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
		T += key.at(i % key.length());
	}

	// Use T vector to initial permutation S
	int j = 0;
	for (int i{ 0 }; i < size; ++i) {
		j = (j + s[i] + T.at(i)) % 256;
		swap(s[i], s[j]);
	}

}
vector<int> Key_Stream_generator(string Ptext, int s[], int size) {
	int i{ 0 }, j{ 0 };
	vector<int> key;
	int value;
	int iteration = Ptext.size() + 3072;
	for (int counter{ 0 }; counter < iteration; ++counter) {
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		swap(s[i], s[j]);
		value = s[((s[i] + s[j]) % 256)];
		if (counter > 3071) // discard the 1st 3072 byte in the keystream
			key.push_back(value);
	}
	return key;
}
int pow(int a, int b) {
	if (b == 0) {
		return 1;
	}
	int result{ a };
	for (int i{ 1 }; i < b; ++i) {
		result *= a;
	}
	return result;
}
int hex2dec(const string &hex) {
	int dec{ 0 };
	int p = hex.length() - 1;
	for (unsigned i{ 0 }; i < hex.length(); ++i) {
		
		if (hex.at(i) >= 65 && hex.at(i) <= 70) {
			dec = dec + ((hex.at(i) - 55) * pow(16,p));
		}
		else if (hex.at(i) >= 48 && hex.at(i) <= 57) {
			dec = dec + ((hex.at(i) - 48) * pow(16, p));
		}
		--p;
	}
	return dec;
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
