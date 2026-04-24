import java.io.*;
import java.net.*;

class clienteTCP {
	public static void main(String args[]) throws Exception {

		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));

		Socket clientSocket = new Socket();

		String servidor = "localhost";
		int porta = 40002;

		try {
			Socket socket = new Socket(servidor, porta);
            PrintStream saida = new PrintStream(socket.getOutputStream());
			
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
			clientSocket.close();
			System.out.println("Socket cliente fechado!");

        } catch (IOException e) {
            e.printStackTrace();
        }
	}
}