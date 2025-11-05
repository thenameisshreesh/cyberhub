#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Caesar Cipher (for text only)
string caesarEncrypt(string text, int shift) {
    string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            result += (c - base + shift) % 26 + base;
        } else {
            result += c;
        }
    }
    return result;
}

string caesarDecrypt(string text, int shift) {
    return caesarEncrypt(text, 26 - (shift % 26));
}

// XOR Cipher (binary safe)
string xorEncryptDecrypt(const string &data, const string &key) {
    string output = data;
    for (size_t i = 0; i < data.size(); ++i) {
        output[i] = data[i] ^ key[i % key.size()];
    }
    return output;
}

// File to string
string readFile(const string &filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "❌ Error opening file: " << filename << endl;
        return "";
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

// String to file
void writeFile(const string &filename, const string &data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "❌ Error writing file: " << filename << endl;
        return;
    }
    file.write(data.c_str(), data.size());
    file.close();
}

int main() {
    cout << "🧠 Smart Encryptor (C++ Version)\n";
    cout << "Choose Mode (1 = Encrypt, 2 = Decrypt): ";
    int mode;
    cin >> mode;

    cout << "Choose Method (1 = Caesar, 2 = XOR): ";
    int method;
    cin >> method;

    cout << "Enter Key (for Caesar = number, for XOR = string): ";
    string key;
    cin >> key;

    cout << "Enter input filename: ";
    string infile;
    cin >> infile;

    string data = readFile(infile);
    if (data.empty()) return 1;

    string output;
    if (method == 1) {
        int shift = stoi(key);
        if (mode == 1)
            output = caesarEncrypt(data, shift);
        else
            output = caesarDecrypt(data, shift);
    } else {
        output = xorEncryptDecrypt(data, key);
    }

    string outfile = (mode == 1) ? "encrypted.dat" : "decrypted_output.dat";
    writeFile(outfile, output);

    cout << "✅ Done! Output saved as " << outfile << endl;
    return 0;
}
