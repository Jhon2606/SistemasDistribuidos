public class MyTask implements Runnable {
    @Override
    public void run() {
        System.out.println("Thread is running...");
    }

    public static void main(String[] args) {
        MyTask task = new MyTask();
        Thread thread = new Thread(task); // Pass the task to a Thread object
        thread.start(); // Always use start() to begin execution
    }
}
