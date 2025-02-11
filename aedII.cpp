#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

#ifdef _WIN32
   #define CLEAR_SCREEN "CLS"
#else
   #define CLEAR_SCREEN "clear"
#endif

struct Paciente {
   int id;
   int prioridade;
   string nome;
   string telefone;
   string historicoMedico;
};

struct NodoAVL {
   Paciente paciente;
   int altura;
   NodoAVL* esq;
   NodoAVL* dir;
};

unordered_map<int, string> mapIdParaNome;
unordered_map<string, int> mapNomeParaId;

int altura(NodoAVL* nodo) {
   return nodo ? nodo->altura : 0;
}

int fatorBalanceamento(NodoAVL* nodo) {
   return nodo ? altura(nodo->esq) - altura(nodo->dir) : 0;
}

NodoAVL* rotacaoDireita(NodoAVL* y) {
   NodoAVL* x = y->esq;
   y->esq = x->dir;
   x->dir = y;
   y->altura = max(altura(y->esq), altura(y->dir)) + 1;
   x->altura = max(altura(x->esq), altura(x->dir)) + 1;
   return x;
}

NodoAVL* rotacaoEsquerda(NodoAVL* x) {
   NodoAVL* y = x->dir;
   x->dir = y->esq;
   y->esq = x;
   x->altura = max(altura(x->esq), altura(x->dir)) + 1;
   y->altura = max(altura(y->esq), altura(y->dir)) + 1;
   return y;
}

NodoAVL* balancear(NodoAVL* nodo) {
   int balance = fatorBalanceamento(nodo);
   if (balance > 1) {
       if (fatorBalanceamento(nodo->esq) < 0)
           nodo->esq = rotacaoEsquerda(nodo->esq);
       return rotacaoDireita(nodo);
   }
   if (balance < -1) {
       if (fatorBalanceamento(nodo->dir) > 0)
           nodo->dir = rotacaoDireita(nodo->dir);
       return rotacaoEsquerda(nodo);
   }
   return nodo;
}

NodoAVL* inserir(NodoAVL* raiz, Paciente paciente) {
   if (!raiz) {
       NodoAVL* novo = new NodoAVL{paciente, 1, nullptr, nullptr};
       mapIdParaNome[paciente.id] = paciente.nome;
       mapNomeParaId[paciente.nome] = paciente.id;
       return novo;
   }
   if (paciente.prioridade < raiz->paciente.prioridade ||
      (paciente.prioridade == raiz->paciente.prioridade && paciente.id < raiz->paciente.id))
       raiz->esq = inserir(raiz->esq, paciente);
   else
       raiz->dir = inserir(raiz->dir, paciente);

   raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
   return balancear(raiz);
}

NodoAVL* buscarPorID(NodoAVL* raiz, int id) {
   if (!raiz) return nullptr;
   if (raiz->paciente.id == id) return raiz;
   NodoAVL* encontrado = buscarPorID(raiz->esq, id);
   return encontrado ? encontrado : buscarPorID(raiz->dir, id);
}

NodoAVL* buscarPorNome(NodoAVL* raiz, const string& nome) {
   if (!raiz) return nullptr;
   if (raiz->paciente.nome == nome) return raiz;
   NodoAVL* encontrado = buscarPorNome(raiz->esq, nome);
   return encontrado ? encontrado : buscarPorNome(raiz->dir, nome);
}

NodoAVL* buscarMinimo(NodoAVL* raiz) {
   while (raiz->esq) raiz = raiz->esq;
   return raiz;
}

NodoAVL* remover(NodoAVL* raiz, int id) {
   if (!raiz) return raiz;

   if (id < raiz->paciente.id)
       raiz->esq = remover(raiz->esq, id);
   else if (id > raiz->paciente.id)
       raiz->dir = remover(raiz->dir, id);
   else {
       if (!raiz->esq || !raiz->dir) {
           NodoAVL* temp = raiz->esq ? raiz->esq : raiz->dir;
           if (!temp) {
               temp = raiz;
               raiz = nullptr;
           } else {
               *raiz = *temp;
           }
           delete temp;
       } else {
           NodoAVL* temp = buscarMinimo(raiz->dir);
           raiz->paciente = temp->paciente;
           raiz->dir = remover(raiz->dir, temp->paciente.id);
       }
   }

   if (!raiz) return raiz;

   raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
   return balancear(raiz);
}

NodoAVL* removerPaciente(NodoAVL* raiz, const string& chave) {
   int id;
   if (isdigit(chave[0])) {
       id = stoi(chave);
   } else {
       if (mapNomeParaId.find(chave) == mapNomeParaId.end()) {
           cout << "Paciente nao encontrado.\n";
           return raiz;
       }
       id = mapNomeParaId[chave];
   }

   NodoAVL* temp = buscarPorID(raiz, id);
   if (temp) {
       mapNomeParaId.erase(temp->paciente.nome);
       mapIdParaNome.erase(id);
       raiz = remover(raiz, id);
   }
   
   cout << "Paciente removido com sucesso.\n";
   return raiz;
}

void alterarPrioridade(NodoAVL*& raiz, const string& chave, int novaPrioridade) {
   int id;
   if (isdigit(chave[0])) {
       id = stoi(chave);
   } else {
       if (mapNomeParaId.find(chave) == mapNomeParaId.end()) {
           cout << "Paciente nao encontrado.\n";
           return;
       }
       id = mapNomeParaId[chave];
   }

   NodoAVL* paciente = buscarPorID(raiz, id);
   if (paciente) {
       Paciente dados = paciente->paciente;
       raiz = remover(raiz, id);
       dados.prioridade = novaPrioridade;
       raiz = inserir(raiz, dados);
       cout << "Ordem de prioridade foi alterada com sucesso.\n";
   } else {
       cout << "Nao existe paciente para alterar prioridade.\n";
   }
}

// Função para listar pacientes em ordem (inorder)
void listarPacientes(NodoAVL* raiz) {
   if (!raiz) return; // Não exibe nada se não houver pacientes

   listarPacientes(raiz->esq);
   cout << "ID: " << raiz->paciente.id << " | Nome: " << raiz->paciente.nome
        << " | Prioridade: " << raiz->paciente.prioridade << endl;
   listarPacientes(raiz->dir);
}

// Função do Menu
void menu(NodoAVL*& raiz) {
   int opcao;
   do {
       system(CLEAR_SCREEN);
       cout << "\nMenu - Sistema de Gerenciamento de Hospital\n";
       cout << "1. Inserir paciente\n";
       cout << "2. Listar pacientes\n";
       cout << "3. Buscar paciente por ID ou Nome\n";
       cout << "4. Alterar prioridade de paciente\n";
       cout << "5. Remover paciente\n";
       cout << "0. Sair\n";
       cout << "Escolha uma opcao: ";
       cin >> opcao;

       switch(opcao) {
           case 1: {
               Paciente p;
               cout << "ID: "; cin >> p.id;
               cout << "Prioridade: "; cin >> p.prioridade;
               cin.ignore();
               cout << "Nome: "; getline(cin, p.nome);
               cout << "Telefone: "; getline(cin, p.telefone);
               cout << "Historico medico: "; getline(cin, p.historicoMedico);
               raiz = inserir(raiz, p);
               cout << "\nPaciente inserido com sucesso.\n";
               cin.ignore(); cin.get();
               break;
           }
           case 2: {
               if (raiz == nullptr) {
                   cout << "Nenhum paciente foi cadastrado.\n";
               } else {
                   cout << "\nListagem de Pacientes:\n";
                   listarPacientes(raiz);
               }
               cin.ignore(); cin.get();
               break;
           }
           case 3: {
               if (raiz == nullptr) {
                   cout << "Nenhum paciente foi cadastrado.\n";
                   cin.ignore(); cin.get();
                   break;
               }
               cout << "Digite o ID ou Nome do paciente a buscar: ";
               string chave;
               cin.ignore();
               getline(cin, chave);
               NodoAVL* paciente = nullptr;

               if (isdigit(chave[0])) {
                   int id = stoi(chave);
                   paciente = buscarPorID(raiz, id);
               } else {
                   paciente = buscarPorNome(raiz, chave);
               }

               if (paciente) {
                   cout << "Paciente encontrado:\n";
                   cout << "ID: " << paciente->paciente.id << " | Nome: " << paciente->paciente.nome
                        << " | Prioridade: " << paciente->paciente.prioridade << endl;
               } else {
                   cout << "Paciente nao encontrado.\n";
               }
               cin.ignore(); cin.get();
               break;
           }
           case 4: {
               if (raiz == nullptr) {
                   cout << "Nenhum paciente foi cadastrado.\n";
                   cin.ignore(); cin.get();
                   break;
               }
               cout << "Digite o ID ou Nome do paciente: ";
               string chave;
               cin.ignore();
               getline(cin, chave);
               int novaPrioridade;
               cout << "Digite a nova prioridade (1 a 5): "; cin >> novaPrioridade;
               alterarPrioridade(raiz, chave, novaPrioridade);
               cin.ignore(); cin.get();
               break;
           }
           case 5: {
               if (raiz == nullptr) {
                   cout << "Nenhum paciente foi cadastrado.\n";
                   cin.ignore(); cin.get();
                   break;
               }
               cout << "Digite o ID ou Nome do paciente a remover: ";
               string chave;
               cin.ignore();
               getline(cin, chave);
               raiz = removerPaciente(raiz, chave);
               cin.ignore(); cin.get();
               break;
           }
           case 0:
               cout << "Saindo...\n";
               break;
           default:
               cout << "Opcao invalida. Tente novamente.\n";
               cin.ignore(); cin.get();
               break;
       }
   } while (opcao != 0);
}

int main() {
   NodoAVL* raiz = nullptr;
   menu(raiz);
   return 0;
}