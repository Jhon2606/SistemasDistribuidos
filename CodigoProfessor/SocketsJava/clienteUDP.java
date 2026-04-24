import java.io.*;
import java.net.*;

class clienteUDP {
	public static void main(String args[]) throws Exception {

		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));

		DatagramSocket clientSocket = new DatagramSocket();

		String servidor = "localhost";
		int porta = 40001;

		InetAddress IPAddress = InetAddress.getByName(servidor);

		byte[] sendData = new byte[1024];
		byte[] receiveData = new byte[1024];

		while (true) {
			System.out.println("Digite o texto a ser enviado ao servidor: ");
			String sentence = inFromUser.readLine();
			sendData = sentence.getBytes();
			DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, porta);

			if (sentence.trim().isEmpty())
				break;

			System.out.println("Enviando pacote UDP para " + servidor + ":" + porta);
			clientSocket.send(sendPacket);
		}	
		clientSocket.close();
		System.out.println("Socket cliente fechado!");
	}
}