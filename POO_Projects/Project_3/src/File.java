/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author alex
 * clasa parinte a claselor de tip Folder si TextFile
 */
public class File implements Cloneable{

    /**
     *
     * @return = se va reurna un File de tip Folder sau TextFile (se foloseste FileFactory)
     * 
     */
    

    public String getName() {
        if(this instanceof Folder){
            return ((Folder)this).getName();
        }
        else{
            return ((TextFile)this).getName();
        }
    }
    
    public Object clone(){  
        try {  
            return super.clone();
        } catch (CloneNotSupportedException ex) {
            System.out.println("EROARE");
            return null;
        }
   }
    
}
