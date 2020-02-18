import hlt.*;
import myBot.*;

import org.la4j.*;

import java.util.ArrayList;
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

        game.ready("Bytecraft");
        Log.log("Successfully created bot! My Player ID is " + game.myId + ". Bot rng seed is " + rngSeed + ".");

        for (;;) {
            game.updateFrame();
            final ArrayList<Command> commandQueue = new ArrayList<>();

            commandQueue.addAll(Movement.getMoves(game));

            if (HaliteSpender.shouldBuyShip(game)) {
                commandQueue.add(game.me.shipyard.spawn());
            }
            if (HaliteSpender.shouldBuyDropoff) {
                commandQueue.add(HaliteSpender.getShipToDropoff().makeDropoff());
            }
            game.endTurn(commandQueue);

        }
    }
}
