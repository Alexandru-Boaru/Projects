/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 * o clasa Factory care se ocupa de crearea obiectelor de tip Folder si TextFile
 */
public class FileFactory {

    /**
     *
     * @param name = numele care va asociat obiectului File
     * @param type = intreg care indica ce tip de obiect File se foloseste (0-Folder,1-TextFile)
     * @return = se returneaza un obiect File
     */
    public static File getFile(String name, int type){
        if(type==0){
            return new Folder(name);
        }
        else if(type==1){
            return new TextFile(name);
        }
        return null;
    }
}
