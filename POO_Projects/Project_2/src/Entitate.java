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
public class Entitate implements Cloneable{
    private String numeEnt;
    private int RF;
    private int nrAtr;
    private ArrayList atr;
    private long timestamp;
    
    /**
     *
     * @param parts = vector care contine numele entitatii, factorul de replicare numele atributelor si tipul acestora
     */
    public Entitate(String[] parts) {
        this.numeEnt=parts[1];
        this.RF = Integer.parseInt(parts[2]);
        this.nrAtr = Integer.parseInt(parts[3]);
        atr=new ArrayList(2*nrAtr);
        for(int i=0; i<2*nrAtr; i++){
            atr.add(parts[4+i]);
        }
        this.timestamp=System.nanoTime();
    }
    
    public Object clone(){  
        try {  
            return super.clone();
        } catch (CloneNotSupportedException ex) {
            System.out.println("EROARE");
            return null;
        }
   }
    
    /**
     *
     * @return numele entitatii
     */
    public String getNumeEnt() {
        return numeEnt;
    }

    /**
     *
     * @return factorul de multiplicitate
     */
    public int getRF() {
        return RF;
    }

    /**
     *
     * @return numarul de atribute
     */
    public int getNrAtr() {
        return nrAtr;
    }

    /**
     *
     * @return lista atributelor
     */
    public ArrayList getAtr() {
        return atr;
    }

    /**
     *
     * @param atr = seteaza lista atributelor
     */
    public void setAtr(ArrayList atr) {
        this.atr = atr;
    }
    
    /**
     *
     * @return timestampul ultimei modificari
     */
    public long getTimestamp() {
        return timestamp;
    }
    
    /**
     *  actualizeaza timestamp-ul instantei
     */
    public void Update(){
        this.timestamp=System.nanoTime();
    }
    
    
}
