# Sistemas Distribuídos

Repositório responsável por armazenar meus estudos e implementações da disciplina de Sistemas Distribuídos.

---

## Cliente UDP (Linux)

Este programa implementa um cliente UDP que:

- Recebe o IP ou nome do servidor via linha de comando
- Cria um socket UDP
- Entra em loop infinito:
  - Lê uma mensagem digitada pelo usuário
  - Envia essa mensagem para o servidor utilizando `sendto`

Observações:
- UDP é um protocolo **sem conexão** (diferente do TCP)
- Não há garantia de entrega, ordem ou recebimento
- O cliente não espera resposta do servidor

---

## Cliente UDP (Windows)

Versão do cliente UDP utilizando a API Winsock (Windows).

Características:

- Não solicita entrada do usuário
- Utiliza um IP de servidor fixo
- Envia apenas uma mensagem
- Necessita inicialização da biblioteca (`WSAStartup`)
- Finaliza com `WSACleanup`

---

## Servidor UDP

Este programa implementa um servidor UDP responsável por:

- Criar um socket UDP
- Associar (bind) o socket a uma porta específica
- Permanecer em execução contínua (loop infinito)
- Receber mensagens de clientes
- Exibir as mensagens recebidas no terminal

Observações:
- O servidor utiliza `read()` para receber dados (poderia usar `recvfrom`)
- Não envia respostas aos clientes