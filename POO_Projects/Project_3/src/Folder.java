/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */
public class Folder extends File{
    private String name;
    
    /**
     *
     * @param name = numele asociat Folder-ului
     */
    public Folder(String name){
        this.name=name;
    }

    /**
     *
     * @return = returneaza numele Folder-ului
     */
    public String getName() {
        return name;
    }
    
}
