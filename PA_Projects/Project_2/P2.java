

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Objects;
import java.util.Queue;
import java.util.Stack;
import java.util.StringTokenizer;


public class P2 {
    
    
    
    static class Task{
        public final static String INPUT_FILE = "p2.in";
	public final static String OUTPUT_FILE = "p2.out";
        
	//clasa Coord retine coordonatele unui nod din matrice 
        class Coord implements Comparator<Coord>{
            public int i;	//linie
            public int j;	//coloana
    
            public Coord(int i, int j){
                this.i=i;
                this.j=j;
            }
            public int compare(Coord a, Coord b){
                if(a.i==b.i){
                    return Integer.compare(a.j, b.j);
                }
                else{
                    return Integer.compare(a.i, b.i);
                }
            }
            
            public boolean equals(Object o){
                if(this==o){
                    return true;
                }
                if(o==null){
                    return false;
                }
                Coord c=(Coord)o;
                if(this.i==c.i && this.j==c.j){
                    return true;
                }
                else{
                    return false;
                }
            }
            
            public int hashCode(){
                return Objects.hash(i,j);
            }
            
            public String toString() { 
                return String.format(i+" "+j); 
            } 
        }
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
                int n,m,k; 	//N,M,K
                int ar[][];	//matricea de valori (graf)
                int vis[][];	//matricea nodurilor vizitate

                //citirea datelor de input
                private void readInput() {
			try {
				MyScanner sc = new MyScanner(new FileInputStream(INPUT_FILE));
				n = sc.nextInt();
                                m = sc.nextInt();
                                k = sc.nextInt();
				ar = new int[n+1][m+1];
                                vis = new int[n+1][m+1];
                                ar[0][0]=0;
				for (int i = 1; i <= n; i++) {
                                    for(int j=1; j<=m; j++){
					ar[i][j] = sc.nextInt();
                                        vis[i][j] = 0;
                                    }
				}                                
				sc.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}

		//scrierea rezulatatului in output
		private void writeOutput(int result) {
			try {
				PrintWriter pw = new PrintWriter(new File(OUTPUT_FILE));
				pw.printf("%d\n", result);
				pw.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}

                private int getResult() {
                    Stack<Coord> st=new Stack<Coord>();	//stiva folosita pentru parcurgerea DFS
                    int max=0;				//aria maxima
                    HashSet<Coord> seen = new HashSet<Coord>();
                    for(int i=1; i<=n; i++){
                        for(int j=1; j<=m; j++){
                            if(vis[i][j]==0){	//daca nodul nu face parte dintr-un alt DFS, atunci se incepe parcurgerea din acel nod
                                seen.clear();
                                int count=1;
                                int max_cur=ar[i][j];//maximul curent in DFS
                                int min_cur=ar[i][j];//minimul curent in DFS
                                int iaux=i;
                                int jaux=j;
                                int ia=0,ja=0;
                                int safe=0;
                                do{
				    //se verifica daca vecinii nodului curent indeplinesc urmatoarele conditii
					//-nu se depaseste marginea matricii
					//-nu este un nod din arborele DFS curent (nu e pe stiva)
					//-nu este un nod care a fost deja procesat in acest DFS
                                    if(iaux+1<=n && !st.contains(new Coord(iaux+1,jaux)) && !seen.contains(new Coord(iaux+1,jaux))){
                                        //in jos

                                        ia=iaux+1;
                                        ja=jaux;
                                    }
                                    else if(jaux+1<=m && !st.contains(new Coord(iaux,jaux+1)) && !seen.contains(new Coord(iaux,jaux+1))){
                                        //in dreapta

                                        ia=iaux;
                                        ja=jaux+1;
                                    }
                                    else if(iaux-1>0 && !st.contains(new Coord(iaux-1,jaux)) && !seen.contains(new Coord(iaux-1,jaux))){
                                        //in jos
                                        ia=iaux-1;
                                        ja=jaux;
                                    }
                                    else if(jaux-1>0 && !st.contains(new Coord(iaux,jaux-1)) && !seen.contains(new Coord(iaux,jaux-1))){
                                        //in stanga
                                        ia=iaux;
                                        ja=jaux-1;
                                    }
                                    else{
					//daca niciunul din vecini nu poate fi parcurs, atunci se nodul curent se marcheaza ca fiind visitat
					//si este adaugat in set-ul nodurilor procesate
                                        vis[iaux][jaux]=1;
                                        seen.add(new Coord(iaux,jaux));
                                        if(st.empty()){//daca stiva este goala inseamna ca s-au procesat toate nodurile posibile
                                            break;
                                        }
                                        else{//se scoate din stiva nodul anterior (backtrack)
                                            Coord c=st.pop();
                                            iaux=c.i;
                                            jaux=c.j;
                                            continue;
                                        }
                                    }
                                    //daca vecinul se afla intre limite atunci se incrementeaza contorul
                                    if(ar[ia][ja]<=max_cur && ar[ia][ja]>=min_cur){
                                        count++;
                                    }
				    //daca vecinul depaseste maximul curent atunci se 
				    //actualizeaza maximul curent (daca se poate) si se incrementeaza contorul
                                    else if(ar[ia][ja]>max_cur && ar[ia][ja]-min_cur<=k){
                                        max_cur=ar[ia][ja];
                                        count++;
                                    }
				    //daca vecinul depaseste minimul curent atunci se 
				    //actualizeaza minimul curent (daca se poate) si se incrementeaza contorul
                                    else if(ar[ia][ja]<min_cur && max_cur-ar[ia][ja]<=k){
                                        min_cur=ar[ia][ja];
                                        count++;
                                    }
				    //daca nu se poate nicio varianta de mai sus, atunci se adaga vecinul in set-ul de noduri
				    //si se trece mai departe
                                    else{
                                        seen.add(new Coord(ia,ja));
                                        continue;
                                    }
				    //se adauga nodul curent in stiva
                                    st.add(new Coord(iaux,jaux));
                                    if(iaux==i && jaux==j){
                                        safe++; //safe este o cale prin care ma asigur ca s-au verificat cei patru vecini ai nodului
						//curent (cand se ajunge la verificarea conditiei de bucla, stiva poate sa fie goala 
						//cand se lucreaza pe nodul sursa)
                                    }
				    //se introduce nodul curent in stiva
                                    seen.add(new Coord(iaux,jaux));
                                    vis[iaux][jaux]=1;	//se marcheaza nodul ca fiind vizitat
				    //se actualizeaza coordonatele curente cu cele ale vecinului pe care se continua parcurgerea
                                    iaux=ia;
                                    jaux=ja;
                                }while(!(st.isEmpty() && safe==4));
                                //daca contorul curent (aria) este mai mare decat un maxim gasit anterior, atunci se actualizeaza maximul
                                if(count>max){
                                    max=count;
                                }
                            }
                        }
                    }
                    return max;
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
