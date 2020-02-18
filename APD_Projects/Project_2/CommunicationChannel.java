/**
 * Class that implements the channel used by headquarters and space explorers to communicate.
 */
import java.util.HashMap;
import java.util.concurrent.*;


public class CommunicationChannel {

        private final ConcurrentLinkedQueue<Message> headQuarterChannel = new ConcurrentLinkedQueue();
        private final ConcurrentLinkedQueue<Message> spaceExplorerChannel = new ConcurrentLinkedQueue();
        private final HashMap<Integer, Integer> savedMessage = new HashMap();

	/**
	 * Creates a {@code CommunicationChannel} object.
	 */
	public CommunicationChannel() {
	}

	/**
	 * Puts a message on the space explorer channel (i.e., where space explorers write to and 
	 * headquarters read from).
	 * 
	 * @param message
	 *            message to be put on the channel
	 */
	public void putMessageSpaceExplorerChannel(Message message) {
            //se adauga mesaj in queue
                spaceExplorerChannel.offer(message);
	}

	/**
	 * Gets a message from the space explorer channel (i.e., where space explorers write to and
	 * headquarters read from).
	 * 
	 * @return message from the space explorer channel
	 */
	public Message getMessageSpaceExplorerChannel() {
            
            //HeadQuarter-ul citeste un mesaj
            return spaceExplorerChannel.poll();
	}

	/**
	 * Puts a message on the headquarters channel (i.e., where headquarters write to and 
	 * space explorers read from).
	 * 
	 * @param message
	 *            message to be put on the channel
	 */
	public void putMessageHeadQuarterChannel(Message message) {
            //daca este mesaj de "EXIT" il trimite mai departe
            if(message.getData().compareTo("EXIT") == 0){
                headQuarterChannel.offer(message);
                return;
            }
            
            //daca este mesaj de "END" il ignora
            if(message.getData().compareTo("END")==0){
                return;
            }
            
            //daca exista un mesaj primit de la acelasi thread si retinut de mai devreme in savedMessage
            //atunci se trimite un mesaj care contine informatia necesara din ambele mesaje de la headQuarter
            //se sterge informatia din savedMessage
            if(savedMessage.containsKey((int)Thread.currentThread().getId())){
                headQuarterChannel.offer(new Message(savedMessage.remove((int)Thread.currentThread().getId()), message.getCurrentSolarSystem(), message.getData()));
            }
            //altfel salveaza informatia mesajului in savedMessage
            else{
                savedMessage.put((int)Thread.currentThread().getId(), message.getCurrentSolarSystem());
            }
            
	}

	/**
	 * Gets a message from the headquarters channel (i.e., where headquarters write to and
	 * space explorer read from).
	 * 
	 * @return message from the header quarter channel
	 */
	public Message getMessageHeadQuarterChannel() {
            
            //space explorer-ul citeste un mesaj
            return headQuarterChannel.poll();
	}
}
