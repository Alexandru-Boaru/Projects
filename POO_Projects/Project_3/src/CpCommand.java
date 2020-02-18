/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */




/**
 *
 * @author alex
 * clasa care se ocupa cu executia comenzii Cp
 */
public class CpCommand implements Command{
    private Tree virtual;
    private Tree virtual1;
    private String[] destination;
    private String[] source;
    private String errmes1;
    private String errmes2;
    private String errmes3;
    
    /**
     *
     * @param root = directortul root "/"
     * @param parts = sir de string-uri  care contine argumentele comenzii
     */
    public CpCommand(Tree root, String[] parts){
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
        errmes1="cp: cannot copy "+ parts[1] +": No such file or directory";
        errmes2="cp: cannot copy into "+ parts[2] +": No such directory";
        errmes3="cp: cannot copy "+parts[1]+": Node exists at destination";
        
    }

    /**
     * Functia cauta path-ul pana la directorul/fiserul sursa si il copiaza in directorul destinatie
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
                if((content.getFile().getName().equals(file)) && content.getFile() instanceof Folder){
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
                virtual1.add(virtual.copy(virtual1));
                
            }
            else{
                //System.out.println(errmes3);
                Main.printErr(errmes3+"\n");
                
            }
        }
    }
}
