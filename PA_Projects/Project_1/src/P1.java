import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;

public class P1 {
	static class Task {
		public final static String INPUT_FILE = "p1.in"; //numele fisierului de input
		public final static String OUTPUT_FILE = "p1.out"; //numele fisierului de output

		int n; //numarul total de elemente
		Integer[] v; //vector in care se stocheaza elementele in ordinea primita

		private void readInput() { //metoda care se ocupa cu citirea datelor din fisier
			try {
				Scanner sc = new Scanner(new File(INPUT_FILE));
				
				n = sc.nextInt();
				v = new Integer[n];
				for (int i = 0; i < n; i++) {
					v[i] = sc.nextInt();
				}
                                
				sc.close();

			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}

		private void writeOutput(int result) { //metoda care se ocupa cu scrierea rezultatului in fisierul de output
			try {
				PrintWriter pw = new PrintWriter(new File(OUTPUT_FILE));
				pw.printf("%d\n", result);
				pw.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
                
		private int getResult() {
			//continultul lui v se transfera intr-un arraylist
                        ArrayList<Integer> l = new ArrayList<Integer>(Arrays.asList(v));
			//se sorteaza elementele in ordine descrescatore
                        Collections.sort(l,Collections.reverseOrder());
                        
                        int Tuzga=0; //scorul maxim al lui Tuzga
                        int Ritzu=0; //scorul maxim al Ritzei

                        for(int i=0; i<n/2;i++){
                             //in turele pare, elementul cel mai mare se adauga la scorul lui Tuzga, iar in cele impare
					//se adauga in la scorul Ritzei
                                Tuzga+=l.get(2*i);
                            
                                Ritzu+=l.get(2*i+1);
                            
                        }
			if(n%2==1){
				Tuzga+=l.get(n-1);
			}
			//se returneaza diferenta maxima
                        return Tuzga-Ritzu;
		}

		public void solve() {
			readInput();
			writeOutput(getResult());
		}
	}

	public static void main(String[] args) {
		new Task().solve();
	}
}
