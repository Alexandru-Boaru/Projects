/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author alex
 * clasa care se ocupa cu crearea unui invocator de comenzi
 */
public class CommandInvoker {
    private static CommandInvoker ci=new CommandInvoker();
    private CommandInvoker(){}

    /**
     *
     * @return = returneaza un invocator de comenzi
     */
    public static CommandInvoker getInvoker(){
        return ci;
    }

    /**
     *
     * @param c = comanda ce trebuie executata
     */
    public void takeCommand(Command c){
        c.execute();
    }
    
}
