/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.lab06;

/**
 *
 * @author Gabriel Santos de Araujo do Nascimento
 * @Disciplina Computação Concorrent
 * @Prof.: Silvana Rossetto
 * Módulo 2 - Laboratório 6
 * @Código: Soma dois vetores usando threads em Java
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
    
    public SomaVetores(Vetor a, Vetor b, Vetor c, int tam) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.tamanho = tam;
    }
    
        @Override
    public void run() {
        for (int i = 0; i < tamanho; i++) {
            c.alteraElem(i, a.getElem(i) + b.getElem(i));
        }
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
        c = new Vetor(tamanho);
        
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
            threads[i] = new SomaVetores(a, b, c, tamanho);
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
