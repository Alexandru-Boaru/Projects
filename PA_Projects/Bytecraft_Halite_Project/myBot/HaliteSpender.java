/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package myBot;

import hlt.*;

/**
 *
 * @author Adrian
 */
public class HaliteSpender {

    public static Boolean shouldBuyDropoff = false;
    private static Ship shipToDropoff;

    public static Boolean shouldBuyShip(Game game) {
        final Player me = game.me;
        final GameMap gameMap = game.gameMap;

        if (game.turnNumber <= 200
                && //in primele 200 de ture se spawneaza
                me.halite >= Constants.SHIP_COST
                && //cat mai multe nave
                !gameMap.at(me.shipyard).isOccupied()) {
            return true;
        }
        return false;
    }

    public static Ship getShipToDropoff() {
        shouldBuyDropoff = false;
        return shipToDropoff;
    }
    
    public static void planToBuyDropoff(Ship ship) {
        if (shouldBuyDropoff == false) {
            shouldBuyDropoff = true;
            shipToDropoff = ship;
        }
    }
}
