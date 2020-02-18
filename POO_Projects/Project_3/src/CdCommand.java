/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author alex
 * clasa care se ocupa cu executia comenzii Cd
 */
public class CdCommand implements Command{
    private Tree virtual;
    private String[] path;
    private String errmes;

    /**
     * 
     * @param root = directortul root "/"
     * @param parts = sir de string-uri  care contine argumentele comenzii
     */
    public CdCommand(Tree root, String[] parts){
        if(parts[1].charAt(0)=='/'){
            virtual=root;
        }
        else{
            virtual=Tree.getCurrent();
        }
        path=parts[1].split("/");
        errmes="cd: "+parts[1]+": No such directory";
        
    }

    /**
     * Functia cauta path-ul pana la directorul dat ca argument si se seteaza ca noul director curent
     */
    public void execute(){
        int check=0;
        if(path.length!=0){
        for(String file : path){
            check=0;
            if(file.equals("..")){
                if(virtual==virtual.getParent()){
                    break;
                }
                else{
                    virtual=virtual.getParent();
                    check=1;
                    continue;
                }
            }
            else if(file.equals(".") || file.equals("")){
                check=1;
                continue;
            }
            for(Tree content: virtual.getNodelist()){
                if((content.getFile().getName().equals(file)) && content.getFile() instanceof Folder){
                    virtual=content;
                    check=1;
                    break;
                }
            }
            if(check==0){
                break;
            }            
        }
        }
        else{
            check=1;
        }
        if(check==0){
            //System.out.println(errmes);
            Main.printErr(errmes+"\n");
            return;
        }
        if(check==1){
            Tree.setCurrent(virtual);
        }
    }
}
