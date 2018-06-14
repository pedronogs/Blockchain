// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdint.h>
#include "Database.h"
#define PORT 8080
using namespace std;

int createConnection(const char *voto, const char *cpf) {
  struct sockaddr_in address;
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      printf("\n Socket creation error \n");
      return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
  {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
  }
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      printf("\nConnection Failed \n");
      return -1;
  }

  cout << endl;
  cout << "----- Mensagens da comunicação cliente-servidor ----- \n";

  send( sock , cpf , strlen(cpf) , 0 );
  printf("Este cliente: CPF enviado.\n");

  send( sock , voto , strlen(voto) , 0 );
  printf("Este cliente: Voto enviado.\n");

  read( sock , buffer, 50);
  printf("%s\n",buffer );

    cout << "-------------------------------------------------------- \n";
}

int main() {
  char *voto;
  voto = new char[10];
  char *cpf;
  cpf = new char[10];
  bool found_cpf = false;

  cout << "Bem-vindo ao sistema de votação em Blockchain, siga os passos e registre seu voto" << endl;
  cout << endl;
  cout << "Por favor, insira o seu CPF: ";
  cin >> cpf;
  while (found_cpf == false) {
    found_cpf = findCPF(cpf);
    if (found_cpf == false) {
      cout  << "CPF inválido, tente novamente: ";
      cin >> cpf;
    }
  }
  found_cpf = false;

  cout << "Digite o numero ao lado do candidato que em que deseja votar: \n"
       << "1. Mintsu \n"
       << "2. Carlo Kleber \n"
       << "3. Lula \n"
       << "Voto : ";
  cin >> voto;

  while (voto[0] < '1' || voto[0] > '3') {
    cout << "Voto invalido, por favor selecione apenas uma das opcoes acima: ";
    cin >> voto;
  }

  createConnection(voto,cpf);

    return 0;
}
