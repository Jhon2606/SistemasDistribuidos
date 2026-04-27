import java.io.*;
import java.net.*;

public class clienteTCP {
    public static void main(String[] args) throws Exception {
        BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));

        String servidor = "localhost";
        int porta = 40002;

        Socket socket = new Socket(servidor, porta);
        // aqui se diferencia do UDP sendo orientedo a conexão
        PrintStream saida = new PrintStream(socket.getOutputStream());
        // socket.getOutputStream() -> obtém o canal de saída do socket (bytes que serão
        // enviados pela rede)
        // PrintStream -> encapsula esse canal, permitindo usar métodos como .println()
        // para enviar texto facilmente
        while (true) {
            System.out.print("Digite o texto a ser enviado ao servidor: ");
            String sentence = inFromUser.readLine();
            System.out.print(sentence);
            if (sentence.trim().isEmpty()) {
                saida.println("");
                break;
            }
            System.out.println("Enviando pacote TCP para " + servidor + ":" + porta);
            saida.println(sentence);
        }
        socket.close();
        System.out.println("Socket cliente fechado.");
    }
}
/*
INÍCIO
  │
  ├─ Cria leitor do teclado
  ├─ Define servidor e porta
  ├─ Abre conexão TCP (three-way handshake)
  ├─ Cria stream de saída
  │
  └─ LOOP ──────────────────────────────────┐
       │                                    │
       ├─ Pede input ao usuário             │
       ├─ Lê linha                          │
       ├─ [vazia?] ──SIM──► envia "" e BREAK│
       └─ Envia mensagem ao servidor ───────┘
  │
  ├─ Fecha socket
  └─ FIM
 */
