#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string randomName () {
    static const string charList = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    string alphanumeric = "";

    for(int i = 0; i < 10; i++) {
      alphanumeric += charList [rand() % charList.size()];
    }
    return alphanumeric;
}

string randomNumber () {
    static const string charList = "0123456789";

    string alphanumeric = "";

    for(int i = 0; i < 10; i++) {
      alphanumeric += charList [rand() % charList.size()];
    }
    return alphanumeric;
}

void databaseRegister () {
  ofstream myfile;
  myfile.open ("Registros - CPF.txt");

  int i = 0;
  string nome, cpf;
  while (i<1000) {
    nome = randomName();
    cpf = randomNumber();
    myfile << i << " " << nome  << endl << cpf << endl;
    i++;
  }
  myfile.close();
}

bool findCPF (string cpf) {
  bool find = false;
  string linha;

  ifstream myfile;
  myfile.open ("Registros - CPF.txt");

  for (int i = 0; i<2000; i++) {
    getline (myfile, linha, '\n');
    if (linha == cpf) {
      find = true;
      cout  << "CPF válido" << endl << endl;
      break;
    }
  }
  myfile.close();
  return find;
}
