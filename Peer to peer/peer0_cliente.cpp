#include "sha256.h"
#include "Database.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <clocale>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080

using namespace std;

void turnIntoClient (int peer, char *cpf_hasheado, char *voto) {
  struct sockaddr_in address;
  int sock = 0;
  struct sockaddr_in serv_addr;
  char *buffer;
  buffer = new char[64];
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
      cout << "Erro na criacao do socket" << endl;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(peer);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
  {
      cout << "Endereço errado";
      return;
  }
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      cout << "Conexão falhou, peer <" << peer << "> desconectado" << endl;
      return;
  }

  send ( sock , cpf_hasheado , 64 , 0 );
  send ( sock , voto , 1, 0 );

  char *hash_recebido = new char [64];
  read ( sock, buffer, 64 );
  hash_recebido = buffer;
  cout << "Hash recebido do servidor: " << hash_recebido << endl;
  cout << endl;
  cout << "Enviando para outros peers..." << endl;

  send ( sock , hash_recebido , 64 , 0 );

  close(sock);
}

int main () {
  system("clear");
  char *cpf = new char[10];
  char *voto = new char[1];

  while (1) {
      cout << endl;
      cout << "Bem-vindo ao sistema de votação em Blockchain, siga os passos e registre seu voto." << endl;
      cout << endl;
      cout << "Por favor, insira o seu CPF: ";
      cin >> cpf;
      cout << endl;

      int found_cpf = false;
      while (found_cpf == false) {
        found_cpf = findCPF(cpf);
        if (found_cpf == false) {
          cout  << "CPF inválido, tente novamente: ";
          cin >> cpf;
        }
      }
      found_cpf = false;

      string cpf_string = SHA256(cpf);
      char *cpf_hasheado = new char [64];
      for (int i = 0; i<64; i++) cpf_hasheado[i] = cpf_string[i];

      cout << endl;
      cout << "Digite o numero ao lado do candidato que em que deseja votar: \n"
           << endl
           << "1. Luís Inácio Lula da Silva (PT) \n"
           << "2. Jair Bolsonaro (PSL) \n"
           << "3. Marina Silva (Rede) \n"
           << "4. Ciro Gomes (PDT) \n"
           << "5. Branco (NULL) \n"
           << endl
           << "Voto registrado: ";
      cin >> voto;

      while (voto[0] < '1' || voto[0] > '5') {
        cout << endl;
        cout << "Voto invalido, por favor selecione apenas uma das opcoes acima: ";
        cin >> voto;
      }
      cout << endl;

      int peer[3] = {8080,8081,8082};
      int i = 0;
      while (i < 3) {
        turnIntoClient(peer[i], cpf_hasheado, voto);
        i++;
      }
      cout << endl;
      cout << "----------------------------------------------------------------------------------------------" << endl;
  }
}
