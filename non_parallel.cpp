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

int main() {
    //string senhaMisteriosa = "7aa2d52c87d4e78738ecd70142af805c"; // DexterPas
    string senhaMisteriosa = "5eabcf4e5f25cec96078e633ad4ddf2c"; //Debs
    bool encontrado = false;
    
    //for(int tamanhoSenha = 5; tamanhoSenha<=10 && !encontrado; tamanhoSenha++){ // Testa senhas de 5 a 10 chars
    for(int tamanhoSenha = 3; tamanhoSenha<=5 && !encontrado; tamanhoSenha++){
        bool terminou = false;
        vector<unsigned char> tentativaSenha(tamanhoSenha, 0x00);  // Inicia com todos os bytes em 0

        while (!terminou && !encontrado) {
            // Testa a tentativa atual
            string senhaAtual = VectorToStr(tentativaSenha);
            string senhaAtualCrypto = md5(senhaAtual);

            if (senhaAtualCrypto == senhaMisteriosa) {
                encontrado = true;
                cout << "A senha é: " << senhaAtual << "\n";
            }

            if (!encontrado) {
                int i = tamanhoSenha - 1;
                bool incrementoRealizado = false;

                while (i >= 0 && !incrementoRealizado) {
                    if (tentativaSenha[i] < 0xFF) { // Menor que 255
                        tentativaSenha[i]++;       // Incrementa o byte i
                        incrementoRealizado = true; // Sinaliza que o incremento foi feito, para sair do laço
                    } else {
                        tentativaSenha[i] = 0x00; // Se for 255, reseta o byte
                        i--;
                    }
                }
                if (i < 0) {
                    terminou = true;
                }
            }
        }
    }
    if(!encontrado){
        cout << "Senha não encontrada";
    }
    return 0;
}