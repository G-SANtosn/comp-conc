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
 * @Código: Modelando um buffer em Java
 */

class Buffer {
    private String[] info;                  //A informação
    private int tamanho = 100;              //Tamanho do buffer.
    private int preenchidos;                //Número de posições preenchidas
    private boolean prodBloq;               //Se há produtores bloqueados
    private boolean consuBloq;              //Se há consumidores bloqueados
    
    
    /**
     * Construtor padrão.
     */
    public Buffer() {
        info = new String[tamanho];
        preenchidos = 0;
        prodBloq = false;
        consuBloq = true;
    }
    
    /**
     * Construtor alternativo.
     * Usuário pode determinar o tamanho do buffer.
     */
    public Buffer(int tam) {
        this.tamanho = tam;
        info = new String[tam];
        preenchidos = 0;
        prodBloq = false;
        consuBloq = true;
    }
    
    /**
     * Imprime o buffer.
     */
    void imprime() {
        System.out.print("[");
        
        for (int i = 0; i < tamanho; i++) {
            System.out.print(info[i]);
            
            if(i < tamanho - 1) {
                System.out.print(", ");
                
                if( ((i + 1)%10 == 0) && (i != 0)) {
                    System.out.println("");
                }
                
            }
        }
        System.out.println("]");
    }
    
    /**
     * Insere um texto no buffer.
     * @param texto Texto a ser inserido no buffer.
     * @param pos   Posição onde o texto vai ser inserido.
     * @param id    Identificador da thread.
     */
    public synchronized void insere(String texto, int pos, int id) {
        try {
            //Se o buffer estiver cheio, o produtor é bloqueado.
            while (this.cheio()) {
                System.out.println("Buffer cheio. Produtor " + id + " bloqueado.");
                prodBloq = true;
                wait();
            }
            
            //Se a posição estiver cheia, o produtor é bloqueado.
            while (info[pos] != null) {
                System.out.println("Produtor " + id + " bloqueado em " + pos + ".");
                prodBloq = true;
                wait();
            }
            
            info[pos] = texto;
            preenchidos++;
            System.out.println("Texto " + texto + " inserido em " + pos + " por produtor " + id + ".");
            
            //Se há consumidores bloquedos, eles são liberados.
            if (consuBloq) {
                notifyAll();
                consuBloq = false;
                System.out.println("Consumidores liberados.");
            }
            
        } catch(InterruptedException e) {
            return;
        }
    }
    
    /**
     * Remove informação do buffer.
     * @param pos Posição de onde a informação vai ser removida.
     * @param id  Identificador de thread.
     */
    public synchronized void remove(int pos, int id) {
        try {
            //Se o buffer estiver vazio, o consumidor é bloqueado.
            while (this.vazio()) {
                System.out.println("Buffer vazio. Consumidor " + id + " bloqueado.");
                consuBloq = true;
                wait();
            }
            
            //Se a posição estiver vazio, o consumidor é bloqueado.
            while (info[pos] == null) {
                System.out.println("Consumidor " + id + " bloqueado em " + pos + ".");
                consuBloq = true;
                wait();
            }
                        
            info[pos] = null;
            preenchidos--;
            System.out.println("Texto removido de " + pos + " por consumidor " + id + ".");
            
            //Se há produtores bloqueados, eles são liberados.
            if (prodBloq) {
                notifyAll();
                prodBloq = false;
                System.out.println("Produtores liberados.");
            }
            
        } catch(InterruptedException e) {
            return;
        }
    }
    
    /**
     * Função que retorna o tamanho do buffer.
     * @return O tamanho do buffer.
     */
    public int getTamanho() {
        return this.tamanho;
    }
    
    /**
     * Função que diz se o buffer está cheio.
     * @return 
     */
    public boolean cheio() {
        return (preenchidos == tamanho);
    }
    
    /**
     * Função que diz se o buffer está vazio.
     * @return 
     */
    public boolean vazio() {
        return (preenchidos == 0);
    }
    
}
public class lab7ativ1 {
    
}
