#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>

using namespace std;

vector<unsigned char> StrToVector(string& s){
    vector<unsigned char> vetor;
    vetor.reserve(s.size());

    if (!s.empty() && s.back() == '\0') {
        vetor.assign(s.begin(), s.end() - 1);
    } else {
        vetor.assign(s.begin(), s.end());
    }
    return vetor;
}

string VectorToStr(const vector<unsigned char>& vetor){
    string str(vetor.begin(), vetor.end());
    return str;
}

std::string md5(const std::string &str){
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    std::stringstream ss;

    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>( hash[i] );
    }
    return ss.str();
}

int main() {
    string senhaMisteriosa = "7aa2d52c87d4e78738ecd70142af805c"; // DexterPas
    
    bool encontrado = false;
    
    for(int tamanhoSenha = 5; tamanhoSenha<=10 && !encontrado; tamanhoSenha++){ // Testa senhas de 5 a 10 chars
        bool terminou = false;
        vector<unsigned char> tentativaSenha(tamanhoSenha, 0x00);  // Inicia com todos os bytes em 0

        while (!terminou && !encontrado) {
            // Testa a tentativa atual
            string senhaAtual = VectorToStr(tentativaSenha);
            string senhaAtualCrypto = md5(senhaAtual);

            // Correção: usando a variável correta 'senhaMisteriosa'
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