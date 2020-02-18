/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */

public class LRU implements Cache{
    private int max;
    private Subscriptie[] cache;
    private int[] contor;
    private int i;
    private int timestamp=0;
    
    /**
     *
     * @param max = capacitatea maxima a vectorului care defineste cache-ul
     */
    public LRU(int max) {
        this.max = max;
        cache=new Subscriptie[max];
        contor=new int[max];
        i=0;
    }

    /**
     *
     * @return intoarce vectorul de obiecte de tip Subscriptie din cache
     */
    public Subscriptie[] getCache() {
        return cache;
    }

    /**
     *
     * @param o = noul obiect care trebuie introdus in cache
     */
    public void Inlocuire(Subscriptie o){
        int min=this.contor[0];
        int p=0;
        for(int j=0; j<this.max; j++){
            if(min>this.contor[j]){
                min=this.contor[j];
                p=j;
            }
        }
        this.cache[p]=o;       
        this.contor[p]=this.timestamp;
        this.timestamp++;
    }
    
    /**
     *
     * @param nume = numele obicetului care trebuie eliminat, datorita suprascrierii in memoria principala
     */
    public void Eliminare(String nume){
        if(this.i==1 && this.cache[0].getNume().equals(nume)){
            this.cache[0]=null;
            this.contor[0]=0;
            i--;
        }
        else{
            for(int j=0;j<this.i;j++){
                if(this.cache[j].getNume().equals(nume)){
                    for(int k=j; k<this.i-1; k++){
                        this.cache[k]=this.cache[k+1];
                        this.contor[k]=this.contor[k+1];
                    }
                    
                    this.cache[this.i-1]=null;
                    this.contor[this.i-1]=0;
                    i--;
                    break;
                }    
            }
        }
    }
   
    /**
     *
     * @param nume = numele obiectului de tip Free ce urmeaza sa fie introdus in cache
     */
    public void Add(String nume){
        Free o=new Free(nume);
        if(this.i==this.max){
            this.Inlocuire(o);
        }
        else{
            this.cache[i]=o;
            this.contor[i]=this.timestamp;
            i++;
            this.timestamp++;
        }
    }
    
    /**
     *
     * @param nume = numele obiectului de tip Basic ce urmeaza sa fie introdus in cache
     * @param req_b = numarul de cereri basic ale obiectului ce urmeaza sa fie introdus in cache
     */
    public void Add(String nume, int req_b){
        Basic o=new Basic(nume,req_b);
        if(this.i==this.max){
            this.Inlocuire(o);
        }
        else{
            this.cache[i]=o;
            this.contor[i]=this.timestamp;
            i++;
            this.timestamp++;
        }
    }
    
    /**
     *
     * @param nume = numele obiectului de tip Premium ce urmeaza sa fie introdus in cache
     * @param req_b = numarul de cereri basic ale obiectului ce urmeaza sa fie introdus in cache
     * @param req_p = numarul de cereri premium ale obiectului ce urmeaza sa fie introdus in cache
     */
    public void Add(String nume, int req_b, int req_p){
        Premium o=new Premium(nume,req_b,req_p);
        if(this.i==this.max){
            this.Inlocuire(o);
        }
        else{
            this.cache[i]=o;
            this.contor[i]=this.timestamp;
            i++;
            this.timestamp++;
        }
    }
    
    /**
     *
     * @param nume = numele obiectului care se utilizeaza
     * @return 1, daca obiectul exista in cache/ 0, daca obiectul nu exista in cache
     */
    public int Get(String nume){
        if(this.Search(nume)!=-1){
            this.contor[this.Search(nume)]=this.timestamp;
            this.timestamp++;
            return 1;
        }        
        return 0;
    }
    
    /**
     *
     * @param nume = numele obiectului care se cauta in cache
     * @return j=indexul obiectului in vector, daca exista obiectul/ -1, daca nu exista obiectul
     */
    public int Search(String nume){
        Subscriptie o1=null;
        
        for(int j=0;j<i;j++){
            o1=cache[j];
            if(o1.getNume().equals(nume)){
                return j;
            }
        }
        return -1;
    }
}

