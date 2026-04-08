# Trabalho 2: Order Book (Sistema de Ordens de Compra e Venda)
Neste projeto, implementamos um sistema simples para gerenciar ordens de compra e venda de uma única ação. 
Uma ordem representa a intenção de comprar ou vender a ação por um determinado preço. O sistema é capaz de armazenar ordens em aberto e executar automaticamente transações sempre que houver compatibilidade entre ordens.

## Instruções de compilação
Certifique-se de ter um compilador c++ instalado. 
Para compilar o projeto, digite no terminal:

`g++ main.cpp Order.cpp OrderBook.cpp Transaction.cpp -o main`

## Instruções de execução
Após a compilação, execute: `.\main.exe`

## Estrutura dos dados
A classe principal do nosso sistema, `OrderBook`, armazena as seguintes informações:
- Node* BuyHead;
- Node* SellHead;
- TransactionNode* TransHead;
- TransactionNode* TransTail;
- int sizeBuy;
- int sizeSell;
- int sizeTrans;

Para armazenar as orders de compra e venda e as transações foi utilizado listas encadeadas, sendo listas encadeadas simples para as ordens e lista encadeada dupla para as transações. Para implementar a lista encadeada foi criada uma struct Node, para armazenar as orders de compra e venda, com um atributo do tipo Order e outro atributo que é um ponteiro que aponta para a próxima ordem da lista, e foi criada uma struct NodeTransaction, que possui um atributo do tipo Transaction e outro atributo que é um ponteiro que aponta para a próxima transação da lista.

Essas duas variáveis: 

```cpp
Node* BuyHead;
Node* SellHead;
```
armazenam apenas o primeiro elemento da lista encadeada das ordens de compra e venda, mas como a lista de ordens de compra está ordenada do maior preço para o menor, eu só preciso analisar o BuyHead quando for adicionar uma nova ordem de venda, e o mesmo vale para o oposto, ao adicionar uma nova ordem de compra eu só olho para o SellHead, pois a lista já está ordenada pelo menor preço.

Já essas duas variáveis:

```cpp
TransactionNode* TransHead;
TransactionNode* TransTail;
```
armazenam o primeiro e o último elemento da lista encadeada de transações. Isso ajuda quando for adicionar uma nova transação na lista, ao invés de percorrer a lista toda com um while eu só adiciono o elemento ao final com o TransTail.

E essas variáveis: 

```cpp
int sizeBuy;
int sizeSell;
int sizeTrans;
```
armazenam o tamanho de cada lista encadeada.

