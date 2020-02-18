/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.Comparator;

/**
 *
 * @author alex
 * clasa cu o metoda folosita pentru sortarea lexicografica a continutului unui director
 */
public class SortLexicographically implements Comparator<Tree> {
   public int compare(Tree a, Tree b){
       return a.getFile().getName().compareTo(b.getFile().getName());
   }
    
}
