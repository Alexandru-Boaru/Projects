/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */
public class Basic extends Free{
    private int req_b=0;

    /**
     *
     * @param nume = numele obiectului
     * @param req_b = numarul de cereri basic
     */
    public Basic(String nume, int req_b) {
        super(nume);
        this.req_b = req_b;
    }

    public int getReq_b() {
        return req_b;
    }
    
    public void Scade(){
        if(this.req_b==0){
            super.Scade();
        }
        else{
            this.req_b--;
        }
    }
    
    public String Tip(){
        if(this.req_b==0){
            return super.Tip();
        }
        else{
            return "Basic";
        }
    }
    
}
