package Bytecraft;

import java.util.ArrayList;
import hlt.*;

public class Movement {
    public static ArrayList<Command> shipMoves(Game game) {
        Player me = game.me;
        GameMap gameMap = game.gameMap;
        ArrayList<Command> commands = new ArrayList<Command>();

        for (final Ship ship : me.ships.values()) {
            // Daca nava are suficient halite il va depozita
            // Am putea sa cautam cu algoritmul QuadTree cel mai apropiat dropoff / shipyard
            if (ship.halite > Constants.MAX_HALITE * 0.85) {
                commands.add(ship.move(gameMap.naiveNavigate(ship, me.shipyard.position)));
            } else {
                // Verificam daca se merita sa mai minam celula curenta
                if (gameMap.at(ship.position).halite > Constants.MAX_HALITE * 0.05) {
                    commands.add(ship.stayStill());
                } else {
                    // Verificam imprejurimile (cele 4 celule vecine)
                    Direction bestDir = null;
                    int bestHalite = 0;

                    for(Direction dir : Direction.ALL_CARDINALS){
                        Position pos = ship.position.directionalOffset(dir);
                        if(gameMap.at(pos).halite > bestHalite && !gameMap.at(pos).isOccupied()){
                            bestHalite = gameMap.at(pos).halite;
                            bestDir = dir;
                        }
                    }

                    if (bestDir == null) {
                        bestDir = Direction.STILL;
                    }

                    commands.add(ship.move(gameMap.naiveNavigate(ship, ship.position.directionalOffset(bestDir))));

                }
                
            }
        }

        return commands;
    }
}