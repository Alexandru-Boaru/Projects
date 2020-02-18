/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 * clasa factory pentru crearea comenzilor
 */
public class CommandFactory {
    
    /**
     *
     * @param t = baza arborelui (directorul root)
     * @param parts = sir de string-uri care contine argumentele comenzii
     * @return returneaza un tip de comanda
     */
    public static Command getCommand(Tree t, String[] parts){        
        switch(parts[0]){
                case "ls":
                    return new LsCommand(t,parts);
                    
                case "pwd":
                    return new PwdCommand(Tree.getCurrent(),0);
                    //System.out.println();
                    //Main.printOut("\n");
                case "cd":
                    return new CdCommand(t,parts);
                case "cp":
                    return new CpCommand(t,parts);
                case "mv":
                    return new MvCommand(t,parts);
                case "rm":
                    return new RmCommand(t,parts);
                case "touch":
                    return new TouchCommand(t,parts);
                case "mkdir":                    
                    return new MkdirCommand(t,parts);
                default:
                    return null;
                }           
    }
    
}
