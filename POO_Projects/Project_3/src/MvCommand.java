/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author alex
 * clasa care se ocupa cu executia comenzii Mv
 */
public class MvCommand implements Command{
    private Tree virtual;
    private Tree virtual1;
    private String[] destination;
    private String[] source;
    private String errmes1=null;
    private String errmes2=null;
    private String errmes3=null;
    
    /**
     *
     * @param root = directortul root "/"
     * @param parts = sir de string-uri  care contine argumentele comenzii
     */
    public MvCommand(Tree root, String[] parts){
        if(parts[1].charAt(0)=='/'){
            virtual=root;
        }
        else{
            virtual=Tree.getCurrent();
        }
        if(parts[2].charAt(0)=='/'){
            virtual1=root;
        }
        else{
            virtual1=Tree.getCurrent();
        }
        
        this.source=parts[1].split("/");
        this.destination=parts[2].split("/");
        errmes1="mv: cannot move "+parts[1]+": No such file or directory";
        errmes2="mv: cannot move into "+parts[2]+": No such directory";
        errmes3="mv: cannot move "+parts[1]+": Node exists at destination";
        
    }

    /**
     * Functia cauta path-ul pana la directorul/fisierul sursa si muta continutul acestuia in directorul destinatie
     */
    public void execute(){
        int check=0;
        for(String file: source){
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
            //System.out.println(errmes1);
            Main.printErr(errmes1+"\n");
            
            return;
        }
        for(String file: destination){
        check=0;           
            if(file.equals("..")){
                if(virtual1==virtual1.getParent()){
                    break;
                }
                else{
                    virtual1=virtual1.getParent();
                    check=1;
                    continue;
                }
            }
            else if(file.equals(".") || file.equals("")){
                check=1;
                continue;
            }
            
            for(Tree content: virtual1.getNodelist()){
                if((content.getFile().getName().equals(file))){
                    virtual1=content;
                    check=1;
                    break;
                }
            }
            if(check==0){
                break;
            }
        }
        if(check==0){
            //System.out.println(errmes2);
            Main.printErr(errmes2+"\n");
            
            return;
        }
        if(check==1){
            int check1=1;
            for(Tree content: virtual1.getNodelist()){
                if(virtual.getFile().getName().equals(content.getFile().getName())){
                    check1=0;
                    break;
                }
            }
            if(check1==1){               
                virtual1.Move(virtual);
            }
            else{
                //System.out.println(errmes3);
                Main.printErr(errmes3+"\n");
                
            }
        }
    }
}