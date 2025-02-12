#include <iostream>
#include <string>
#include <unordered_map> TESTEEEEEEEEEEEE
#include <algorithm>
using namespace std;

struct Paciente { // Estrutura para armazenar informaçôes de um paciente
    int id;
    int prioridade;
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

int altura(NoAVL* no) { // Retorna a altura do nó
    if (no) {
        return no->altura;
    } else {
        return 0;
    }
}

int fatorBalanceamento(NoAVL* no) { // Calcula o fator de balanceamento de um nó 
    if (no) {
        return altura(no->esq) - altura(no->dir);
    } else {
        return 0;
    }
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
    if (paciente.prioridade < raiz->paciente.prioridade ||
        (paciente.prioridade == raiz->paciente.prioridade && paciente.id < raiz->paciente.id))
        raiz->esq = inserir(raiz->esq, paciente);
    else
        raiz->dir = inserir(raiz->dir, paciente);

    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
    return balancear(raiz);
}

NoAVL* buscarPorID(NoAVL* raiz, int id) { // Busca um paciente na árvore por ID
    if (!raiz) {
        return nullptr;
    }
    if (raiz->paciente.id == id) {
        return raiz;
    } else {
        NoAVL* encontrado = buscarPorID(raiz->esq, id);
        if (encontrado) {
            return encontrado;
        } else {
            return buscarPorID(raiz->dir, id);
        }
    }
}

NoAVL* buscarPorNome(NoAVL* raiz, const string& nome) { // Busca um paciente na árvore por nome
    if (!raiz){
        return nullptr;
    }
    if (raiz->paciente.nome == nome){
        return raiz;
    } else{
        NoAVL* encontrado = buscarPorNome(raiz->esq, nome);
        if (encontrado) {
            return encontrado;
        } else {
            return buscarPorNome(raiz->dir, nome);
        }
    }
}

NoAVL* buscarMinimo(NoAVL* raiz) { // Encontra o nó com menor ID na árvore
    while (raiz->esq) raiz = raiz->esq;
    return raiz;
}

NoAVL* remover(NoAVL* raiz, int id) { // Remove um paciente por ID
    if (!raiz){
        return raiz;
    }

    if (id < raiz->paciente.id)
        raiz->esq = remover(raiz->esq, id);
    else if (id > raiz->paciente.id)
        raiz->dir = remover(raiz->dir, id);
    else {
        if (!raiz->esq || !raiz->dir) {
            NoAVL* temp;
            if (raiz->esq) {
                temp = raiz->esq;
            } else {
                temp = raiz->dir;
            }
            if (!temp) {
                temp = raiz;
                raiz = nullptr;
            } else {
                *raiz = *temp;
            }
            delete temp;
        } else {
            NoAVL* temp = buscarMinimo(raiz->dir);
            raiz->paciente = temp->paciente;
            raiz->dir = remover(raiz->dir, temp->paciente.id);
        }
    }

    if (raiz){ 
        raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;
        return balancear(raiz);
    } else {
        return raiz;
    }
}

NoAVL* removerPaciente(NoAVL* raiz, const string& chave) { // Remove um paciente por ID ou nome
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

    NoAVL* temp = buscarPorID(raiz, id);
    if (temp) {
        mapNomeParaId.erase(temp->paciente.nome);
        mapIdParaNome.erase(id);
        raiz = remover(raiz, id);
    }

    cout << "Paciente removido com sucesso.\n";
    return raiz;
}

void alterarPrioridade(NoAVL*& raiz, const string& chave, int novaPrioridade) { // Altera a prioridade de um paciente 
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

    NoAVL* paciente = buscarPorID(raiz, id);
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

void listarPacientes(NoAVL* raiz) { // Lista todos os pacientes em ordem crescente por prioridade
    if (raiz){
        listarPacientes(raiz->esq);
        cout << "ID: " << raiz->paciente.id << endl;
        cout << "Nome: " << raiz->paciente.nome << endl;
        cout << "Prioridade: " << raiz->paciente.prioridade << endl;
        listarPacientes(raiz->dir);
    } else {
        return;
    }
}

void menu(NoAVL*& raiz) { // Função exibe o menu e interagir com usuário
    int opcao;
    do {
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
                do {
                    cout << "ID: ";
                    cin >> p.id;
                    if (p.id <= 0) {
                        cout << "ID inválido. Deve ser um número positivo.\n";
                    }
                } while (p.id <= 0);

                do {
                    cout << "Prioridade (1 a 5): ";
                    cin >> p.prioridade;
                    if (p.prioridade < 1 || p.prioridade > 5) {
                        cout << "Prioridade inválida. Deve ser entre 1 e 5.\n";
                    }
                } while (p.prioridade < 1 || p.prioridade > 5);

                cin.ignore();
                cout << "Nome: ";
                getline(cin, p.nome);
                cout << "Telefone: ";
                getline(cin, p.telefone);
                cout << "Historico medico: ";
                getline(cin, p.historicoMedico);
                raiz = inserir(raiz, p);
                cout << "\nPaciente inserido com sucesso.\n";
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
                    cout << "Prioridade: " << paciente->paciente.prioridade << endl;
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
                int novaPrioridade;
                do {
                    cout << "Digite a nova prioridade (1 a 5): ";
                    cin >> novaPrioridade;
                    if (novaPrioridade < 1 || novaPrioridade > 5) {
                        cout << "Prioridade inválida. Deve ser entre 1 e 5.\n";
                    }
                } while (novaPrioridade < 1 || novaPrioridade > 5);
                alterarPrioridade(raiz, chave, novaPrioridade);
                break;
            }
            case 5: {
                if (raiz == nullptr) {
                    cout << "Nenhum paciente foi cadastrado.\n";
                    break;
                }
                cout << "Digite o ID ou Nome do paciente a remover: ";
                string chave;
                cin >> chave;
                raiz = removerPaciente(raiz, chave);
                break;
            }
            case 0:
                cout << "Saindo...\n";
                break;
            default:
                cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
    } while (opcao != 0);
}

int main() { // Função principal que inicia o programa 
    NoAVL* raiz = nullptr;
    menu(raiz);
    return 0;
}
