/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.*;


/**
 *
 * @author alex
 */
public class Main {

    /**
     * @param argv the command line arguments
     * @throws java.io.FileNotFoundException
     * @throws java.io.IOException
     */
    public static void main(String[] argv) throws FileNotFoundException, IOException {
        FileReader in = new FileReader(argv[0]);
        FileWriter out = new FileWriter(argv[1]);
        BufferedReader br= new BufferedReader(in);
        String line=null;
        String cache=null;
        cache=br.readLine();
        int max;
        max=Integer.parseInt(br.readLine());
        int com;
        com=Integer.parseInt(br.readLine());
        
        Memorie mem=new Memorie();
        Cache c=null;
        if(cache.equals("LRU")){
            c = new LRU(max);
        }
        else if(cache.equals("FIFO")){
            c = new FIFO(max);
        }
        else if(cache.equals("LFU")){
            c = new LFU(max);
        }
        
        for(int i=0; i<com;i++){
            line=br.readLine();
            String[] parts = line.split(" ");
            if(parts[0].equals("ADD")){
  
                if(parts.length==4){                  
                    if(mem.Search(parts[1])!=-1){
                        c.Eliminare(parts[1]);                        
                    }
                    mem.Add(parts[1], Integer.parseInt(parts[2]),Integer.parseInt(parts[3]));
                }
                else{
                    if(mem.Search(parts[1])!=-1){
                        c.Eliminare(parts[1]);                        
                    }
                    mem.Add(parts[1], Integer.parseInt(parts[2]));
                }
            }
            else if(parts[0].equals("GET")){
    
                if(c.Get(parts[1])==1){
                    
                    out.write("0 "+mem.getMem()[mem.Search(parts[1])].Tip()+"\n");
                    mem.getMem()[mem.Search(parts[1])].Scade();
                    c.getCache()[c.Search(parts[1])].Scade();
                }
                else if(mem.Search(parts[1])!=-1){                    
                    
                    String tip=mem.getMem()[mem.Search(parts[1])].Tip();
                    Subscriptie o=mem.getMem()[mem.Search(parts[1])];
                    String name = o.getNume();
                    int b=o.getReq_b();
                    int p=o.getReq_p();
                    out.write("1 "+tip+"\n");
                    
                    if(tip.equals("Premium")){
                        c.Add(name, b, p);
                    }
                    else if(tip.equals("Basic")){
                        c.Add(name, b);
                    }
                    else{
                        c.Add(name);
                    }
                    mem.getMem()[mem.Search(parts[1])].Scade();
                    c.getCache()[c.Search(parts[1])].Scade();
                    
                }
                else{
                    out.write("2"+"\n");
                }
            }
            
        }
        
        if(in!=null){
            in.close();
        }
        if(out!=null){
            out.close();
        }
    }
}  
