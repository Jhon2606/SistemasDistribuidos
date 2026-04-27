import java.io.*;
import java.net.*;

public class servidorTCP {
    public static void main(String[] args) throws Exception {

        int porta = 40002;

        ServerSocket serverSocket = new ServerSocket(porta, 1);// aceita 1 cliente por vez

        while (!serverSocket.isClosed()) {
            System.out.println("Aguardando conexões...");
            Socket conexao = serverSocket.accept();// O programa para aqui e espera até um cliente se conectar.
            System.out.println("Cliente Conectou!");
            // conversao da rede para o qeu o SO entende e dá pra colocar no buffer
            BufferedReader entrada = new BufferedReader(new InputStreamReader(conexao.getInputStream()));
            PrintStream saida = new PrintStream(conexao.getOutputStream());
            String linha = entrada.readLine();
            while (linha != null && !(linha.trim().equals(""))) {
                System.out.println(linha);
                saida.println("Servidor confirma recebimento de: " + linha);
                linha = entrada.readLine();
            }
            conexao.close();
            System.out.println("Socket cliente fechado.");
        }

    }
}
