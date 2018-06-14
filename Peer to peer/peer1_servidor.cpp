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
#define PORT 8081

using namespace std;

string randomstring () {
    static const string charList = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    string alphanumeric = "";

    for(int i = 0; i < 10; i++) {
      alphanumeric += charList [rand() % charList.size()];
    }
    return alphanumeric;
}

class Block{
    private:
        int indice;
        string data_do_voto;
        int voto;
        string hash_do_bloco;
        string hash_anterior;
        string hash_proof;
        string cpf;
        Block *chain;

    public:
        Block(int indice, string data_do_voto, int voto, string hash_anterior, string cpf)
        {
            this->indice = indice;
            this->data_do_voto = data_do_voto;
            this->voto = voto;
            this->hash_anterior = hash_anterior;
            this->hash_do_bloco = this->calcular_hash();
            // this->hash_proof = this->proof_of_work(4);
            this->cpf = cpf;
            this->chain = NULL;
        }

        int get_indice()
        {
            return indice;
        }

        string get_data_do_voto()
        {
            return data_do_voto;
        }

        int get_voto()
        {
            return voto;
        }

        string get_hash()
        {
            return hash_do_bloco;
        }

        string get_hash_anterior()
        {
            return hash_anterior;
        }

        string get_proof_of_work()
        {
          return hash_proof;
        }

        string get_cpf()
        {
            return cpf;
        }

        void set_indice(int x)
        {
            indice = x;
        }

        void set_data_do_voto(string a)
        {
            data_do_voto = a;
        }

        void set_voto(int x)
        {
            voto = x;
        }

        void set_cpf(string a)
        {
            cpf = a;
        }

        string calcular_hash()
        {
            string sindice;
            stringstream convert;
            convert << indice;
            sindice = convert.str();

            string svoto;
            stringstream convert2;
            convert2 << voto;
            svoto = convert2.str();

            string dados = sindice + data_do_voto + svoto + hash_anterior + cpf;
            char *aux = new char[dados.size() + 1];
            copy(dados.begin(), dados.end(), aux);

            string the_hash = SHA256(aux);
            return the_hash;
        }

        // string proof_of_work(int difficulty) {
        //   char cstr[difficulty + 1];
        //   for (int i = 0; i < difficulty; ++i) {
        //       cstr[i] = '0';
        //   }
        //   cstr[difficulty] = '\0';
        //
        //   string zeroes(cstr), aleat = randomstring(), hashed, zeroes_hashed;
        //   char *proof = new char[10];
        //
        //   for (int i = 0; i < 10; i++ ) {
        //     proof[i] = aleat[i];
        //   }
        //
        //   hashed = SHA256(proof);
        //   zeroes_hashed = hashed.substr(0,difficulty);
        //
        //   while (zeroes_hashed != zeroes ) {
        //     aleat = randomstring();
        //     for (int i = 0; i < 10; i++ ) {
        //       proof[i] = aleat[i];
        //     }
        //     hashed = SHA256(proof);
        //     cout << hashed << endl;
        //     zeroes_hashed = hashed.substr(0,difficulty);
        //   }
        // return hashed;
        // }

        Block *get_chain()
        {
            return chain;
        }

        void set_chain(Block *p)
        {
            chain = p;
        }
};

class BlockChain{
    private:
        Block *primeiro;
        Block *ultimo;

    public:
        BlockChain()
        {
            this->primeiro = NULL;
            this->ultimo = NULL;
        }

        BlockChain(int indice, string data_do_voto, int voto, string hash_anterior, string cpf)
        {
            primeiro = new Block(indice, data_do_voto,voto, hash_anterior, cpf);
            ultimo = primeiro;
        }

        bool vazia()
        {
            return (primeiro == NULL);
        }

        void inserir_bloco(int indice, string data_do_voto, int voto, string hash_anterior, string cpf)
        {
            if(vazia())
                hash_anterior = "0";
            else
                hash_anterior = ultimo->get_hash();

            Block *novo_bloco = new Block(indice, data_do_voto, voto, hash_anterior, cpf);

            if(vazia())
            {
                primeiro = novo_bloco;
                ultimo = novo_bloco;
            }

            else
            {
                ultimo->set_chain(novo_bloco);
                ultimo = novo_bloco;
            }
        }

        char* mostraVoto()
        {
            Block *a = primeiro;
            char *hashbloco;
            string hasher = a->get_hash();
            int i = 0;

            while (hasher[i] != '\0') {
              hashbloco[i] = hasher[i];
              i++;
            }
            return hashbloco;
          }

        void mostrar()
        {
            cout << "Blocos formados: \n";
            cout << endl;
            Block *a = primeiro;
            ofstream myfile;

            if(vazia())
                cout << "O BlockChain ainda esta vazio. \n";

            else
            {
                myfile.open("Blockchain - registros.txt", ios::trunc);
                myfile.close();
                while(a)
                {
                    cout << "Indice         ------- " << a->get_indice() << endl;
                    cout << "Data do voto   ------- " << a->get_data_do_voto() << endl;
                    cout << "Voto           ------- " << a->get_voto() << endl;
                    cout << "CPF            ------- " << a->get_cpf() << endl;
                    cout << "Hash Anterior  ------- " << a->get_hash_anterior() << endl;
                    cout << "Hash Atual     ------- " << a->get_hash() << endl ;
                    cout << endl;
                    // cout << "POW Hash       ------- " << a->get_proof_of_work() << endl << endl;

                    myfile.open ("Blockchain - registros.txt", ios::app);
                      if (myfile.is_open()) {
                        myfile << "Índice: " << a->get_indice() << endl;
                        myfile << "Data: " << a->get_data_do_voto() << endl;
                        myfile << "Voto registrado: " << a->get_voto() << endl;
                        myfile << "CPF: " << a->get_cpf() << endl;
                        myfile << "Hash anterior: " << a->get_hash_anterior() << endl;
                        myfile << "Hash atual: " <<  a->get_hash() << endl;
                        // myfile << a->get_proof_of_work() << endl;
                        myfile << endl;
                      }
                    myfile.close();

                    a = a->get_chain();
                }
                cout << endl;
            }
        }

        void votos () {
          Block *a = primeiro;
          int voto[5] = {0,0,0,0,0};

          if(vazia())
              cout << "O BlockChain ainda esta vazio. \n";
          else {
            while (a) {
                if ((a->get_voto()) == 1) voto[0]++;
                if ((a->get_voto()) == 2) voto[1]++;
                if ((a->get_voto()) == 3) voto[2]++;
                if ((a->get_voto()) == 4) voto[3]++;
                if ((a->get_voto()) == 5) voto[4]++;

                a = a->get_chain();
            }
            cout << "Votos pelo candidato do PT: " << voto[0] << endl;
            cout << "Votos pelo candidato do PSL: " << voto[1] << endl;
            cout << "Votos pela candidata da Rede: " << voto[2] << endl;
            cout << "Votos pelo candidata do PDT: " << voto[3] << endl;
            cout << "Votos em branco: " << voto[4] << endl;
          }
        }

        bool verifica_blocos () {
          Block *atual = primeiro;
          string hash_passado = "0";

          cout << "Comparando o hash anterior do bloco atual com o hash atual do bloco anterior ..." << endl;
          cout << endl;
          while (atual) {
              if ((atual->get_hash_anterior()) != (hash_passado)) return false;
              else {
                cout << hash_passado << endl;
                cout << atual->get_hash_anterior() << endl;

                hash_passado = atual->get_hash();
                atual = atual->get_chain();
              }
          }
          cout << endl;
          cout << "Resultado parcial: válido" << endl;
          cout << endl;
          return true;
        }

        bool verifica_hash (char *hash_recebido) {
          string hash_atual = ultimo->get_hash();

          cout << "Comparando hash recebido com o hash calculado ..." << endl;
          for (int i = 0; i < 64; i++) {
            if (hash_atual[i] != hash_recebido[i]) {
              cout << "Hash recebido difere do hash calculado." << endl;
              cout << "Resultado parcial: inválido" << endl;
              return false;
            }
          }
          cout << endl;
          cout << hash_atual << endl;
          cout << hash_recebido << endl;
          cout << endl;
          cout << "Resultado parcial: válido" << endl;
          return true;
        }

        string mostra_hash() {
          Block *atual = ultimo;
          return atual->get_hash();
        }
};

int main()
{
    system("clear");
    cout << "Inicializando criação do socket de servidor..." << endl;

    // Configurações da classe BlockChain e do programa principal
    srand(time(NULL));
    int j = 1;
    int voto;
    string data = __DATE__;
    string cpf;
    string proof;
    char escolha;
    BlockChain meu_blockchain;

    // Configurações iniciais do socket (servidor)
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char *buffer, *buffer1, *buffer2;
    buffer = new char [64];
    buffer1 = new char [1];
    buffer2 = new char [64];
    int i = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) // Criação do socket servidor
    {
        cout << "Criação do socket falhou." << endl;
    }

    // Forçando o uso da porta 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        cout << "Falha na conexão com a porta <" << PORT << ">." << endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        cout << "Falha no endereçamento do socket." << endl;
    }
    // Fim das configurações iniciais do socket (servidor)

    while (1) {
      cout << "Servidor em funcionamento. Socket esperando conexões..." << endl;
      cout << endl;

      // Inicia a função do socket servidor
      if (listen(server_fd, 100) < 0) // Escuta por conexões de um outro cliente -- programa entra em loop
      {
          cout << "Erro no período de escuta." << endl;
      }

      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  // Ao receber uma conexão, o servidor a aceita e realiza funções
                         (socklen_t*)&addrlen))<0)
      {
          cout << "Nao foi possivel aceitar conexao." << endl;
      }

      read(new_socket , buffer, 64);
      cpf = buffer;

      read(new_socket , buffer1, 1);
      voto = buffer1[0] - 48;

      meu_blockchain.inserir_bloco(j, data, voto, " ", cpf);

      string hash_atual = meu_blockchain.mostra_hash();
      char *hash_envio = new char [64];
      for (int i = 0; i < 64; i++) {
        hash_envio[i] = hash_atual[i];
      }

      send ( new_socket , hash_envio , strlen(hash_envio) , 0 );

      read(new_socket , buffer2, 64);
      char *hash_recebido = new char [64];
      hash_recebido = buffer2;

      j++;

      if (meu_blockchain.verifica_blocos() == true && meu_blockchain.verifica_hash(hash_recebido) == true) {
          cout << "Resultado final: cadeia de blocos válida" << endl << endl;
          meu_blockchain.mostrar();
        }
      else {
        cout << "Cadeia de blocos inválida." << endl;
        return 0;
      }
      meu_blockchain.votos();

      cout << endl;
      cout << "----- Mensagens da comunicação entre os sockets de cliente e servidor ----- \n";

      cout << "Este servidor: CPF recebido." << endl;
      cout << "Este servidor: Voto recebido." << endl;
      cout << "Este servidor: Dados computados." << endl;

      cout << "-------------------------------------------------------- \n";
      cout << endl;
  }
  return 0;
}
