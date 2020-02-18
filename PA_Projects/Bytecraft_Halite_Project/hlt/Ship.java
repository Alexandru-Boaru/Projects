package hlt;

public class Ship extends Entity {

    public final int halite;

    public Ship(final PlayerId owner, final EntityId id, final Position position, final int halite) {
        super(owner, id, position);
        this.halite = halite;
    }

    public boolean isFull() {
        return halite >= Constants.MAX_HALITE;
    }

    public Command makeDropoff() {
        return Command.transformShipIntoDropoffSite(id);
    }

    public Command move(final Direction direction) {
        return Command.move(id, direction);
    }

    public Command stayStill() {
        return Command.move(id, Direction.STILL);
    }

    static Ship _generate(final PlayerId playerId) {
        final Input input = Input.readInput();

        final EntityId shipId = new EntityId(input.getInt());
        final int x = input.getInt();
        final int y = input.getInt();
        final int halite = input.getInt();

        return new Ship(playerId, shipId, new Position(x, y), halite);
    }

    public Position closestDropoff(Game game) {
        final Player me = game.me;
        final GameMap gameMap = game.gameMap;
        Position closestDropoff = me.shipyard.position;
        int distanceToClosestDropoff = gameMap.calculateDistance(position, me.shipyard.position);

        for (Dropoff dropoff : me.dropoffs.values()) {        //se cauta cel mai apropiat dropoff
            if (distanceToClosestDropoff > gameMap.calculateDistance(position, dropoff.position)) {
                distanceToClosestDropoff = gameMap.calculateDistance(position, dropoff.position);
                closestDropoff = dropoff.position;
            }
        }
        return closestDropoff;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        if (!super.equals(o)) {
            return false;
        }

        Ship ship = (Ship) o;

        return halite == ship.halite;
    }

    @Override
    public int hashCode() {
        int result = super.hashCode();
        result = 31 * result + halite;
        return result;
    }
}
