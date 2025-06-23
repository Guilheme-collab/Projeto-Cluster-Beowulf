# Projeto Cluster Beowulf

Código na linguagem C++, utilizando da ferramenta de MPI para realizar quebra de senha em função hash MD5 de 5 a 10 caracteres via método de força bruta, utilizando de metodologia de processamento em paralelo. O projeto visa o aprendizado, especificamente a apresentação de conceitos de paralelismo em prática, não buscando resultados concretos.

## Funcionamento

O código inicia o funcionamento de um ambiente MPI e, mediante a divisão do processamento em 4 nós, irá particionar o espaço de possibilidades de senha entre os nós do cluster, fixando-se os 2 primeiros bits	de cada senha.

* Primeiro nó: senhas com primeiros dois bits 00.
* Segundo nó: senhas com primeiros dois bits 01.
* Terceiro nó: senhas com primeiros dois bits 10.
* Quarto nó: senhas com primeiros dois bits 11.

Cada nó irá realizar a tentativa de senhas em seus respectivos espaços.

### Construído com

O código foi estruturado de como a funcionar com as seguintes ferramentas e práticas:

* Linguagem C++
* Biblioteca OpenMPI
* Cluster Beowulf
* VirtualBox para simulação de vários computadores
* 1 Máquina Mestre de 1536Mb Memória, CPU 1 núcleo, Sistema Operacional Debian
* 4 Máquinas Escravas de 256Mb, CPU 1 núcleo, Sistema Operacional Debian
* Biblioteca OpenSSL
* Configuração de Rede Bridge

## Execução

Não há entradas no código, seu funcionamento é automático após inicialização. Após ser encontrada a senha correta, o nó em conclusão irá enviar um sinal de parada aos outros nós, encerrando o funcionamento do código.

### Observações

O projeto não visa eficiência ou até mesmo viabilidade, uma vez que é utilizado apenas um computador simulando os outros nós, logo a perfomance e velocidade
das tentativas de senha são extramemente lentas. O tempo estimado para encontrar a senha correta de 5 digitos utilizada como exemplo no código é de um mês.

### Resultados

O projeto, embora não consiga providenciar um resultado na quebra de senha de maneira eficaz ou viável mediante o tempo de execução necessária, ainda atinge
o objetivo principal de demonstrar o funcionamento em prática de arquitetura de paralelismo.

## Autores

* **Gabriel Derrel Martins Santee** - *Código C++* - [Github](https://github.com/gabriel0derrel)
* **Guilherme Ponciano Silva** - *Implementação do Cluster/VirtualBox* - [GitHub](https://github.com/Guilheme-collab)
* **Lucas Pereira Nunes** - *Artigo Científico* - [GitHub](https://github.com/LucasPNunes1)
* **Ronaldo Oliveira de Jesus** - *Documentação/Definição do Problema* - [GitHub](https://github.com/ParadoxIsReal)

Você também pode ver a lista de todos os [colaboradores](https://github.com/Guilheme-collab/Projeto-Cluster-Beowulf/graphs/contributors) que participaram deste projeto.

Projeto desenvolvido para a disciplina de [Engenharia de Computação] – [PUCGO](https://www.pucgoias.edu.br/)

---
