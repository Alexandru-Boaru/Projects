// This Java API uses camelCase instead of the snake_case as documented in the API docs.
//     Otherwise the names of methods are consistent.

import hlt.*;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Random;

//cautam zone cu cat mai mult halite (impartim harta in cateva patrate)
//prioritizare zone halite mult (trimitere barci)
//dropoff-uri in zonele cu mult halite

public class MyBot {
    public static void main(final String[] args) {
        final long rngSeed;
        if (args.length > 1) {
            rngSeed = Integer.parseInt(args[1]);
        } else {
            rngSeed = System.nanoTime();
        }
        final Random rng = new Random(rngSeed);

        Game game = new Game();
        // At this point "game" variable is populated with initial map data.
        // This is a good place to do computationally expensive start-up pre-processing.
        // As soon as you call "ready" function below, the 2 second per turn timer will start.
        game.ready("MyJavaBot");

        Log.log("Successfully created bot! My Player ID is " + game.myId + ". Bot rng seed is " + rngSeed + ".");

        final ArrayList<Integer> fullShips = new ArrayList<>(); //lista pentru navele care s-au umplut
        
        for (;;) {
            game.updateFrame();
            final Player me = game.me;
            final GameMap gameMap = game.gameMap;
            final ArrayList<Command> commandQueue = new ArrayList<>();
            
            boolean built=true;  //variabila folosita pt a impedica construirea 
                                 //mai multor dropoff-uri in aceeasi tura
            
            for(final Ship ship : me.ships.values()){

                if(game.turnNumber >= Constants.MAX_TURNS-20){//*0.90){    //in ultimele 10% din ture toate navele
                                                                    //se indreapta spre cel mai apropiat 
                                                                    //dropoff, indiferent de cat s-a adunat
                    Position home=me.shipyard.position;
                    int dist=gameMap.calculateDistance(ship.position,me.shipyard.position);
                    
                    for(Dropoff drop: me.dropoffs.values()){        //se cauta cel mai apropiat dropoff
                        if(dist>gameMap.calculateDistance(ship.position,drop.position)){
                            dist=gameMap.calculateDistance(ship.position,drop.position);
                            home=drop.position;
                        }
                    }
                    
                    if(gameMap.calculateDistance(ship.position,home)==1){   //odata ajunse la dropoff navele
                                                                            //intra una peste alta, astfel ele
                                                                            //se distrug intre ele, dar halite-ul
                                                                            //va ajunge in storage
                        ArrayList<Direction> place = gameMap.getUnsafeMoves(ship.position, home);
                        commandQueue.add(ship.move(place.get(0)));
                    }
                    else{
                        commandQueue.add(ship.move(gameMap.naiveNavigate(ship,home)));
                    }
                    continue;
                }
                
                if(gameMap.calculateDistance(ship.position,me.shipyard.position)==0){
                    fullShips.remove(Integer.valueOf(ship.id.id));
                }
                else{
                    for(Dropoff drop: me.dropoffs.values()){
                        if(gameMap.calculateDistance(ship.position,drop.position)==0){
                            fullShips.remove(Integer.valueOf(ship.id.id));
                        }
                    }
                }
                
                if(ship.isFull() || fullShips.contains(ship.id.id)){    //daca o nava se umple, atunci ea se
                                                                        //indreapta spre cel mai apropiat dropoff
                                                                        //si evitand coliziunile
                    if(!fullShips.contains(ship.id.id)){                
                        fullShips.add(ship.id.id);                      //se adauga id-ul navei in lista
                    }
                    Position home=me.shipyard.position;
                    int dist=gameMap.calculateDistance(ship.position,me.shipyard.position);
                    for(Dropoff drop: me.dropoffs.values()){            //se cauta cel mai apropiat dropoff
                        if(dist>gameMap.calculateDistance(ship.position,drop.position)){
                            dist=gameMap.calculateDistance(ship.position,drop.position);
                            home=drop.position;
                        }
                    }
                    
                    if(dist>20 && me.halite>8000 && built){             //daca nu exista un dropoff in vecinatate
                        built=false;                                    //atunci nava devine dropoff
                        commandQueue.add(ship.makeDropoff());
                    }
                    else{
                        commandQueue.add(ship.move(gameMap.naiveNavigate(ship,home)));
                    }
                    continue;
                }
                
                if(gameMap.at(ship).halite >100){                       //se asteapta pana cand celula ajunge
                                                                        //sa aiba sub 100 de halite pentru a
                                                                        //nu consuma prea mult halite pe 
                                                                        //drumul de intoarcere
                    commandQueue.add(ship.stayStill());
                    continue;
                }
                
                MapCell max=gameMap.at(ship);
                ArrayList<Position> p = gameMap.at(ship).position.getSurroundingCardinals();
                
                for(Position i:p){                                      //se cauta in care din cele 4 directii
                                                                        //directii cardinale se gaseste mai mult
                                                                        //halite, evitandu-se coliziunile
                                                                 
                    if(gameMap.at(i).halite>max.halite &&
                        gameMap.at(i).halite/4 >= gameMap.at(ship).halite/10 &&
                        gameMap.at(ship).halite/10 <= ship.halite && 
                        gameMap.at(i).isEmpty() &&
                        game.turnNumber < Constants.MAX_TURNS-20){//*0.90){
                        
                        max=gameMap.at(i);
                    }
                }
                if(max==gameMap.at(ship)){
                    commandQueue.add(ship.stayStill());
                }
                else{
                    commandQueue.add(ship.move(gameMap.naiveNavigate(ship,max.position)));
                }                  
                
            }
            
            if (game.turnNumber <=200 &&                                //in primele 200 de ture se spawneaza
                me.halite >= Constants.SHIP_COST &&                     //cat mai multe nave
                !gameMap.at(me.shipyard).isOccupied())
                {                    
                    commandQueue.add(me.shipyard.spawn());
                    
                
                }
            
            game.endTurn(commandQueue);
            
            
        }
    }
}
