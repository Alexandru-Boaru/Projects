/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 */
public interface Cache {

    /**
     *
     * @param nume = numele obiectului de tip Free ce urmeaza sa fie introdus in cache
     */
    public abstract void Add(String nume);

    /**
     *
     * @param nume = numele obiectului de tip Basic ce urmeaza sa fie introdus in cache
     * @param c_basic = numarul de cereri basic ale obiectului ce urmeaza sa fie introdus in cache
     */
    public abstract void Add(String nume, int c_basic);

    /**
     *
     * @param nume = numele obiectului de tip Premium ce urmeaza sa fie introdus in cache
     * @param c_basic = numarul de cereri basic ale obiectului ce urmeaza sa fie introdus in cache
     * @param c_premium = numarul de cereri premium ale obiectului ce urmeaza sa fie introdus in cache
     */
    public abstract void Add(String nume, int c_basic, int c_premium);

    /**
     *
     * @param nume = numele obiectului care se utilizeaza
     * @return 1, daca obiectul exista in cache/ 0, daca obiectul nu exista in cache
     */
    public abstract int Get(String nume);

    /**
     *
     * @param nume = numele obiectului care se cauta in cache
     * @return indexul obiectului in vector, daca exista obiectul/ -1, daca nu exista obiectul
     */
    public abstract int Search(String nume);

    /**
     *
     * @param o = noul obiect care trebuie introdus in cache
     */
    public abstract void Inlocuire(Subscriptie o);

    /**
     *
     * @param nume = numele obicetului care trebuie eliminat, datorita suprascrierii in memoria principala
     */
    public abstract void Eliminare(String nume);

    /**
     *
     * @return intoarce vectorul de obiecte de tip Subscriptie din cache
     */
    public abstract Subscriptie[] getCache();
}


