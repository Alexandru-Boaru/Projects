/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
/**
 *
 * @author alex
 */
public class Tema2 {

    /**
     * @param args = numele fisierului de input
     */
    public static void main(String[] args){
        
        FileReader in = null;
        FileWriter out = null;
        try {
            in = new FileReader(args[0]);
        } catch (FileNotFoundException ex) {
            System.out.println("EROARE");
        }
        try {
            out = new FileWriter(args[0]+"_out");
        } catch (IOException ex) {
            System.out.println("EROARE");
        }
        BufferedReader br= new BufferedReader(in);
        String line=null;
       
        String[] parts;
        LNoSQL baza = null;
        ArrayList<Entitate> ent = new ArrayList<Entitate>();
        try {
                line=br.readLine();
            } catch (IOException ex) {
                System.out.println("EROARE");
            }
        while(line!=null){
            
            try {

                parts = line.split(" ");
                switch(parts[0]){
                    case "CREATEDB":
                        baza=new LNoSQL(parts[1],Integer.parseInt(parts[2]),Integer.parseInt(parts[3]));
                        break;
                    case "CREATE":
                        ent.add(new Entitate(parts));
                        break;
                    case "INSERT":
                        baza.Insert(parts, ent);
                        break;
                    case "DELETE":
                        baza.Delete(parts, ent, out);
                        break;
                    case "UPDATE":
                        baza.Update(parts, ent);
                        break;
                    case "GET":
                        baza.Get(parts, ent, out);
                        break;
                    case "SNAPSHOTDB":
                        baza.SnapshotDB(ent, out);
                        break;
                    case "CLEANUP":
                        baza.CleanUp(Long.parseLong(parts[2]));
                        break;
                }
                
                try {
                    line=br.readLine();
                } catch (IOException ex) {
                    System.out.println("EROARE");
                }
                
            } catch (IOException ex) {
                System.out.println("EROARE");
            }
            
        }
            
      
        
        if(in!=null){
            try {
                in.close();
            } catch (IOException ex) {
                System.out.println("EROARE");
            }
        }
        if(out!=null){
            try {
                out.close();
            } catch (IOException ex) {
                System.out.println("EROARE");
            }
        }

    }
    
}
