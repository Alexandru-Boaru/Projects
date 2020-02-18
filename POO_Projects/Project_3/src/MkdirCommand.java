/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



/**
 *
 * @author alex
 * clasa care se ocupa cu executia comenzii Mkdir
 */
public class MkdirCommand implements Command{
    private Tree virtual;
    private String[] path;
    private String name;
    private String errmes1=null;
    private String errmes2_1=null;
    private String errmes2_2=null;
    
    /**
     *
     * @param root = directortul root "/"
     * @param parts = sir de string-uri  care contine argumentele comenzii
     */
    public MkdirCommand(Tree root, String[] parts){
        if(parts[1].charAt(0)=='/'){
            virtual=root;
        }
        else{
            virtual=Tree.getCurrent();
        }
        path=parts[1].split("/");
        name=path[path.length-1];
        if(parts[1].lastIndexOf('/')>=0){
           errmes1="mkdir: "+parts[1].substring(0, parts[1].lastIndexOf('/'))+": No such directory";
           
           
        }
        errmes2_1="mkdir: cannot create directory ";
        errmes2_2=": Node exists";
        
    }
    
    /**
     * Functia cauta path-ul pana la directorul dat ca argument si creeaza in acesta un nou director
     */
    public void execute(){
        int check=0;
        CommandInvoker ci= CommandInvoker.getInvoker();
        for(String file : path){
            check=0;
            if(file==path[path.length-1]){
                check=1;
                break;
            }
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
            if(virtual.getNodelist()!=null){
                for(Tree content: virtual.getNodelist()){
                    
                    if((content.getFile().getName().equals(file)) && (content.getFile() instanceof Folder)){
                        virtual=content;
                        check=1;
                        break;
                    }
                }
            }
            else{
                check=1;
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
        if(check==1){
            for(Tree file: virtual.getNodelist()){
                if(file.getFile().getName().equals(name)){
                    
                    //System.out.print(errmes2_1);
                    Main.printErr(errmes2_1);
                    
                    ci.takeCommand(new PwdCommand(file,1));
                    //System.out.println(errmes2_2);
                    Main.printErr(errmes2_2+"\n");
                    
                    return;
                }
            }
            Tree t= new Tree(virtual,name,0);
            virtual.add(t);
        }
        
    }
}