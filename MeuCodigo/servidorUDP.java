import java.io.*;
import java.net.*;

public class servidorUDP {
    // throws Exception significa que o método não trata erros localmente
    public static void main(String[] args) throws Exception {
        int porta = 40001;
        int numConn = 0;// contador de datagramas recebidos (mensagens)
        // Ao passar a porta no construtor, o socket fica vinculado (bind) àquela porta
        DatagramSocket serverSocket = new DatagramSocket(porta);

        byte[] receiveData = new byte[1024];
        byte[] sendData = new byte[1024];
        System.out.println("Esperando por datagrama UDP na porta " + porta);

        while (true) {
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            // A thread fica parada aqui até que um datagrama UDP chegue na porta em questão
            // o metodo recive preeenche os dados, o IP de origem e a porta de origem do
            // cliente
            serverSocket.receive(receivePacket);
            numConn++;
            System.out.print("Datagrama UDP [" + numConn + "] recebido...");

            String sentence = new String(receivePacket.getData()); // converte esses bytes em texto legível
            System.out.println(sentence);
        }
    }
}
/*
CLIENTE UDP                          SERVIDOR UDP
    |                                     |
    |  [cria DatagramSocket]              |  [DatagramSocket na porta 40001]
    |                                     |
    |  [monta DatagramPacket]             |  [entra no while(true)]
    |                                     |
    |-------- datagrama UDP ------------>|  serverSocket.receive() desbloqueia
    |                                     |
    |                                     |  converte bytes → String
    |                                     |  imprime no console
    |                                     |
    |                                     |  volta para receive() e BLOQUEIA
*/