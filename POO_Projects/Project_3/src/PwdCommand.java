/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.ArrayList;



/**
 *
 * @author alex
 * clasa care se ocupa cu executia comenzii Pwd
 */
public class PwdCommand implements Command{
    
    private Tree virtual;
    private ArrayList<String> path;
    private int n;

    /**
     *
     * @param current = directorul curent
     * @param n = intreg folosit pentru a determina daca printarea se face in output sau in error
     */
    public PwdCommand(Tree current, int n){
        //virtual=Tree.getCurrent();
        virtual=current;
        path= new ArrayList<String>();
        this.n=n;
    }
    
    /**
     * Functia cauta path-ul pana la directorul si apoi printeaza calea absoluta a acestuia
     */
    public void execute(){
        do{
            path.add(0,virtual.getFile().getName());
            virtual=virtual.getParent();
        }while(virtual!=virtual.getParent());
        if(path.size()==1 && path.get(0).equals("/")){
            //System.out.print("/");
            if(n==0){
                Main.printOut("/");
            }
            else{
                Main.printErr("/");
            }
        }
        else{
            for(String file: path){
                //System.out.print("/"+file);
                if(n==0){
                    Main.printOut("/"+file);
                
                }
                else{
                    Main.printErr("/"+file);
                }
            }
        }
    }
}
