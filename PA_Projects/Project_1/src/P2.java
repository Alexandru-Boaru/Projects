/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;


public class P2 {
    static class Task {
		public final static String INPUT_FILE = "p2.in"; //numele fisierului de input
		public final static String OUTPUT_FILE = "p2.out"; //numele fisierului de output

		int n; //numarul total de elemente primite
                int k; //numarul de elemente ce trebuie eliminate
		Integer[] v; //vector in care se stocheaza elementele in ordinea primita

		private void readInput() { //metoda care se ocupa cu citirea datelor din fisier
			try {
				Scanner sc = new Scanner(new File(INPUT_FILE));
				n = sc.nextInt();
                                k = sc.nextInt();
				v = new Integer[n];
				for (int i = 0; i < n; i++) {
					v[i] = sc.nextInt();
				}
                                
                                System.out.println();
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
			//daca numarul de elemente eliminate este egal cu cel al elementelor primite, rezultatul final este 0

                        if(n-k==0){
                            return 0;
                        }
			//se sorteaza elementele in ordine descrescatore
                        Collections.sort(l,Collections.reverseOrder());

			//se initializeaza tabela de valori
                        int dp[][]=new int[n][k+1];
                        
			//se umple prima linie cu numai cu primul element
                        for(int i=0; i<k+1; i++){
                            dp[0][i]=l.get(0);
                        }
                        
			//se calculeaza la fiecare runda care este diferenta de punctaj si se pune in cate o celula din prima coloana
			//ca si cum s-ar juca un joc normal
                        for(int i=1; i<n; i++){
                            if(i%2==1){
                                dp[i][0]=dp[i-1][0]-l.get(i);
                            }
                            else{
                                dp[i][0]=dp[i-1][0]+l.get(i);
                            }
                        }
                        //se completeaza celelalte celule, pe fiecare coloana eliminandu-se cate un element si obtinand pe diagonala
			//rezultale cele mai bune
                        for(int j=1; j<k+1; j++){
                            for(int i=1; i<n-j; i++){
                                if(i%2==1){
                                    dp[i][j]=Math.max(dp[i][j-1],dp[i-1][j]-l.get(i+j));
                                }
                                else{
                                    dp[i][j]=Math.max(dp[i][j-1],dp[i-1][j]+l.get(i+j));
                                }
                            }
                        }
                        //se returneaza rezultatul cautat
                        return dp[n-k-1][k];
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

