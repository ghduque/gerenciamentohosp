# Sistema de Gerenciamento de Hospitais / Hospital Management System 

---
# Descrição / Description

Sistema que permite o cadastro, busca e remoção de pacientes em um hospital, utilizando uma árvore AVL para organizar os pacientes por prioridade. O sistema também permite alteração de prioridade e listagem de pacientes.
A system that allows the registration, search, and removal of patients in a hospital, using an AVL tree to organize patients by priority. The system also allows priority modification and patient listing.

---

# Funcionamento / How it works

Inicialmente, o usuário terá acesso a um menu com as seguintes opções:
Initially, the user will have access to a menu with the following options:

1. **Inserir paciente:** O usuário pode cadastrar um paciente informando ID, prioridade (de 1 a 5), nome, telefone e histórico médico.
   **Insert patient:** The user can register a patient by providing ID, priority (from 1 to 5), name, phone number, and medical history.

2. **Listar pacientes:** Exibe a lista de pacientes cadastrados, ordenados por prioridade.
   **List patients:** Displays the list of registered patients, ordered by priority.

3. **Buscar paciente por ID ou Nome:** O usuário pode buscar um paciente informando seu ID ou nome.
   **Search patient by ID or Name:** The user can search for a patient by providing their ID or name.

4. **Alterar prioridade de paciente:** Permite alterar a prioridade de um paciente existente.
   **Change patient priority:** Allows changing the priority of an existing patient.

5. **Remover paciente:** Remove um paciente do sistema pelo seu ID ou nome.
   **Remove patient:** Removes a patient from the system by their ID or name.

6. **Sair:** Encerra o programa.
   **Exit:** Ends the program.

---

# Requisitos / Requirements

Antes de executar o projeto, verifique se possui os seguintes requisitos:
Before running the project, make sure you have the following requirements:

* Sistema Operacional: Windows, Linux ou macOS
  Operating System: Windows, Linux, or macOS

* Compilador C++: g++ (parte do GCC) ou equivalente
  C++ Compiler: g++ (part of GCC) or equivalent

* Git (opcional, para clonar o repositório)
  Git (optional, for cloning the repository)

---

# Como Executar o Projeto / How to Run the Project

1. Compile o arquivo principal
   Compile the main file

   ```bash
   g++ main.cpp -o pacientes
   ```

2. Execute o arquivo compilado
   Run the compiled file

   No Windows:
   On Windows:

   ```bash
   pacientes.exe
   ```

   No Linux/macOS:
   On Linux/macOS:

   ```bash
   ./pacientes
   ```

O sistema estará pronto para uso, permitindo a gestão eficiente dos pacientes cadastrados no hospital.
The system will be ready to use, allowing efficient management of registered patients in the hospital.

---

# Autores / Authors

* Gabriel Henrique Silva
* João Bruno Faria
* Rafael Oliveira

