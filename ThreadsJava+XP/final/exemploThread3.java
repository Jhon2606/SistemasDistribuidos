public class exemploThread3 implements Runnable {

    public static final int SIZE = 100000000;
    private static int numThreads;
    private long start;
    private long end;
    private static double[] array = new double[SIZE];
    private static double global_sum = 0.0;
    private static double[] partial_sums;

    public exemploThread3(long st, long e) {
        this.start = st;
        this.end = e;
    }

    //SEM THREAD
    private static double sum_single() {
        double sum = 0.0;
        for (int i = 0; i < SIZE; i++) {
            sum += array[i];
        }
        return sum;
    }

    public static void main(String[] args) throws InterruptedException {

        numThreads = Integer.parseInt(args[0]);

        for (int i = 0; i < SIZE; i++) {
            array[i] = 1.0;
        }

        //SEM THREAD
        long start = System.currentTimeMillis();
        double result_single = sum_single();
        long end = System.currentTimeMillis();

        double time_single = (double)(end - start);
        System.out.println("Sem thread: soma = " + result_single + ", tempo = " + time_single);

        //COM THREAD (implements Runnable)
        long chunk = SIZE / numThreads;
        Thread[] threads = new Thread[numThreads];
        partial_sums = new double[numThreads];

        global_sum = 0.0;
        start = System.currentTimeMillis();

        for (int i = 0; i < numThreads; i++) {
            long st = i * chunk;
            long e  = (i == numThreads - 1) ? SIZE : (i + 1) * chunk;

            exemploThread3 task = new exemploThread3(st, e);
            threads[i] = new Thread(task);
            threads[i].start();
        }

        //O ganho de desempenho com threads existe mas é menor que o ideal (reducao de +-75% com 4 threads). Por causa do Overhead, Mémoria compartilhada e gargalo de memoria.
        for (int i = 0; i < numThreads; i++) {
            threads[i].join();
            global_sum += partial_sums[i];
        }

        end = System.currentTimeMillis();
        double time_thread = (double)(end - start);
        System.out.println("Com thread: soma = " + global_sum + ", tempo = " + time_thread);
    }

    @Override
    public void run() {
        System.out.println("Start = " + this.start + "     End = " + this.end);
        double local_sum = 0.0;
        for (long i = this.start; i < this.end; i++) {
            local_sum += array[(int) i];
        }
        // Guarda resultado parcial (indice baseado no start)
        int index = (int)(this.start / (SIZE / numThreads));
        partial_sums[index] = local_sum;
    }
}
