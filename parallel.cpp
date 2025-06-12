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
#define NUM_BITS 3
#define PARAR 42

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

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    string senhaMisteriosa = "25771f6518f331cf9ab1f36db4d736f1"; // DeBrA
    
    bool encontrado = false;
    long long iteradorVerificador = 0;
    
    for(int tamanhoSenha = 5; tamanhoSenha<=10 && !encontrado; tamanhoSenha++){ // Testa senhas de 5 a 10 chars
        vector<unsigned char> tentativaSenha(tamanhoSenha, 0x00);
        unsigned char prefixo_byte0 = (unsigned char)(rank << (8 - NUM_BITS));
        tentativaSenha[0] = prefixo_byte0;

        unsigned char mascara_byte0 = (1 << (8 - NUM_BITS)) - 1;
        unsigned char max_val_byte0 = prefixo_byte0 | mascara_byte0;

        bool terminou_tamanho_atual = false;
        
        while(!terminou_tamanho_atual && !encontrado){
            
            if(iteradorVerificador % 1000000 == 0){
                cout << "Nó "<< rank << " ainda Trabalhando " << iteradorVerificador/1000000<< " ...\n";
            }
            iteradorVerificador++;
            
            int mensagemChegou = 0; // Guardará o sinal de parada
            MPI_Status status;
            MPI_Iprobe(MPI_ANY_SOURCE, PARAR, MPI_COMM_WORLD, &mensagemChegou, &status); // Verifica se existe algum sinal de parada emitido
            
            if(mensagemChegou){
                encontrado = true;
            }

            if(md5(VectorToStr(tentativaSenha)) == senhaMisteriosa && !encontrado){
                cout << "Nó " << rank << " encontrou a senha: " << VectorToStr(tentativaSenha) << "\n";
                fflush(stdout);
                
                encontrado = true;

                // Envia a mensagem de parada para todos os outros nós
                int sinalDeParada = 1;
                for(int i = 0; i < size; ++i){
                    if(i != rank){
                        MPI_Send(&sinalDeParada, 1, MPI_INT, i, PARAR, MPI_COMM_WORLD);
                    }
                }
            }

            if(!encontrado){
                int i = tamanhoSenha - 1;
                bool incrementoRealizado = false;
                while(i >= 0 && !incrementoRealizado){
                    if (i == 0) {
                        if (tentativaSenha[i] < max_val_byte0) {
                            tentativaSenha[i]++; incrementoRealizado = true;
                        } 
                        else{
                            i--;
                        }
                    } else {
                        if (tentativaSenha[i] < 0xFF) {
                            tentativaSenha[i]++; incrementoRealizado = true;
                        }
                        else{
                            tentativaSenha[i] = 0x00; i--;
                        }
                    }
                }
                if(i < 0){
                    terminou_tamanho_atual = true;
                }
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce(MPI_IN_PLACE, &encontrado, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);

    if (rank == 0 && !encontrado) {
        cout << "Senha não encontrada!" << endl;
    }
    
    MPI_Finalize();
    return 0;
}