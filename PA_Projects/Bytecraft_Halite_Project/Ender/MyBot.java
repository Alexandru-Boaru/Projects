// This Java API uses camelCase instead of the snake_case as documented in the API docs.
//     Otherwise the names of methods are consistent.

import hlt.*;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Random;

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

        
        for (;;) {
            game.updateFrame();
            final Player me = game.me;
            final GameMap gameMap = game.gameMap;

            
            
            final ArrayList<Command> commandQueue = new ArrayList<>();

            /*for (final Ship ship : me.ships.values()) {
                if (gameMap.at(ship).halite < Constants.MAX_HALITE / 10 || ship.isFull()) {
                    final Direction randomDirection = Direction.ALL_CARDINALS.get(rng.nextInt(4));
                    commandQueue.add(ship.move(randomDirection));
                } else {
                    commandQueue.add(ship.stayStill());
                }
            }

            if (
                game.turnNumber <= 200 &&
                me.halite >= Constants.SHIP_COST &&
                !gameMap.at(me.shipyard).isOccupied())
            {
                commandQueue.add(me.shipyard.spawn());
            }*/
            
            for(final Ship ship : me.ships.values()){
                
                if(game.turnNumber >= 385){
                    if(gameMap.calculateDistance(ship.position,me.shipyard.position)==1){
                        ArrayList<Direction> place = gameMap.getUnsafeMoves(ship.position, me.shipyard.position);
                        commandQueue.add(ship.move(place.get(0)));
                    }
                    else{
                        commandQueue.add(ship.move(gameMap.naiveNavigate(ship,me.shipyard.position)));
                    }
                    continue;
                }
                
                if(gameMap.calculateDistance(ship.position,me.shipyard.position)==0){
                    ship.full = false;
                }
                
                if(ship.isFull() || ship.full==true){
                    ship.full = true;
                    commandQueue.add(ship.move(gameMap.naiveNavigate(ship,me.shipyard.position)));
                    continue;
                }
                
                if(ship.halite > Constants.MAX_HALITE*0.75){
                    commandQueue.add(ship.move(gameMap.naiveNavigate(ship,me.shipyard.position)));
                    continue;
                }
                
                if(gameMap.at(ship).halite >100){
                    commandQueue.add(ship.stayStill());
                    continue;
                }
                
                /*if(game.turnNumber <= 10){
                    final Direction randomDirection = Direction.ALL_CARDINALS.get(rng.nextInt(4));
                    commandQueue.add(ship.move(randomDirection));
                    break;
                }*/
                
                MapCell max=gameMap.at(ship);
                ArrayList<Position> p = gameMap.at(ship).position.getSurroundingCardinals();
                for(Position i:p){
                    if(gameMap.at(i).halite>max.halite &&
                            gameMap.at(i).halite/4 > gameMap.at(ship).halite/10 &&
                            gameMap.at(ship).halite/10 <= ship.halite && 
                            gameMap.at(i).isEmpty() &&
                            game.turnNumber < 385){
                        
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
            if (game.turnNumber <=200 &&
                me.halite >= Constants.SHIP_COST &&
                !gameMap.at(me.shipyard).isOccupied())
                {                    
                    commandQueue.add(me.shipyard.spawn());
                    
                
                }
            game.endTurn(commandQueue);
        }
    }
}
