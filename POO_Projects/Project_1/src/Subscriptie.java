/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */
public abstract class Subscriptie {
    private String nume;

    /**
     *
     * @param nume = numele obiectului
     */
    protected Subscriptie(String nume) {
        this.nume = nume;
    }

    /**
     *
     * @return intoarce numele obiectului
     */
    public String getNume() {
        return nume;
    }
    
    /**
     *
     * @return intoarce tipul obiectului, pe lant de mostenire
     */
    public abstract String Tip();

    /**
     *  Metoda care decrementeaza numarul de cereri
     */
    public abstract void Scade();
    
    /**
     *
     * @return intoarce 0 pentru numarul de cereri basic
     */
    public int getReq_b(){
        return 0;
    }
    
    /**
     *
     * @return intoarce 0 pentru numarul de cereri premium
     */
    public int getReq_p(){
        return 0;
    }
}
