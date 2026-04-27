import java.io.*;// fornece classes para leitura de entrada do usuário
import java.net.*;// fornece classes para comunicação de rede 

public class clienteUDP {
    public static void main(String[] args) throws Exception {
        // captura a entrada do tecaldo e guarda na variavel inFromUser
        BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
        // Cria o socket UDP do cliente
        DatagramSocket clientSocket = new DatagramSocket();

        String servidor = "localhost";
        int porta = 40001;
        // Converte localhost para um objeto InetAddress endereço IP que a rede Java
        // entende
        InetAddress IPAddress = InetAddress.getByName(servidor);
        byte[] sendData = new byte[1024]; // guarda os bytes a serem enviados
        byte[] receiveData = new byte[1024];// para receber dados

        while (true) {
            System.out.println("Digite o texto a ser enviado ao servidor: ");
            String sentence = inFromUser.readLine();
            // Converte a String digitada em um array de bytes, pois a rede trabalha com
            // bytes, não com texto diretamente
            sendData = sentence.getBytes();
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, porta);
            // Se o usuário pressionar Enter sem digitar nada, o loop é encerrado. O trim()
            // remove espaços em branco antes de verificar se está vazio.
            if (sentence.trim().isEmpty())
                break;

            System.out.println("Enviando pacote UDP para " + servidor + ":" + porta);
            clientSocket.send(sendPacket);
        }
        clientSocket.close();
        System.out.println("Socket cliente fechado.");
    }
}
