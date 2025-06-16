#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cstdlib> // Para system("CLS")
using namespace std;

struct Paciente { // Estrutura para armazenar informações de um paciente
    int id;
    int nivelUrgencia;
    string nome;
    string telefone;
    string historicoMedico;
};

struct NoAVL { // Nó da árvore AVL para armazenar pacientes
    Paciente paciente;
    int altura;
    NoAVL* esq;
    NoAVL* dir;
};

unordered_map<int, string> mapIdParaNome; // Mapeamento de ID para nome do paciente
unordered_map<string, int> mapNomeParaId; // Mapeamento de nome para ID do paciente

int altura(NoAVL* no) { // Função para retornar a altura do nó
    return no ? no->altura : 0;
}

int fatorBalanceamento(NoAVL* no) { // Calcula o fator de balanceamento de um nó
    return no ? altura(no->esq) - altura(no->dir) : 0;
}

NoAVL* rotacaoDireita(NoAVL* y) { // Realiza a rotação a direita
    NoAVL* x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    return x;
}

NoAVL* rotacaoEsquerda(NoAVL* x) { // Realiza a rotação a esquerda
    NoAVL* y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    return y;
}

NoAVL* balancear(NoAVL* no) { // Balanceia a árvore após uma operação de inserção ou remoção
    int balance = fatorBalanceamento(no);
    if (balance > 1) {
        if (fatorBalanceamento(no->esq) < 0)
            no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }
    if (balance < -1) {
        if (fatorBalanceamento(no->dir) > 0)
            no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }
    return no;
}

NoAVL* inserir(NoAVL* raiz, Paciente paciente) { //Insere um novo paciente na árvore AVL
    if (!raiz) {
        NoAVL* novo = new NoAVL{paciente, 1, nullptr, nullptr};
        mapIdParaNome[paciente.id] = paciente.nome;
        mapNomeParaId[paciente.nome] = paciente.id;
        return novo;
    }
    if (paciente.nivelUrgencia < raiz->paciente.nivelUrgencia ||
        (paciente.nivelUrgencia == raiz->paciente.nivelUrgencia && paciente.id < raiz->paciente.id))
        raiz->esq = inserir(raiz->esq, paciente);
    else
        raiz->dir = inserir(raiz->dir, paciente);

    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    return balancear(raiz);
}

NoAVL* buscarPorID(NoAVL* raiz, int id) { // Busca um paciente na árvore por ID
    if (!raiz) return nullptr;
    if (raiz->paciente.id == id) return raiz;
    if (id < raiz->paciente.id) return buscarPorID(raiz->esq, id);
    return buscarPorID(raiz->dir, id);
}

NoAVL* buscarPorNome(NoAVL* raiz, const string& nome) {  // Busca um paciente na árvore por nome
    if (!raiz) return nullptr;
    if (raiz->paciente.nome == nome) return raiz;
    NoAVL* encontrado = buscarPorNome(raiz->esq, nome);
    if (encontrado) return encontrado;
    return buscarPorNome(raiz->dir, nome);
}

NoAVL* remover(NoAVL* raiz, int id) {
    if (!raiz) return raiz;

    if (id < raiz->paciente.id) raiz->esq = remover(raiz->esq, id);
    else if (id > raiz->paciente.id) raiz->dir = remover(raiz->dir, id);
    else {
        if (!raiz->esq || !raiz->dir) {
            NoAVL* temp = raiz->esq ? raiz->esq : raiz->dir;
            if (!temp) { temp = raiz; raiz = nullptr; }
            else *raiz = *temp;
            delete temp;
        } else {
            NoAVL* temp = buscarPorNome(raiz->dir, raiz->paciente.nome);
            raiz->paciente = temp->paciente;
            raiz->dir = remover(raiz->dir, temp->paciente.id);
        }
    }

    if (raiz) {
        raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
        return balancear(raiz);
    }
    return raiz;
}

void listarPacientes(NoAVL* raiz) { // Lista todos os pacientes em ordem crescente por nível de urgência
    if (raiz) {
        listarPacientes(raiz->esq);
        cout << "ID: " << raiz->paciente.id << endl;
        cout << "Nome: " << raiz->paciente.nome << endl;
        cout << "Nivel de urgencia: " << raiz->paciente.nivelUrgencia << endl;
        cout << "Telefone: " << raiz->paciente.telefone << endl;
        cout << "Historico Medico: " << raiz->paciente.historicoMedico << "\n" << endl;
        listarPacientes(raiz->dir);
    }
}

void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void menu(NoAVL*& raiz) { // Função exibe o menu e interage com o usuário
    int opcao;
    do {
        cout << "\nMenu - Sistema de Gerenciamento de Hospital\n";
        cout << "1. Inserir paciente\n";
        cout << "2. Listar pacientes\n";
        cout << "3. Buscar paciente por ID ou Nome\n";
        cout << "4. Alterar Nivel de urgencia do paciente\n";
        cout << "5. Remover paciente\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch(opcao) {
            case 1: {
                Paciente p;
                cout << "ID: ";
                cin >> p.id;
                cout << "Nivel de urgencia (1 a 5): ";
                cin >> p.nivelUrgencia;
                cin.ignore();
                cout << "Nome: ";
                getline(cin, p.nome);
                cout << "Telefone: ";
                getline(cin, p.telefone);
                cout << "Historico medico: ";
                getline(cin, p.historicoMedico);
                raiz = inserir(raiz, p);
                cout << "\nPaciente inserido com sucesso.\n";
                cin.ignore();
                break;
            }
            case 2: {
                if (raiz == nullptr) {
                    cout << "Nenhum paciente foi cadastrado.\n";
                } else {
                    cout << "\nListagem de Pacientes:\n";
                    listarPacientes(raiz);
                }
                break;
            }
            case 3: {
                if (raiz == nullptr) {
                    cout << "Nenhum paciente foi cadastrado.\n";
                    break;
                }
                cout << "Digite o ID ou Nome do paciente a buscar: ";
                string chave;
                cin >> chave;
                NoAVL* paciente = nullptr;

                if (isdigit(chave[0])) {
                    int id = stoi(chave);
                    paciente = buscarPorID(raiz, id);
                } else {
                    paciente = buscarPorNome(raiz, chave);
                }

                if (paciente) {
                    cout << "Paciente encontrado:\n";
                    cout << "ID: " << paciente->paciente.id << endl;
                    cout << "Nome: " << paciente->paciente.nome << endl;
                    cout << "Nivel de urgencia: " << paciente->paciente.nivelUrgencia << endl;
                    cout << "Telefone: " << paciente->paciente.telefone << endl;
                    cout << "Historico Medico: " << paciente->paciente.historicoMedico << "\n" << endl;
                } else {
                    cout << "Paciente nao encontrado.\n";
                }
                break;
            }
            case 4: {
                if (raiz == nullptr) {
                    cout << "Nenhum paciente foi cadastrado.\n";
                    break;
                }
                cout << "Digite o ID ou Nome do paciente: ";
                string chave;
                cin >> chave;
                int novoniveldeurgencia;
                do {
                    cout << "Digite o novo nivel de urgencia (1 a 5): ";
                    cin >> novoniveldeurgencia;
                    if (novoniveldeurgencia < 1 || novoniveldeurgencia > 5) {
                        cout << "Nivel de urgencia invalido. Deve ser entre 1 e 5.\n";
                    }
                } while (novoniveldeurgencia < 1 || novoniveldeurgencia > 5);

                int id;
                if (isdigit(chave[0])) {
                    id = stoi(chave);
                } else {
                    if (mapNomeParaId.find(chave) == mapNomeParaId.end()) {
                        cout << "Paciente nao encontrado.\n";
                        break;
                    }
                    id = mapNomeParaId[chave];
                }

                NoAVL* paciente = buscarPorID(raiz, id);
                if (paciente) {
                    Paciente dados = paciente->paciente;
                    raiz = remover(raiz, id);
                    dados.nivelUrgencia = novoniveldeurgencia;
                    raiz = inserir(raiz, dados);
                    cout << "Ordem de urgencia foi alterada com sucesso.\n";
                } else {
                    cout << "Nao existe paciente para alterar nivel de urgencia.\n";
                }
                break;
            }
            case 5: {
                if (raiz == nullptr) {
                    cout << "Nenhum paciente foi cadastrado.\n";
                    break;
                }
                cout << "Digite o ID ou Nome do paciente para remover: ";
                string chave;
                cin >> chave;
                int id;
                if (isdigit(chave[0])) {
                    id = stoi(chave);
                } else {
                    if (mapNomeParaId.find(chave) == mapNomeParaId.end()) {
                        cout << "Paciente nao encontrado.\n";
                        break;
                    }
                    id = mapNomeParaId[chave];
                }

                NoAVL* paciente = buscarPorID(raiz, id);
                if (paciente) {
                    raiz = remover(raiz, id);
                    cout << "Paciente removido com sucesso.\n";
                } else {
                    cout << "Paciente nao encontrado para remocao.\n";
                }
                break;
            }
            case 0:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opcao invalida!\n";
        }

        // Limpeza da tela após pressionar Enter
        cout << "Pressione Enter para continuar...";
        cin.ignore();
        cin.get();  // Aguarda o usuário pressionar Enter
        clear();    // Limpa a tela após interação
    } while (opcao != 0);
}

int main() { // Função principal que inicia o programa
    NoAVL* raiz = nullptr;
    menu(raiz);
    return 0;
}
