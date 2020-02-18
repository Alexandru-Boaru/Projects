/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.util.ArrayList;

/**
 *
 * @author alex
 */
public class Node {
    private int numarNod;
    private int contorNod;
    private int capNod;
    private ArrayList<Entitate> nodeList;

    /**
     *
     * @param numarNod = numarul nodului
     * @param capNod = capacitatea maxima a unui nod
     */
    public Node(int numarNod, int capNod) {
        this.numarNod = numarNod;
        this.capNod = capNod;
        this.nodeList = new ArrayList<Entitate>(capNod);
    }

    /**
     *
     * @return numarul curent de elemente din nod
     */
    public int getContorNod() {
        return contorNod;
    }

    /**
     *
     * @return lista cu instante a nodului
     */
    public ArrayList<Entitate> getNodeList() {
        return nodeList;
    }
    
    /**
     *
     * @param i = noua entitate ce urmeaza a fi introdusa la inceputul nodului 
     */
    public void Insert(Entitate i) {
        this.nodeList.add(0,i);
        this.contorNod++;
    }
    
    /**
     *  decrementeaza contorul numarului de instante din nod
     */
    public void Decrement(){
        this.contorNod--;
    }   
    
    /**
     *
     * @return verifica daca nodul este plin
     */
    public int Full(){
        if(this.contorNod==this.capNod){
            return 1;
        }
        return 0;
    }
}
