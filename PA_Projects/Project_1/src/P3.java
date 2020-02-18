import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;

public class P3 {
    static class Task {
		public final static String INPUT_FILE = "p3.in"; //numele fisierului de input
		public final static String OUTPUT_FILE = "p3.out"; //numele fisierului de output

		int n; //numarul de elemente
		int[] v; //vector in care se stocheaza elementele in ordine

		private void readInput() { //metoda care se ocupa cu citirea datelor din fisier
			try {
				Scanner sc = new Scanner(new File(INPUT_FILE));
				n = sc.nextInt();
				v = new int[n];
				for (int i = 0; i < n; i++) {
					v[i] = sc.nextInt();
				}
				sc.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}

		private void writeOutput(long result) { //metoda care se ocupa cu scrierea rezultatului in fisierul de output
			try {
				PrintWriter pw = new PrintWriter(new File(OUTPUT_FILE));
				pw.printf("%d\n", result);
				pw.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}

                
                
                
		private long getResult() {
                        long a,b,c;
                        long s=0;
			//se calculeaza suma tuturor elementelor
                        for(int i=0; i<n; i++){
                            s+=v[i];
                        }
			//se initializeaza tabela de valori
                        long dp[][]=new long[n][n];
			
                        for(int k=0; k<n; ++k){
                            for(int i=0, j=k; j<n ;++i,++j){
				//se extrage elementul dp[i+2][j]
                                if(i+2<=j){
                                    a=dp[i+2][j];
                                }
                                else{
                                    a=0;
                                }
                                //se extrage elementul dp[i+1][j-1]
                                if(i+1<=j-1){
                                    b=dp[i+1][j-1];
                                }
                                else{
                                    b=0;
                                }
                                //se extrage elementul dp[i][j-2]
                                if(i<=j-2){
                                    c=dp[i][j-2];
                                }
                                else{
                                    c=0;
                                }
                                //se aplica aplica formula folosita pentru calculul scorului cel mai mare posibil pe sirul [i,...,j]
                                dp[i][j]=Math.max(v[i]+Math.min(a, b),v[j]+Math.min(b,c));
                            }
                        }
			//rezultatul maxim final al lui Tuzgu
                        long Tuzgu = dp[0][n-1];
			//se returneaza diferenta maxima
                        return 2*Tuzgu-s;
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
