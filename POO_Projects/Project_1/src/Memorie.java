/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author alex
 */
public class Memorie {
    private Subscriptie[] mem;
    private int i=0;
    private int max;

    /**
     * creaza un vector mem cu un maxim 100 de elemente
     */
    public Memorie() {
        this.max = 100;
        mem=new Subscriptie[max];
    }

    /**
     *
     * @return returneaza vectorul care contine obiectele din memoria principala
     */
    public Subscriptie[] getMem() {
        return mem;
    }
    
    /**
     *
     * @param nume = numele obiectului pe care il cauta
     * @return j=indexul obiectului in vector, daca exista obiectul/ -1, daca nu exista obiectul
     */
    public int Search(String nume){
        for(int j=0;j<i;j++){
            if(mem[j].getNume().equals(nume)){
                return j;
            }
        }
        return -1;
    }
    
    /**
     * verifica daca s-a ajuns la capacitate maxima si eventual dubleaza capacitatea memoriei
     */
    public void Full(){
        if(i>=this.max){
            this.max=2*this.max;
            Subscriptie[] a= new Subscriptie [this.max];
            for(int j=0;j<this.max/2;j++){
                a[j]=this.mem[j];
            }
            this.mem=a;
        }
    }
    
    /**
     *
     * @param nume = numele obiectului de tip Premium ce urmeaza sa fie adugat in memorie
     * @param req_b = numarul de cereri de tip premium
     * @param req_p = numarul de cereri de tip basic
     */
    public void Add(String nume, int req_b, int req_p){
        
        Premium o=new Premium(nume,req_b,req_p);
        this.Full();
        if(this.Search(nume)!=-1){
            mem[this.Search(nume)]=o;            
        }
        else{
            mem[i]=o;
            i++;
        }
    }
    
    /**
     *
     * @param name = numele obiectului de tip Basic ce urmeaza sa fie adugat in memorie
     * @param req_b = numarul de cereri de tip premium
     */
    public void Add(String name, int req_b){
        
        Basic o=new Basic(name,req_b);
        this.Full();
        if(this.Search(name)!=-1){
            mem[this.Search(name)]=o;           
        }
        else{
            mem[i]=o;
            i++;
        }
    }
    
}
