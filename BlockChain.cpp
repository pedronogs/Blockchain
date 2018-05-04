#include "sha256.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <clocale>
#include <time.h>

using namespace std;

class Block{
    private:
        int indice;
        string data_do_voto;
        int voto;
        string hash_do_bloco;
        string hash_anterior;
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
        Block* get_chain()
        {
            return chain;
        }

        void set_chain(Block* p)
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

            Block* novo_bloco = new Block(indice, data_do_voto, voto, hash_anterior, cpf);

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
            Block* a = primeiro;

            if(vazia())
                cout << "O BlockChain ainda está vazio. \n";

            else
            {
                while(a)
                {
                    cout << "Índice         ------- " << a->get_indice() << endl;
                    cout << "Data do voto   ------- " << a->get_data_do_voto() << endl;
                    cout << "Voto           ------- " << a->get_voto() << endl;
                    cout << "CPF            ------- " << a->get_cpf() << endl;
                    cout << "Hash Anterior  ------- " << a->get_hash_anterior() << endl;
                    cout << "Hash           ------- " << a->get_hash() << endl << endl << endl;

                    a = a->get_chain();
                }
                cout << endl;
            }

        }
};
int main()
{
    setlocale(LC_ALL, "");
    int menu = 0;
    system("color 02");
    BlockChain meu_blockchain;
    int Bolsonaro = 0;
    int Lula = 0;
    int Luciano_Huck = 0;
    int j = 1;                                              //int para o indice
    int aux;                                            //int para o voto
    string data;                                        //string para data
    string cpf;                                         //string para o cpf
    while(menu != 4)
    {
        cout << "Bem vindo ao Sistema de Eleições feito em BlockChain! \nSelecione o que deseja fazer: \n"
             << "1. Votar! \n"
             << "2. Apresentar o BlockChain formado! \n"
             << "3. Apresentar a quantidade de votos de cada participante. \n"
             << "4. Sair. \n ";
        cin >> menu;

        switch(menu)
        {
            case 1:
                cout << "Você selecionou a opção Votar, insira, por favor, o seu cpf: ";
                cin >> cpf;

                data = __DATE__;

                cout << "Digite o número ao lado do candidato que em que deseja votar: \n"
                     << "1. Bolsonaro \n"
                     << "2. Lula \n"
                     << "3. Luciano Huck \n"
                     << "Voto : ";
                cin >> aux;

                switch(aux)
                {
                    case 1:
                        Bolsonaro++;
                        break;
                    case 2:
                        Lula++;
                        break;
                    case 3:
                        Luciano_Huck++;
                        break;
                    default:
                        cout << "Voto inválido, por favor selecione apenas uma das opções acima: ";
                        cin >> aux;
                }
                meu_blockchain.inserir_bloco(j, data, aux, " ", cpf);
                j++;
                break;
            case 2:
                meu_blockchain.mostrar();
                break;
            case 3:
                cout << "Votos para Bolsonaro    = " << Bolsonaro << endl;
                cout << "Votos para Lula         = " << Lula << endl;
                cout << "Votos para Luciano Huck = " << Luciano_Huck << endl;
                break;
            case 4:
                break;
            default:
                cout << "Por favor selecione alguma das opções acima. \n";
                cin >> menu;
        }

        system("pause");
        system("cls");
    }
    return 0;


}
