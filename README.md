Sistema de Gerenciamento de Hospitais

Descrição
Sistema que permite o cadastro, busca e remoção de pacientes em um hospital, utilizando uma árvore AVL para organizar os pacientes por prioridade. O sistema também permite alteração de prioridade e listagem de pacientes.
Funcionamento
Inicialmente, o usuário terá acesso a um menu com as seguintes opções:
1.	Inserir paciente: O usuário pode cadastrar um paciente informando ID, prioridade (de 1 a 5), nome, telefone e histórico médico.
2.	Listar pacientes: Exibe a lista de pacientes cadastrados, ordenados por prioridade.
3.	Buscar paciente por ID ou Nome: O usuário pode buscar um paciente informando seu ID ou nome.
4.	Alterar prioridade de paciente: Permite alterar a prioridade de um paciente existente.
5.	Remover paciente: Remove um paciente do sistema pelo seu ID ou nome.
6.	Sair: Encerra o programa.

Requisitos
Antes de executar o projeto, verifique se possui os seguintes requisitos:
•	Sistema Operacional: Windows, Linux ou macOS
•	Compilador C++: g++ (parte do GCC) ou equivalente
•	Git (opcional, para clonar o repositório)

Como Executar o Projeto

1. Compile o arquivo principal
g++ main.cpp -o pacientes

2. Execute o arquivo compilado
No Windows:
pacientes.exe
No Linux/macOS:
./pacientes
O sistema estará pronto para uso, permitindo a gestão eficiente dos pacientes cadastrados no hospital.

Autores
•	Gabriel Henrique Silva
•	João Bruno Faria
•	Rafael Oliveira
