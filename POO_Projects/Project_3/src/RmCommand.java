/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



/**
 *
 * @author alex
 * clasa care se ocupa cu executia comenzii Rm
 */
public class RmCommand implements Command{
    
    private Tree virtual;
    private String[] path;
    //private String name;
    private String errmes=null;

    /**
     *
     * @param root = directortul root "/"
     * @param parts = sir de string-uri  care contine argumentele comenzii
     */
    public RmCommand(Tree root, String[] parts){
        if(parts[1].charAt(0)=='/'){
            virtual=root;
        }
        else{
            virtual=Tree.getCurrent();
        }
        errmes="rm: cannot remove "+parts[1]+": No such file or directory";
        path=parts[1].split("/");
    }
    
    /**
     * Functia cauta path-ul pana la directorul dat ca argument si il sterge din arbore (cu tot cu continutul acestuia)
     */
    public void execute(){
        int check=0;
        for(String file: path){
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
                if((content.getFile().getName().equals(file))){
                    virtual=content;
                    check=1;
                    break;
                }
            }
            if(check==0){
                break;
            }
        }
        if(check==0){
            //System.out.println(errmes);
            Main.printErr(errmes+"\n");
            
            return;
        }
        if(check==1){
            int check1=1;
            Tree test=Tree.getCurrent();
            while(test!=test.getParent()){
                if(test==virtual){
                    check1=0;
                    break;
                }
                test=test.getParent();                   
            }
            if(check1==0){
                return;
            }
            virtual.getParent().getNodelist().remove(virtual);
            
        }
    }
}
