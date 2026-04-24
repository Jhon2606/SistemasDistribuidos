import java.io.*;
import java.net.*;

class servidorTCP {
	public static void main(String args[]) throws Exception {

		int porta = 40002;
		int numConn = 1;
		
		ServerSocket serverSocket = new ServerSocket(porta, 1);
        
		while (!serverSocket.isClosed()) {
			System.out.println("Aguardando conexões!");
            Socket conexao = serverSocket.accept();
            System.out.println("Cliente Conectou!");

			try {
				// obtendo os objetos de controle do fluxo de comunicação
				BufferedReader entrada = new BufferedReader(new InputStreamReader(conexao.getInputStream()));
//				PrintStream saida = new	PrintStream(conexao.getOutputStream());
				String linha = entrada.readLine();
				while (linha != null && !(linha.trim().equals(""))) {
					System.out.println(linha);
					// espera por uma nova linha.
					linha = entrada.readLine();
				}
			} catch (IOException e) {
				// caso ocorra alguma excessão de E/S, mostre qual foi
				System.out.println("IOException: " + e);
			}
        }
	}
}