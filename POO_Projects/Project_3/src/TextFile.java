/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */
public class TextFile extends File{
    private String name;
    
    /**
     *
     * @param name = nume asociat TextFile-ului
     */
    public TextFile(String name){
        this.name=name;
    }

    /**
     *
     * @return = returneaza numele TextFile-ului
     */
    public String getName() {
        return name;
    }
    
    
}
