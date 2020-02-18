/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */
public class Free extends Subscriptie {
    
    /**
     *
     * @param nume = numele obiectului
     */
    public Free(String nume) {
        super(nume);
    }
    
    /**
     * Metoda care decrementeaza numarul de cereri
     */
    public void Scade(){
        
    }
    
    /**
     *
     * @return intoarce tipul obiectului in functie de numarul de cereri, pe lantul mostenirilor
     */
    public String Tip(){
        return "Free";
    }

}
