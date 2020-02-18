/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */




/**
 *
 * @author alex
 * clasa care se ocupa cu executia comenzii Ls
 */
public class LsCommand implements Command{
    
    private Tree virtual;
    private String[] path;
    private int checkR=0;
    private String errmes=null;

    /**
     *
     * @param root = directortul root "/"
     * @param parts = sir de string-uri  care contine argumentele comenzii
     */
    public LsCommand(Tree root, String[] parts){
        if(parts.length==1){
            virtual=root;
        }
        else if(parts.length==2){
            if(parts[1].equals("-R")){
                virtual=root;
                checkR=1;
            }
            else{
                if(parts[1].charAt(0)=='/'){
                    virtual=root;
                }
                else{
                    virtual=Tree.getCurrent();
                }
                path=parts[1].split("/");
                errmes="ls: "+parts[1]+": No such directory";
            }
        }
        else if(parts.length==3){
            checkR=1;
            if(parts[1].charAt(0)=='/' || parts[2].charAt(0)=='/'){
                virtual=root;
            }
            else{
                virtual=Tree.getCurrent();
            }
            if(!parts[1].equals("-R")){               
                path=parts[1].split("/");
                errmes="ls: "+parts[1]+": No such directory";
            }
            else{                
                path=parts[2].split("/");
                errmes="ls: "+parts[2]+": No such directory";
            }
        }
    }
    
    /**
     * Functia cauta path-ul pana la directorul dat ca argument si listeaza continutul acestuia
     */
    public void execute(){
        int check=0;
        if(path!=null && path.length!=0){
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
            if(this.checkR==0){
                virtual.print();
            }
            else{
                virtual.printR();
            }
        }
    }
}
