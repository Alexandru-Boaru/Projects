/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package myBot;

import hlt.*;
import java.util.ArrayList;

/**
 *
 * @author Adrian
 */
public class Movement {

    private static ArrayList<EntityId> returningShips = new ArrayList<>(); //lista pentru navele care s-au umplut

    public static ArrayList<Command> getMoves(Game game) {
        ArrayList<Command> moves = new ArrayList<>();
        final GameMap gameMap = game.gameMap;
        final Player me = game.me;
	Position bestPos;

        if (game.turnNumber >= Constants.MAX_TURNS - 20) {
            return getMovesTowardsDropoffs(game);
        }
        updateReturningShips(game);
        for (Ship ship : me.ships.values()) {
            if (returningShips.contains(ship.id)) {
                Position closestDropoff = ship.closestDropoff(game);
                if (gameMap.calculateDistance(ship.position, closestDropoff) > 20
                        && me.halite > 8000) {             //daca nu exista un dropoff in vecinatate atunci nava devine dropoff
                    HaliteSpender.planToBuyDropoff(ship);
                } else {
                    moves.add(ship.move(gameMap.naiveNavigate(ship, closestDropoff)));
                }
            } else {
		bestPos = ship.position.directionalOffset(getBestDirectionToMove(gameMap, ship));
                moves.add(ship.move(gameMap.naiveNavigate(ship, bestPos)));
            }
        }
        return moves;
    }

    private static ArrayList<Command> getMovesTowardsDropoffs(Game game) {
        ArrayList<Command> moves = new ArrayList<>();
        final GameMap gameMap = game.gameMap;
        final Player me = game.me;
        Position closestDropoff;

        for (final Ship ship : me.ships.values()) {

            closestDropoff = ship.closestDropoff(game);

            if (gameMap.calculateDistance(ship.position, closestDropoff) == 1) {
                ArrayList<Direction> place = gameMap.getUnsafeMoves(ship.position, closestDropoff);
                moves.add(ship.move(place.get(0)));
            } else {
                moves.add(ship.move(gameMap.naiveNavigate(ship, closestDropoff)));
            }
        }
        return moves;
    }

    private static void updateReturningShips(Game game) {
        final GameMap gameMap = game.gameMap;
        final Player me = game.me;

        for (final Ship ship : me.ships.values()) {
            if (ship.isFull() && !returningShips.contains(ship.id)) {
                returningShips.add(ship.id);
            } else if (gameMap.calculateDistance(ship.position, me.shipyard.position) == 0) {
                returningShips.remove(ship.id);
            } else {
                for (Dropoff dropoff : me.dropoffs.values()) {
                    if (gameMap.calculateDistance(ship.position, dropoff.position) == 0) {
                        returningShips.remove(ship.id);
                    }
                }
            }
        }
    }

    private static Direction getBestDirectionToMove(GameMap gameMap, Ship ship) {
        Direction bestDirection = Direction.STILL;
        Position relativePosition;
        int bestHalite;
        
        if (gameMap.at(ship).halite > 100 || gameMap.at(ship).halite / 10 > ship.halite) {
            return Direction.STILL;
        }
	bestHalite = gameMap.at(ship.position).halite;
        for (Direction direction : Direction.ALL_CARDINALS) {                                      //se cauta in care din cele 4 directii
            //directii cardinale se gaseste mai mult
            //halite, evitandu-se coliziunile
            relativePosition = ship.position.directionalOffset(direction);
            if (gameMap.at(relativePosition).halite > bestHalite
                    && gameMap.at(relativePosition).halite / 4 >= gameMap.at(ship).halite / 10
                    && gameMap.at(relativePosition).isEmpty()) {
		bestHalite = gameMap.at(relativePosition).halite;
                bestDirection = direction;
            }
        }
        return bestDirection;
    }
}
