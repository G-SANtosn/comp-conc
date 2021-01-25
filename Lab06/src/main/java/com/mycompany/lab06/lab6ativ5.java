/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.lab06;

/**
 *
 * @author Gabriel
 */

class Vetor {
    private int vetor[];
    
    public Vetor(int tam) {
        vetor = new int[tam];
    }
    
    public void inicializa() {
        for (int i = 0; i < this.vetor.length; i++) {
            vetor[i] = 0;
        }
    }
    
    public void imprime() {
        String saida = "[";
        
        for (int i = 0; i < this.vetor.length; i++) {
            saida = saida + this.vetor[i];
            
            if(i != this.vetor.length - 1) {
                saida = saida + ", ";
            }
        }
        
        saida = saida + "]";
        
        System.out.println(saida);
    }
    
    public int getTamanho() {
        return this.vetor.length;
    }
    
    public int getElem(int pos) {
        return vetor[pos];
    }
    
    public void alteraElem(int pos, int valor) {
        this.vetor[pos] = valor;
    }
}

class SomaVetores extends Thread {
    public static Vetor a;
    public static Vetor b;
    public static Vetor c;
    int tamanho;
    private static boolean impresso = false;
    
    public SomaVetores(Vetor a, Vetor b, int tam) {
        this.a = a;
        this.b = b;
        this.tamanho = tam;
        c = new Vetor(tamanho);
    }
    
    public synchronized void imprimeC() {
        System.out.println("Vetor C:");
        c.imprime();
    }
    
    @Override
    public void run() {
        for (int i = 0; i < tamanho; i++) {
            c.alteraElem(i, a.getElem(i) + b.getElem(i));
        }
        
        lab6ativ5.c = this.c;
        
        /*
        if (!this.impresso) {
            System.out.println("Vetor C:");
            c.imprime();
            impresso = true;
            lab6ativ5.c = this.c;
        }
        */
    }
}

public class lab6ativ5 {
    public static Vetor a;
    public static Vetor b;
    public static Vetor c;
    public static final int N = 10;
    
    public static void main(String[] args) {
        Thread[] threads = new Thread[N];
        int tamanho = 10;
        
        a = new Vetor(tamanho);
        b = new Vetor(tamanho);
        
        for (int i = 0; i < 10; i++) {
            a.alteraElem(i, i*2);
        }
        
        for (int i = 0; i < 10; i++) {
            b.alteraElem(i, i*3);
        }
        
        System.out.println("Vetor A:");
        a.imprime();
        
        System.out.println("Vetor B:");
        b.imprime();
        
        for (int i = 0; i < N; i++) {
            threads[i] = new SomaVetores(a, b, tamanho);
        }
        
        for (int i = 0; i < N; i++) {
            threads[i].start();
        }
        
        for (int i = 0; i < N; i++) {
            try {
                threads[i].join();
            } catch(InterruptedException e) {
                return;
            }
        }
        
        System.out.println("Vetor C:");
        c.imprime();
        
    }
}
