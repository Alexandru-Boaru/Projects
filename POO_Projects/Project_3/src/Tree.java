/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.ArrayList;
import java.util.Collections;


/**
 *
 * @author alex
 * clasa care se ocupa cu crearea arborelui si cu gestionarea nodurilor
 */
public class Tree{
    private File file;
    private Tree parent;
    private ArrayList<Tree> nodelist; 
    private static Tree current;
    
    private Tree(){}
    
    /**
     *
     * @param parent = nodul parinte
     * @param name = numele asociat obiectului File
     * @param type = tipul obiectului File
     */
    public Tree(Tree parent, String name, int type){
        this.file = FileFactory.getFile(name, type);
        this.parent=parent;
        this.nodelist=new ArrayList();
    }
    
    /**
     *
     * @param parent = nodul parinte
     * @param file = fisierul care trebuie pus in nod
     * @param nodelist = continutul nodului
     */
    public Tree(Tree parent, File file,ArrayList<Tree> nodelist){
        this.parent=parent;
        this.file=file;
        this.nodelist=nodelist;
    }
    
    /**
     *
     * @return = returneaza o singura instanta a root-ului (baza arborelui)
     */
    public static Tree getTree(){
        Tree tree = new Tree();
        tree.file=FileFactory.getFile("/", 0);
        tree.parent=tree;
        tree.nodelist=new ArrayList();
        Tree.current=tree;
        return tree;
    }
    
    /**
     *
     * @param t = nod ce trebuie adaugat
     */
    public void add(Tree t){
        this.nodelist.add(t);
    }
    
    /**
     *
     * @param t = nod ce trebuie sters
     */
    public void remove(Tree t){
        this.nodelist.remove(t);
    }
    
    /**
     *
     * @param name = numele fisierului din nodul ce trebuie sters
     */
    public void remove(String name){
        for(Tree content: this.nodelist){
            if(content.getFile().getName().equals(name)){
                this.nodelist.remove(content);
                break;
            }
        }
    }

    /**
     *
     * @return = intoarce lista de noduri copii a nodului respectiv
     */
    public ArrayList<Tree> getNodelist() {
        return nodelist;
    }

    /**
     *
     * @return = intoarce nodul ce contine directorul curent
     */
    public static Tree getCurrent() {
        return Tree.current;
    }

    /**
     *
     * @param current = noul nod care contine directorul curent
     */
    public static void setCurrent(Tree current) {
        Tree.current = current;
    }
    
    /**
     *
     * @return = returneaza nodul parinte
     */
    public Tree getParent() {
        return parent;
    }

    /**
     *
     * @param parent = noul parinte al nodului
     */
    public void setParent(Tree parent) {
        this.parent = parent;
    }

    /**
     *
     * @return = returneaza fisierul din nod
     */
    public File getFile() {
        return file;
    }
    
    /**
     *
     * @param parent = nodul parinte al nodului respectiv
     * @return = retuneaza o cpoie recursiva nodului
     */
    public Tree copy(Tree parent){
        
        ArrayList<Tree> nodelist=null;
        File clonefile=(File) this.getFile().clone();
        Tree temp=null;
        if(this.getNodelist()!=null && (clonefile instanceof Folder)){
            nodelist=new ArrayList<Tree>();
            temp=new Tree(parent, clonefile, nodelist);
            for(Tree content: this.getNodelist()){
            
                nodelist.add(content.copy(temp));
            }
        }
        return new Tree(parent, clonefile, nodelist);
    }
    
    /**
     * metoda folosita pentru printarea nodurilor copii ai nodului 
     */
    public void print(){
        CommandInvoker ci = CommandInvoker.getInvoker();
        if(this.getParent()==this){
            //System.out.println("/:");
            Main.printOut("/:\n");
            
        }
        else{
            
            ci.takeCommand(new PwdCommand(this,0));
            //System.out.println(":");
            Main.printOut(":\n");
            
        }
        
        Collections.sort(this.getNodelist(), new SortLexicographically());
            for(Tree content: this.getNodelist()){
                ci.takeCommand(new PwdCommand(content,0));
                if(content!= this.getNodelist().get(this.getNodelist().size()-1)){
                    //System.out.print(" ");
                    Main.printOut(" ");
                
                }
            }
        //System.out.println("\n");
        Main.printOut("\n\n");        
    }
    
    /**
     * metoda folosita pentru printarea recursiva a nodurilor
     */
    public void printR(){
        
        this.print();
        for(Tree content: this.getNodelist()){
            if(content.getFile() instanceof Folder){
                content.printR();
            }
        }
    }
    
    /**
     *
     * @param t = nod ce trebuie mutat la noua destinatie (nodul care apeleaza metoda)
     */
    public void Move(Tree t){
        this.add(t);
        t.getParent().remove(t);
        t.setParent(this);
        if(t.getFile() instanceof Folder){
            ArrayList<Tree> temp= new ArrayList<Tree>();
            for(Tree file: t.getNodelist()){
                temp.add(file);
            }
            for(Tree file: temp){
                t.Move(file);
            }
        }
    }
    
}
