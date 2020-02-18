/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */
public class Premium extends Basic{
    private int req_p=0;

    /**
     *
     * @param nume = numele obiectului
     * @param req_b = numarul de cereri de tip basic
     * @param req_p = numarul de cereri de tip premium
     */
    public Premium(String nume, int req_b, int req_p) {
        super(nume, req_b);
        this.req_p = req_p;
    } 

    public int getReq_p() {
        return req_p;
    }
    
    
    
    public void Scade(){
        if(this.req_p==0){
            super.Scade();
        }
        else{
            this.req_p--;
        }
    }
    
    public String Tip(){
        if(this.req_p==0){
            return super.Tip();
        }
        else{
            return "Premium";
        }
    }
    
}
