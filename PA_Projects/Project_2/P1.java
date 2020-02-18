

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Scanner;
import java.util.StringTokenizer;


public class P1 {
    static class Task{
        public final static String INPUT_FILE = "p1.in";
	public final static String OUTPUT_FILE = "p1.out";
                int n; //numar de noduri
                int ar[][]; //matrice pentru distante
				//prima coloana este pentru indicii nodurilor
				//a doua coloana este distanta dintre sursa si nod
		//clasa cu functii pentru citire rapida
		class MyScanner {
		    	BufferedReader br;
		    	StringTokenizer st;

    			public MyScanner(FileInputStream f) {
        			br = new BufferedReader(new InputStreamReader(f));
    			}

    			String next() {
        			while (st == null || !st.hasMoreElements()) {
            			try {
                			st = new StringTokenizer(br.readLine());
            			} catch (IOException e) {
                			e.printStackTrace();
            			}
        			}

        		return st.nextToken();
    			}

    			int nextInt() {
    		    		return Integer.parseInt(next());
    			}

    			long nextLong() {
    			    return Long.parseLong(next());
    			}

    			double nextDouble() {
    			    return Double.parseDouble(next());
    			}

    			String nextLine() {
        			String str = "";
        			try {
        			    str = br.readLine();
        			} catch (IOException e) {
        			    e.printStackTrace();
        			}
        			return str;
    			}
    			void close() throws IOException {
				br.close();
   			}
		}
		//citirea datelor de input
                private void readInput() {
			try {
				MyScanner sc = new MyScanner(new FileInputStream(INPUT_FILE));
				n = sc.nextInt();
				ar = new int[n+1][2];
                                ar[0][0]=0;
                                ar[0][1]=0;
				for (int i = 1; i <= n; i++) {
                                        ar[i][0]=i;			//indice
					ar[i][1] = sc.nextInt();	//distanta
				}
                                

				sc.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
		//scrierea rezulatatelor in output
		private void writeOutput(int result[][]) {
			try {
				PrintWriter pw = new PrintWriter(new File(OUTPUT_FILE));
				//daca se gaseste -1 in prima celula (adica valorile din input nu pot crea un graf valid),
				// se trece -1 in output
                                if(result[0][0]==-1){
                                    pw.printf("-1\n");
                                    pw.close();
                                    return;
                                }	
				//altfel se trece numarul de muchii si apoi toate muchiile
                                pw.printf("%d\n",result.length-2);
                                for(int i=2; i<=result.length-1; i++){
                                    pw.printf("%d %d\n", result[i][0],result[i][1]);
                                }
				pw.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
                
                private int[][] getResult() {
		    //sortarea matricii in functie de distante
                    Arrays.sort(ar, new Comparator<int[]>(){
                        public int compare(int a[], int b[]){
                            return a[1]-b[1];
                        }
                    });
                    
                    
                    int last=1; //nodul de pe nivelul anterior
                    int level=0;//nivelul curent
                    int res[][]=new int[n+1][2]; //matricea de muchii
                    res[0][0]=0;
		    //daca se gasesc mai multe distante 0, atunci datele sunt invalide
		    if(ar[2][1]==0){
			res[0][0]=-1;
			return res;
		    }
		    //se populeaza arborele/graful cu muchii
                    for(int i=2; i<=n; i++){
			//daca nivelul este egal cu distanta, atunci se adauga muchie (last,ar[i][0])
                        if(level==ar[i][1]){
                            res[i][0]=last;
                            res[i][1]=ar[i][0];
                        }
			//daca nivelul urmator nu este egal cu distanta (distanta este mai mare) atunci datele sunt invalide
                        else if(level+1!=ar[i][1]){
                            res[0][0]=-1;
                            return res;
                        }
			//se actualizeaza last cu ultimul nod din nivel si se incrementeaza nivelul
                        else{
                            last=ar[i-1][0];
                            res[i][0]=last;
                            res[i][1]=ar[i][0];
                            level++;
                        }
                    }
		    //se returneaza matricea cu muchii
                    return res;
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

