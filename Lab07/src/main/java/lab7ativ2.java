/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Gabriel Santos de Araujo do Nascimento
 * @Disciplina Computação Concorrent
 * @Prof.: Silvana Rossetto
 * Módulo 2 - Laboratório 7
 * @Código: Threads compartilhando um buffer em Java
 */
import java.util.Random;


/**
 * Threads produtoras.
 * @author Gabriel
 */
class Produtor extends Thread {
    int id;             //identificador da thread
    Buffer buffer;      //buffer que a thread modificará
    int inicio;
    int intervalo;
    int fim;
    
    /**
     * Construtor da classe Produtor.
     * @param id Identificador da threads.
     * @param nthreads Número de thread.
     * @param b Buffer que a thread vai percorrer.
     */
    public Produtor(int id, int nthreads, Buffer b) {
        this.id = id;
        this.buffer = b;
        this.intervalo = this.buffer.getTamanho()/nthreads;
        this.inicio = (this.id - 1)*intervalo;
        if(this.id < nthreads) {
            this.fim = this.inicio + this.intervalo;
        } else {
            fim = this.buffer.getTamanho();
        }
    }
    
    /**
    * Cria uma String de 5 dígitos aleatória.
    **/
    private static String fazTexto() {
        String texto = "";
        
        for (int i = 0; i < 5; i++) {
            texto += alfabeto();
        }
        
        return texto;
    }
    
    /**
    * Cria uma String aleatória com número de dígitos determinado pelo usuário.
    * @param tam Número de dígitos do texto.
    **/
    private static String fazTexto(int tam) {
        String texto = "";
        
        for (int i = 0; i < tam; i++) {
            texto += alfabeto();
        }
        
        return texto;
    }
    
    
    /**
    * Retorna uma letra aleatória.
    **/
    public static String alfabeto() {
        String[] letras = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
                            "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
                            "U", "V", "W", "X", "Y", "Z"};
        Random n = new Random();
        
        //Retorna aleatoriamente um elemento de letras.
        return letras[n.nextInt(letras.length)];
    }
    
    @Override
    public void run() {
        try {
            for (int i = inicio; i < fim; i++) {
                this.buffer.insere(fazTexto(), i, id);
            }            
            
            sleep(5);
        } catch(InterruptedException e) {
            return;
        }
    }
}

/**
 * Threads condumidoras.
 * @author Gabriel
 */

class Consumidor extends Thread {
    int id;             //identificador da thread
    Buffer buffer;      //buffer que a thread modificará
    int inicio, intervalo, fim;
    
    /**
     * Construtor da classe Consumidor.
     * @param id Identificador da thread.
     * @param nthreads Número de threads.
     * @param b Buffer que a thread vai percorrer.
     */
    public Consumidor(int id, int nthreads, Buffer b) {
        this.id = id;
        this.buffer = b;
        this.intervalo = this.buffer.getTamanho()/nthreads;
        this.inicio = (this.id - 1)*intervalo;
        if(this.id < nthreads) {
            this.fim = this.inicio + this.intervalo;
        } else {
            fim = this.buffer.getTamanho();
        }
    }
    
    @Override
    public void run() {
        try {
            for (int i = inicio; i < fim; i++) {
                this.buffer.remove(i, id);
            }            
            
            sleep(5);
        } catch(InterruptedException e) {
            return;
        }
    }
}

/**
 * Classe principal.
 * @author Gabriel
 */
public class lab7ativ2 {
    static final int C = 5;
    static final int P = 5;
    
    /**
     * Roda a atividade.
     * @param args
     * 
     */
    public static void main(String[] args) {
        rodaPrincipal();        
        
        //System.out.println("\n\n\n");
        //testaCasos();
    }
    
    /**
     * O programa principal, com um buffer de tamanho padrão,
     * e número padrão de produtores e consumidores.
     */
    public static void rodaPrincipal() {
        Buffer buffer = new Buffer();               //Buffer compartilhado
        Produtor[] prod = new Produtor[P];          //Threads produtoras
        Consumidor[] consu = new Consumidor[C];     //Threads consumidoras
        
        for (int i = 0; i < P; i++) {
            prod[i] = new Produtor(i + 1, P, buffer);
            prod[i].start();
        }
        
        for (int i = 0; i < C; i++) {
            consu[i] = new Consumidor(i + 1, C, buffer);
            consu[i].start();
        }
        
        for (int i = 0; i < P; i++) {
            try {
                prod[i].join();
            } catch(InterruptedException e) {
                return;
            }
        }
        
        for (int i = 0; i < C; i++) {
            try {
                consu[i].join();
            } catch(InterruptedException e) {
                return;
            }
        }
        
        buffer.imprime();
    }
    
    /**
     * Roda os 4 casos do enunciado.
     */
    public static void testaCasos() {
        //Caso A: 1 produtor e 1 consumidor.
        System.out.println("Caso A: 1 produtor e 1 consumidor.");
        casos(1, 1);
        System.out.println("\n\n\n");
        
        //Caso B: 1 produtor e vários consumidores.
        System.out.println("Caso B: 1 produtor e vários consumidores.");
        casos(1, C);
        System.out.println("\n\n\n");
        
        //Caso C: vários produtores e 1 consumidor.
        System.out.println("Caso C: vários produtores e 1 consumidor.");
        casos(P, 1);
        System.out.println("\n\n\n");
        
        //Caso D: vários produtores e vários consumidores.
        System.out.println("Caso D: vários produtores e vários consumidores.");
        casos(P, C);
    }
    
    /**
     * Testa casos diferentes, num buffer de tamanho 50.
     * @param numP Número de produtores.
     * @param numC Número de consumidores.
     */
    public static void casos(int numP, int numC) {
        casos(numP, numC, 50);
    }
    
    /**
     * Testa casos diferentes.
     * @param numP Número de produtores.
     * @param numC Número de consumidores.
     * @param tam Tamanho do buffer.
     */
    public static void casos(int numP, int numC, int tam) {
        Buffer buffer = new Buffer(50);                 //Buffer compartilhado
        Produtor[] prod = new Produtor[numP];           //Threads produtoras
        Consumidor[] consu = new Consumidor[numC];      //Threads consumidoras
        
        for (int i = 0; i < numP; i++) {
            prod[i] = new Produtor(i + 1, numP, buffer);
            prod[i].start();
        }
        
        for (int i = 0; i < numC; i++) {
            consu[i] = new Consumidor(i + 1, C, buffer);
            consu[i].start();
        }
        
        for (int i = 0; i < numP; i++) {
            try {
                prod[i].join();
            } catch(InterruptedException e) {
                return;
            }
        }
        
        for (int i = 0; i < numC; i++) {
            try {
                consu[i].join();
            } catch(InterruptedException e) {
                return;
            }
        }
        
        buffer.imprime();
    }
}
