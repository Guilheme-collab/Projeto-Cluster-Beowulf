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
#include <openssl/md5.h>

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

int main(){
    //string senhaMisteriosa = "25771f6518f331cf9ab1f36db4d736f1"; // DeBrA
    string senhaMisteriosa = "38a03f670ea483d5ed5258e35f5a2d1d"; //Dex
    bool encontrado = false;

    unsigned int iteradorVerificador = 0;
    
    //for(int tamanhoSenha = 5; tamanhoSenha<=10 && !encontrado; tamanhoSenha++){ // Testa senhas de 5 a 10 chars
    for(int tamanhoSenha = 2; tamanhoSenha<=2 && !encontrado; tamanhoSenha++){
        bool terminou = false;
        vector<unsigned char> tentativaSenha(tamanhoSenha, 0x00);  // Inicia com todos os bytes em 0

        while(!terminou && !encontrado){
            // Testa a tentativa atual
            string senhaAtual = VectorToStr(tentativaSenha);
            string senhaAtualCrypto = md5(senhaAtual);

            if(senhaAtualCrypto == senhaMisteriosa){
                encontrado = true;
                cout << "A senha é: " << senhaAtual << "\n";
            }

            if(!encontrado){
                int i = tamanhoSenha - 1;
                bool incrementoRealizado = false;

                while (i >= 0 && !incrementoRealizado){
                    if(tentativaSenha[i] < 0xFF) { // Menor que 255
                        tentativaSenha[i]++;       // Incrementa o byte i
                        incrementoRealizado = true; // Sinaliza que o incremento foi feito, para sair do laço
                    }
                    else{
                        tentativaSenha[i] = 0x00; // Se for 255, reseta o byte
                        i--;
                    }
                }
                if(i < 0){
                    terminou = true;
                }
            }
            if(iteradorVerificador%1000000 == 0){
                cout << "Ainda Trabalhando " << iteradorVerificador/1000000 << " ...\n";
                iteradorVerificador = 0;
            }
            iteradorVerificador++;
        }
    }
    if(!encontrado){
        cout << "Senha não encontrada";
    }
    return 0;
}