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

/**
 *
 * @author alex
 */
public class Main {

    static FileWriter out;
    static FileWriter err;
    
    static void printOut(String s){
        try {
            out.write(s);
        } catch (IOException ex) {
            System.out.println("ERROR");
        }
    }
    
    static void printErr(String s){
        try {
            err.write(s);
        } catch (IOException ex) {
            System.out.println("ERROR");
        }
    }
    /**
     * @param args = numele fiserelor input, output si error
     */
    public static void main(String[] args) {
        // TODO code application logic here
        FileReader in = null;
        
        try {
            in = new FileReader(args[0]);
        } catch (FileNotFoundException ex) {
            System.out.println(ex.getMessage());        
        }
        try {
            out = new FileWriter(args[1]);
        } catch (IOException ex) {
            System.out.println(ex.getMessage());        
        }
        try {
            err = new FileWriter(args[2]);
        } catch (IOException ex) {
            System.out.println(ex.getMessage());        
        }
        BufferedReader br= new BufferedReader(in);
        String line=null;
        String[] parts;
        Tree t= Tree.getTree();
        
        CommandInvoker ci = CommandInvoker.getInvoker();
        
        try {
                line=br.readLine();
            } 
        catch (IOException ex) {
                System.out.println("EROARE");
        }
        int indexCommand=1;
        while(line!=null){
            
            //System.out.println(indexCommand);
            Main.printOut(indexCommand+"\n");
            Main.printErr(indexCommand+"\n");
            
            indexCommand++;
                parts = line.split(" ");
                Command c=CommandFactory.getCommand(t, parts);
                if(c!=null){
                    ci.takeCommand(c);
                    if(c instanceof PwdCommand){
                        Main.printOut("\n");
                    }
                }
                try {
                    line=br.readLine();
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
        
        if(err!=null){
            try {
                err.close();
            } catch (IOException ex) {
                System.out.println("EROARE");
            }
        }
        
    }
    
}
