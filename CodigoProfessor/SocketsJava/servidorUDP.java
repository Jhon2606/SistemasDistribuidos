import java.io.*;
import java.net.*;

class servidorUDP {
	public static void main(String args[]) throws Exception {

		int porta = 40001;
		int numConn = 1;
		
		DatagramSocket serverSocket = new DatagramSocket(porta);

		byte[] receiveData = new byte[1024];
		byte[] sendData = new byte[1024];

		System.out.println("Esperando por datagrama UDP na porta " + porta);
		while (true) {

			DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
			serverSocket.receive(receivePacket);
			System.out.print("Datagrama UDP [" + numConn + "] recebido...");

			String sentence = new String(receivePacket.getData());
			System.out.println(sentence);
		}
	}
}