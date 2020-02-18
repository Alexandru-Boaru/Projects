import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Set;
import java.nio.charset.StandardCharsets;

/**
 * Class for a space explorer.
 */
public class SpaceExplorer extends Thread {
        private int id;
        private Integer hashCount;
        private volatile Set<Integer> discovered;
        private CommunicationChannel channel;
        
	/**
	 * Creates a {@code SpaceExplorer} object.
	 * 
	 * @param hashCount
	 *            number of times that a space explorer repeats the hash operation
	 *            when decoding
	 * @param discovered
	 *            set containing the IDs of the discovered solar systems
	 * @param channel
	 *            communication channel between the space explorers and the
	 *            headquarters
	 */
	public SpaceExplorer(Integer hashCount, Set<Integer> discovered, CommunicationChannel channel) {
            
            this.hashCount = hashCount;
            this.discovered = discovered;
            this.channel = channel;
            this.id = (int)currentThread().getId();
	}

	@Override
	public void run() {
            while (true) {

                
                Message m1, m2;
                int currentSolarSystem, nextSolarSystem;
                String frequencyToDecode, frequencyDecoded;
                
                
                //preiau mesajul primit de la HeadQuarter
                
                m1 = channel.getMessageHeadQuarterChannel();
                if(m1 == null){
                    continue;
                }
                
                //salvez datele primite
                nextSolarSystem = m1.getCurrentSolarSystem();
                frequencyToDecode = m1.getData();
                
                //daca se primeste mesaj de "EXIT", se incheie metoda
                if(nextSolarSystem==-1 && frequencyToDecode.compareTo("EXIT")==0){
                        break;
                }
                currentSolarSystem = m1.getParentSolarSystem();
                
                //se verifica daca nu a fost deja explorat sistemul solar
                if(!discovered.contains(nextSolarSystem)){
                    discovered.add(nextSolarSystem);
                }
                else{
                    continue;
                }
                
                //decodarea frecventei
                frequencyDecoded = encryptMultipleTimes(frequencyToDecode, hashCount);
                 
                //adaugarea mesajului in CommunicationChannel
                channel.putMessageSpaceExplorerChannel(new Message(currentSolarSystem, nextSolarSystem, frequencyDecoded));
            }
	}
	
	/**
	 * Applies a hash function to a string for a given number of times (i.e.,
	 * decodes a frequency).
	 * 
	 * @param input
	 *            string to he hashed multiple times
	 * @param count
	 *            number of times that the string is hashed
	 * @return hashed string (i.e., decoded frequency)
	 */
	private String encryptMultipleTimes(String input, Integer count) {
		String hashed = input;
		for (int i = 0; i < count; ++i) {
			hashed = encryptThisString(hashed);
		}

		return hashed;
	}

	/**
	 * Applies a hash function to a string (to be used multiple times when decoding
	 * a frequency).
	 * 
	 * @param input
	 *            string to be hashed
	 * @return hashed string
	 */
	private static String encryptThisString(String input) {
		try {
			MessageDigest md = MessageDigest.getInstance("SHA-256");
			byte[] messageDigest = md.digest(input.getBytes(StandardCharsets.UTF_8));

			// convert to string
			StringBuffer hexString = new StringBuffer();
			for (int i = 0; i < messageDigest.length; i++) {
				String hex = Integer.toHexString(0xff & messageDigest[i]);
				if (hex.length() == 1)
					hexString.append('0');
				hexString.append(hex);
			}
			return hexString.toString();

		} catch (NoSuchAlgorithmException e) {
			throw new RuntimeException(e);
		}
	}
}
