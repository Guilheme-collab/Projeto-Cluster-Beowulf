// --------------------------- GRUPO ------------------------------
// Gabriel Derrel Martins Santee
// Guilherme Ponciano Silva
// Lucas Pereira Nunes
// Ronaldo Oliveira de Jesus

// --------------------------- GitHub -----------------------------
// Código no Github:
// https://github.com/Guilheme-collab/Projeto-Cluster-Beowulf
#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <iomanip>
#include <sstream>
#include <mpi.h>
#include <openssl/md5.h>
#define NUM_BITS 2

using namespace std;

string VectorToStr(const vector<unsigned char>& vetor){
    string str(vetor.begin(), vetor.end());
    return str;
}

string md5(const string &senha){
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, senha.c_str(), senha.size());
    MD5_Final(hash, &md5);

    stringstream ss;

    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        ss << hex << setw(2) << setfill('0') << static_cast<int>( hash[i] );
    }
    return ss.str();
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializa o MPI
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //string senhaMisteriosa = "7aa2d52c87d4e78738ecd70142af805c"; // DexterPas
    string senhaMisteriosa = "5eabcf4e5f25cec96078e633ad4ddf2c"; //Debs
    
    bool encontradoLocal = false;
    bool encontradoGlobal = false;
    
    while(!encontradoGlobal){
        //for(int tamanhoSenha = 5; tamanhoSenha<=10 && !encontradoGlobal; tamanhoSenha++){ // Testa senhas de 5 a 10 chars
        for(int tamanhoSenha = 3; tamanhoSenha<=5 && !encontradoGlobal; tamanhoSenha++){ // Testa senhas de 5 a 10 chars
            bool terminou = false;
            vector<unsigned char> tentativaSenha(tamanhoSenha, 0x00);  // Inicia com todos os bytes em 0
            unsigned char prefixo_byte0 = (unsigned char)(rank << (8 - NUM_BITS));
            tentativaSenha[0] = prefixo_byte0;

            unsigned char mascara_byte0 = (1 << (8 - NUM_BITS)) - 1;
            unsigned char max_val_byte0 = prefixo_byte0 | mascara_byte0;

            bool terminou_tamanho_atual = false;
            while (!terminou_tamanho_atual && !encontradoGlobal) {
                
                string senhaAtual = VectorToStr(tentativaSenha);
                string senhaAtualCrypto = md5(senhaAtual);

                if (senhaAtualCrypto == senhaMisteriosa) {
                    std::cout << "A senha é: " << senhaAtual << "\n";
                    encontradoLocal = true;
                    MPI_Allreduce(&encontradoLocal, &encontradoGlobal, 1, MPI_C_BOOL, MPI_LOR,MPI_COMM_WORLD);
                }

                if(!encontradoGlobal){
                    int i = tamanhoSenha - 1;
                    bool incrementoRealizado = false;

                    while (i >= 0 && !incrementoRealizado) {
                        // Caso especial para o primeiro byte (que contém o prefixo)
                        if (i == 0) {
                            if (tentativaSenha[i] < max_val_byte0) {
                                tentativaSenha[i]++;
                                incrementoRealizado = true;
                            } else {
                                // Se o primeiro byte já está no máximo, não há mais o que incrementar.
                                // O laço while(i>=0) vai terminar, pois i se tornará -1.
                                i--; 
                            }
                        } 
                        else {
                            if (tentativaSenha[i] < 0xFF) {
                                tentativaSenha[i]++;
                                incrementoRealizado = true;
                            } else {
                                tentativaSenha[i] = 0x00;
                                i--; // Passa o "carry" para o byte anterior
                            }
                        }
                    }

                    // Se i < 0, significa que todas as combinações para este tamanho e este prefixo foram testadas.
                    if (i < 0) {
                        terminou_tamanho_atual = true;
                    }
                }
            }
        }
        MPI_Allreduce(MPI_IN_PLACE, &encontradoGlobal, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);
    }
    if (rank == 0 && !encontradoGlobal) {
        cout << "Senha não encontrada." << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD); // Sincronização
    MPI_Finalize();
    return 0;
}