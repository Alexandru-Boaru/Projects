
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;


public class P4 {
    static class Task {
		public final static String INPUT_FILE = "p4.in"; //numele fisierului de input
		public final static String OUTPUT_FILE = "p4.out"; //numele fisierului de output

		int n; //numarul de elemente ale unui sir
                int k; //diferenta la care trebuie sa se ajunga
                int v; //numarul pana la cat se pot lua elemente
		private void readInput() { //metoda care se ocupa cu citirea datelor din fisier
			try {
				Scanner sc = new Scanner(new File(INPUT_FILE));
				n = sc.nextInt();
				k = sc.nextInt();
				v = sc.nextInt();
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
                        int mod = 1000000007;
			//declararea tabelei pentru combinari
                        int dp[][]=new int[n+1][n+1];
			//caz de baza
                        dp[0][0] = 1;
                        for(int i = 1; i < n+1; i++) {
			//cazuri de baza
                            dp[i][0] = 1;
                            for(int j = 1; j <= i; j++) {
				//completare tabela
				//proprietate combinari C(n,k) = C(n-1,k-1)+C(n-1,k)
                                dp[i][j] = (dp[i - 1][j - 1] + dp[i - 1][j])%mod;
                            }
                        }
			//numarul cautat de combinari
                        int num=dp[n][n/2];
                        int aux=n/2;
                        if(n%2==1){
			    //se mai inmulteste odata cu n/2+1 daca n este par
                            num=(int) ((1L*num*(aux+1))%mod);
                        }
                        else{
			    //se mai inmulteste odata cu n/2 daca n este impar
                            num=(int) ((1L*num*aux)%mod);
                        }
			//se pregateste o variabila pentru calculul produsului
                        int check=1;
                        for(int i=1;i<n;i++){
                        	if(i%2==1){
				    //se inmulteste cu (v-k-i/2) pentru i impar
                        	    check=(int) ((1L*check*(v-k-i/2))%mod);
                        	}
                        	else{
				    //se inmulteste cu (k-i/2) pentru i par
                        	    check=(int) ((1L*check*(k-i/2))%mod);
                        	}
                	}
			//(numar combinari modificat)*produs
                	int res = (int) ((1L*check*num)%mod);
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
