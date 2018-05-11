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
            this->hash_proof = this->proof_of_work(4);
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

        string proof_of_work(int difficulty) {
          char cstr[difficulty + 1];
          for (int i = 0; i < difficulty; ++i) {
              cstr[i] = '0';
          }
          cstr[difficulty] = '\0';

          string zeroes(cstr), aleat = randomstring(), hashed, zeroes_hashed;
          char *proof = new char[10];

          for (int i = 0; i < 10; i++ ) {
            proof[i] = aleat[i];
          }

          hashed = SHA256(proof);
          zeroes_hashed = hashed.substr(0,difficulty);

          while (zeroes_hashed != zeroes ) {
            aleat = randomstring();
            for (int i = 0; i < 10; i++ ) {
              proof[i] = aleat[i];
            }
            hashed = SHA256(proof);
            cout << hashed << endl;
            zeroes_hashed = hashed.substr(0,difficulty);
          }
        return hashed;
        }

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

        void mostrar()
        {
            cout << "Blocos formados: \n";
            Block *a = primeiro;
            ofstream myfile;

            if(vazia())
                cout << "O BlockChain ainda esta vazio. \n";

            else
            {
                myfile.open("Blockchain_register.txt", ios::trunc);
                myfile.close();
                while(a)
                {
                    cout << "Indice         ------- " << a->get_indice() << endl;
                    cout << "Data do voto   ------- " << a->get_data_do_voto() << endl;
                    cout << "Voto           ------- " << a->get_voto() << endl;
                    cout << "CPF            ------- " << a->get_cpf() << endl;
                    cout << "Hash Anterior  ------- " << a->get_hash_anterior() << endl;
                    cout << "Hash           ------- " << a->get_hash() << endl ;
                    cout << "POW Hash       ------- " << a->get_proof_of_work() << endl << endl;

                    myfile.open ("Blockchain_register.txt", ios::app);
                      if (myfile.is_open()) {
                        myfile << a->get_indice() << " ---- ";
                        myfile << a->get_data_do_voto() << " ---- ";
                        myfile << a->get_voto() << " ---- ";
                        myfile << a->get_cpf() << " ---- " << endl;
                        myfile << a->get_hash_anterior() << endl;
                        myfile << a->get_hash() << endl;
                        myfile << a->get_proof_of_work() << endl;
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
          int voto[3] = {0,0,0};

          if(vazia())
              cout << "O BlockChain ainda esta vazio. \n";
          else {
            while (a) {
                if ((a->get_voto()) == 1) voto[0]++;
                if ((a->get_voto()) == 2) voto[1]++;
                if ((a->get_voto()) == 3) voto[2]++;

                a = a->get_chain();
            }
            cout << "Votos pelo Bolsonaro: " << voto[0] << endl;
            cout << "Votos pelo Lula: " << voto[1] << endl;
            cout << "Votos pelo Luciano Huck: " << voto[2] << endl;
          }
        }

        bool verifica () {
          Block *atual = primeiro;
          string hash_passado = "0";

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
          return true;
        }
};

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "");
    int menu = 0;
    BlockChain meu_blockchain;
    int j = 1, i=0;                                              //int para o indice
    int aux;                                               //int para o voto
    string data;                                        //string para data
    string cpf;
    string proof;
    bool verificado, found_cpf = false;

    while(menu != 4)
    {
        cout << "\n"
             << "Bem vindo ao Sistema de Eleicoes feito em BlockChain! \nSelecione o que deseja fazer: \n"
             << "\n"
             << "1. Votar! \n"
             << "2. Apresentar o BlockChain formado! \n"
             << "3. Apresentar a quantidade de votos de cada participante. \n"
             << "4. Sair. \n"
             << "5. Criar database. \n"
             << "\n";

        cout << "Opcao selecionada: ";
        cin >> menu;
        cout << "\n";

        switch(menu)
        {
            case 1:
                cout << "Voce selecionou a opcao 'Votar'. Insira, por favor, o seu CPF: ";
                cin >> cpf;
                while (found_cpf == false) {
                  found_cpf = findCPF(cpf);
                  if (found_cpf == false) {
                    cout  << "CPF inválido, tente novamente: ";
                    cin >> cpf;
                  }
                }
                found_cpf = false;

                data = __DATE__;

                cout << "Digite o numero ao lado do candidato que em que deseja votar: \n"
                     << "1. Mintsu \n"
                     << "2. Carlo Kleber \n"
                     << "3. Lula \n"
                     << "Voto : ";
                cin >> aux;
                while (aux<1 || aux>3) {
                  cout << "Voto invalido, por favor selecione apenas uma das opcoes acima: ";
                  cin >> aux;
                }
                meu_blockchain.inserir_bloco(j, data, aux, " ", cpf);
                j++;
                break;
            case 2:
                verificado = meu_blockchain.verifica();
                if (verificado == true) {
                    cout << "Cadeia de blocos válida" << endl << endl;
                    meu_blockchain.mostrar();
                  }
                else cout << "Cadeia de blocos inválida" << endl;
                break;
            case 3:
                meu_blockchain.votos();
                break;
            case 4:
                break;
            case 5:
                cout << "Database criada, não rode novamente" << endl;
                databaseRegister();
                break;
            default:
                cout << "Por favor selecione alguma das opcoes acima. \n";
                cin >> menu;
        }
    }
    return 0;
}
