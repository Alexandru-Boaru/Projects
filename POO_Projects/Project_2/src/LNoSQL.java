/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;


/**
 *
 * @author alex
 */
public class LNoSQL {
    private String name;
    private int nrNod;
    private int capNod;
    private ArrayList<Node> DBList;

    /**
     *
     * @param name = numele bazei de date
     * @param nrNod = numarul initial de noduri 
     * @param capNod = capaciatatea maxima a unui nod
     */
    public LNoSQL(String name, int nrNod, int capNod) {
        this.name = name;
        this.nrNod = nrNod;
        this.capNod = capNod;
        this.DBList= new ArrayList<Node>(nrNod);
        for(int i=0; i<capNod; i++){
            this.DBList.add(new Node(i,capNod));
        }
    }
    
    /**
     *
     * @param parts = vector care contine atributele instantei ce urmeaza a fi introdusa
     * @param ent = vectorul de entitati in care caut entitatea dupa care voi clona o instanta
     */
    public void Insert(String[] parts,ArrayList<Entitate> ent){
        Entitate temp=null;
        for(int i=0; i<ent.size();i++){
            
            if(ent.get(i).getNumeEnt().equals(parts[1])){
                
                temp = (Entitate)ent.get(i).clone();
                temp.setAtr((ArrayList) ent.get(i).getAtr().clone());
                break;
            }
        }
        for(int p=0; p<temp.getNrAtr(); p++){
            switch((String)temp.getAtr().get(2*p+1)){
                case "Integer": 
                    temp.getAtr().set(2*p+1,Integer.parseInt(parts[p+2]));
                    break;
                case "Float":
                    temp.getAtr().set(2*p+1,Float.parseFloat(parts[p+2]));
                    break;
                case "String": 
                    temp.getAtr().set(2*p+1,parts[p+2]);
                    break;
            }
        }
        Entitate clona=null;
        int max=0;
        
        for(int i=0; i<this.DBList.size(); i++){
                        
            if(this.DBList.get(i).getContorNod()>max){
                            
                max=this.DBList.get(i).getContorNod();
            }
        }
        
        if(max==this.capNod){
            max--;
        }
        if(this.nrNod-this.CheckFull()<temp.getRF()){
            int m=temp.getRF()-(this.nrNod-this.CheckFull());
            this.nrNod+=m;
            for(int i=0; i<m; i++){
                this.DBList.add(new Node(i,capNod));
            }
        }
        
        int check=0;
        for(int i=0; i<temp.getRF(); i++){
            clona=(Entitate)temp.clone();
            while(true){
                check=0;
                for(int j=0; j<this.DBList.size(); j++){
                    if(this.DBList.get(j).getContorNod()==max){
                        clona.Update();
                        this.DBList.get(j).Insert(clona);
                        check=1;
                        break;
                    }
                }
                if(check==1){
                    break;
                }
                else{
                    max--;
                }
            }
        }
        
    }
        
    /**
     *
     * @return numarul de noduri pline
     */
    public int CheckFull(){
        int nrNoduriPline=0;
        for(int i=0; i<this.DBList.size(); i++){
            if(this.DBList.get(i).Full()==1){
                nrNoduriPline++;
            }
        }
        return nrNoduriPline;
    }
    
    /**
     *
     * @param parts = vector care contine informatii folosite pentru a cauta instantele care urmeaza sa fie sterse
     * @param ent = vector de entitati folosit pentru tipul cheii primare (Integer, Float sau String) 
     * @param out = fisierul de output in care urmeaza sa printez mesajul de eroare, daca este necesar
     * @throws IOException
     */
    public void Delete(String[] parts, ArrayList<Entitate> ent, FileWriter out) throws IOException{
        int check=0;
        Object o=null;
        for(int h=0; h<ent.size(); h++){
            if(ent.get(h).getNumeEnt().equals(parts[1])){
                switch((String)ent.get(h).getAtr().get(1)){
                    case "Integer":
                        o=Integer.parseInt(parts[2]);
                        break;
                    case "Float":
                        o=Float.parseFloat(parts[2]);
                        break;
                    case "String":
                        o=parts[2];
                        break;
                }
                break;
            }
        }
            
        for(int i=0; i<this.DBList.size(); i++){
            
            for(int j=0; j<this.DBList.get(i).getContorNod(); j++){
                
                if(this.DBList.get(i).getNodeList().get(j).getNumeEnt().equals(parts[1]) &&
                        this.DBList.get(i).getNodeList().get(j).getAtr().get(1).equals(o)){
                    
                    this.DBList.get(i).getNodeList().remove(j);
                    this.DBList.get(i).Decrement();
                    check += 1;
                }
            }
        }
        if(check==0){
            out.write("NO INSTANCE TO DELETE\n");
        }
    }
    
    /**
     *
     * @param parts = vector care contine informatii folosite pentru a cauta si pentru a actualiza instantele cerute
     * @param ent = vector de entitati folosit pentru tipul cheii primare (Integer, Float sau String) 
     */
    public void Update(String[] parts, ArrayList<Entitate> ent){
        
        Entitate temp=null;
        Entitate ref=null;
        Object o=null;
        for(int h=0; h<ent.size(); h++){
            if(ent.get(h).getNumeEnt().equals(parts[1])){
                ref=ent.get(h);
                switch((String)ent.get(h).getAtr().get(1)){
                    case "Integer":
                        o=Integer.parseInt(parts[2]);
                        break;
                    case "Float":
                        o=Float.parseFloat(parts[2]);
                        break;
                    case "String":
                        o=parts[2];
                        break;
                }
                break;
            }
        }        
        int a=(parts.length-2)/2;
        for(int i=0; i<this.DBList.size(); i++){
            for(int j=0; j<this.DBList.get(i).getContorNod(); j++){
                if(this.DBList.get(i).getNodeList().get(j).getNumeEnt().equals(parts[1]) && 
                        this.DBList.get(i).getNodeList().get(j).getAtr().get(1).equals(o)){
                    
                    temp=this.DBList.get(i).getNodeList().get(j);
                    for(int k=0; k<a; k++){
                        for(int m=0; m<temp.getNrAtr(); m++){
                            if(temp.getAtr().get(2*m).equals(parts[2*k+3])){
                                switch((String)ref.getAtr().get(2*m+1)){
                                    case "Integer":
                                        temp.getAtr().set(2*m+1, Integer.parseInt(parts[2*k+4]));
                                        break;
                                    case "Float":
                                        temp.getAtr().set(2*m+1, Float.parseFloat(parts[2*k+4]));
                                        break;
                                    case "String":
                                        temp.getAtr().set(2*m+1, parts[2*k+4]);
                                        break;
                                }   
                            }
                        }
                    }
                    temp.Update();
                    this.DBList.get(i).getNodeList().remove(j);
                    this.DBList.get(i).getNodeList().add(0, temp);
                    
                    break;
                }
            }
        }
    }
    
    /**
     *
     * @param parts = vector care contine informatii folosite pentru a cauta instanta care trebuie printata
     * @param ent = vector de entitati folosit pentru tipul cheii primare (Integer, Float sau String) 
     * @param out = fisierul de output in care urmeaza sa printez informatiile instantei cautate sau mesajul de eroare
     * @throws IOException
     */
    public void Get(String[] parts, ArrayList<Entitate> ent, FileWriter out) throws IOException{
        int check=0;
        DecimalFormat format = new DecimalFormat("#.##");
        Entitate ref=null;
        Object o=null;
        for(int h=0; h<ent.size(); h++){
            if(ent.get(h).getNumeEnt().equals(parts[1])){
                ref=ent.get(h);
                switch((String)ent.get(h).getAtr().get(1)){
                    case "Integer":
                        try{ o=Integer.parseInt(parts[2]);
                        }
                        catch(Exception e){
                            out.write("NO INSTANCE FOUND\n");
                            return;
                        }
                        break;
                    case "Float":
                        try{
                            o=Float.parseFloat(parts[2]);
                        }
                        catch(Exception e){
                            out.write("NO INSTANCE FOUND\n");
                            return;
                        }
                        break;
                    case "String":
                        o=parts[2];
                        break;
                }
                break;
            }
        }
        
        Entitate temp=null;
        for(int i=0; i<this.DBList.size(); i++){    //printez nodul
            for(int j=0; j<this.DBList.get(i).getContorNod(); j++){
                if(this.DBList.get(i).getNodeList().get(j).getNumeEnt().equals(parts[1]) && 
                        this.DBList.get(i).getNodeList().get(j).getAtr().get(1).equals(o)){
                    if(check==0){
                        out.write("Nod"+(i+1));
                        temp=this.DBList.get(i).getNodeList().get(j);
                        check=1;
                    }
                    else{
                        out.write(" Nod"+(i+1));
                    }
                }
            }           
        }           
        if(check==1){
            out.write(" "+temp.getNumeEnt());
            for(int i=0; i<temp.getNrAtr(); i++){   //printez informatiile instantei
                out.write(" "+temp.getAtr().get(2*i) +":");
                if(ref.getAtr().get(2*i+1).equals("Float")){
                    out.write(format.format(temp.getAtr().get(2*i+1)));
                }
                else if(ref.getAtr().get(2*i+1).equals("Integer")){
                    out.write(temp.getAtr().get(2*i+1).toString());
                }
                else{
                    out.write((String) temp.getAtr().get(2*i+1));
                }
            }
            out.write("\n");
        }
        else{
            out.write("NO INSTANCE FOUND\n");
        }
    }
    
    /**
     *
     * @param ent = vector de entitati folosit pentru tipul cheii primare (Integer, Float sau String) 
     * @param out = fisierul de output in care urmeaza sa printez informatiile instantelor din baza de date
     * @throws IOException
     */
    public void SnapshotDB(ArrayList<Entitate> ent, FileWriter out) throws IOException{
        int check=0;
        Entitate ref=null;
        DecimalFormat format = new DecimalFormat("#.##");
        
        Entitate temp=null;
        for(int i=0; i<this.DBList.size(); i++){    //printez numarul nodului
            if(this.DBList.get(i).getContorNod()!=0){
                out.write("Nod"+(i+1)+"\n");
                check=1;
            }
            for(int j=0; j<this.DBList.get(i).getContorNod(); j++){
                temp=this.DBList.get(i).getNodeList().get(j);
                out.write(temp.getNumeEnt());
                for(int h=0; h<ent.size(); h++){
                    if(temp.getNumeEnt().equals(ent.get(h).getNumeEnt())){
                        ref=ent.get(h);
                    }
                }
                for(int k=0; k<temp.getNrAtr(); k++){   //printez informatiile instantei
                    out.write(" "+temp.getAtr().get(2*k) +":");
                    if(ref.getAtr().get(2*k+1).equals("Float")){
                        out.write(format.format(temp.getAtr().get(2*k+1)));
                    }
                    else if(ref.getAtr().get(2*k+1).equals("Integer")){
                        out.write(temp.getAtr().get(2*k+1).toString());
                    }
                    else{
                        out.write((String) temp.getAtr().get(2*k+1));    
                    }
                    
                }
                out.write("\n");
            }
        }
        if(check==0){
            out.write("EMPTY DB\n");
        }
    }
    
    /**
     *
     * @param timestamp = timestampul in functie de care trebuie sa stergem instantele mai vechi din baza de date
     */
    public void CleanUp(long timestamp){
        
        for(int i=0; i<this.DBList.size(); i++){
            int j=0;
            while(j<this.DBList.get(i).getContorNod()){
                if(this.DBList.get(i).getNodeList().get(j).getTimestamp()<timestamp){
                    this.DBList.get(i).getNodeList().remove(j);
                    this.DBList.get(i).Decrement();
                }
                else{
                    j++;
                }
            }
        }
    }
        
}
