
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
import java.util.PriorityQueue;
import java.util.StringTokenizer;


public class P4 {
static class Task{
        public final static String INPUT_FILE = "p4.in";
	public final static String OUTPUT_FILE = "p4.out";
        public static final int NMAX = 50005;
                int n;
                int m;
                int t;
                ArrayList<Edge> adj[] = new ArrayList[NMAX]; 	//structura in care se salveaza muchiile grafului
                long dist[][];				     	//matricea de distante
                public PriorityQueue<Edge> pq;		     	//coada de muchii
                long tip[];					//vector in care se stocheaza penalizarile

                //clasa Edge pentru muchii
                public class Edge implements Comparator<Edge>, Comparable<Edge>{
			public int node;	//nodul spre care muchia se indreapta
			public long cost;	//costul minim curent gasit catre acest nod
                        public int type;	//tipul muchiei


                        public Edge(){
                            
                        }
                        Edge(int _node, long _cost, int _type) {
				node = _node;
				cost = _cost;
                                type = _type;
			}
			
                        public int compare(Edge n1, Edge n2){
                            if(n1.cost<n2.cost){
                                return -1;
                            }
                            if(n1.cost>n2.cost){
                                return 1;
                            }
                            return 0;
                        }
                        
                        public int compareTo(Edge e){
                            if(this.cost>e.cost){
                                return 1;
                            }
                            if(this.cost<e.cost){
                                return -1;
                            }
                            return 0;
                            
                        }
                        
                        public String toString(){
                            return node+" "+cost+" "+Integer.toBinaryString(type);
                        }
                        public boolean equals(Object o){
                            if(this==o) return true;
                            Edge nod=(Edge)o;
        
                            if(this.node==nod.node && this.type==nod.type){
                                return true;
                            }
                            else{
                                return false;
                            }
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
                
		//citirea datelor de input
                private void readInput() {
			try {
				MyScanner sc = new MyScanner(new FileInputStream(INPUT_FILE));
				n = sc.nextInt();
                                m = sc.nextInt();
                                t = sc.nextInt();

                                dist= new long[n+1][(int)Math.pow(2,t)];

                                Arrays.fill(dist[1],0);	//se populeaza linia care corespunde nodului sursa cu 0
                                for(int i=2; i<=n; i++)	//restul liniilor se populeaza cu Long.MAX_VALUE
                                    Arrays.fill(dist[i],Long.MAX_VALUE);
                                tip=new long[t+1];
                                pq=new PriorityQueue<Edge>(n,new Edge());
				for (int i = 1; i <= n; i++) {
					adj[i] = new ArrayList<>();
				}
                                for (int i = 1; i <= m; i++) {
					int x, y, z;
                                        long w;
					x = sc.nextInt();
					y = sc.nextInt();
					w = sc.nextLong();
                                        z = sc.nextInt();
					adj[x].add(new Edge(y, w, z));
                                        adj[y].add(new Edge(x, w, z));
                                        
				}
                                for(int i=1; i<=t; i++){
                                        tip[i]=sc.nextLong();
                                }
				sc.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}

		//scrierea rezulatatului in output
		private void writeOutput(long result) {
			try {
				PrintWriter pw = new PrintWriter(new File(OUTPUT_FILE));
				pw.printf("%d\n", result);  
				pw.close();
			} catch (IOException e) {
				throw new RuntimeException(e);
			}
		}
                
                private long getResult() {

		    int paths=(int)Math.pow(2,t)-1;	//numarul de cai posibile

                    for(Edge edge:adj[1]){		//completez costurile in celule care corespund muchiilor ce pleaca din sursa
                        pq.add(edge);
                        dist[edge.node][1<<(edge.type-1)]=edge.cost+tip[edge.type];
                    }



                    while(!pq.isEmpty()){

			//se scoate muchia cu cost minim din coada                        
                        Edge e=pq.poll();
                        int u = e.node;
			//daca muchia este indreptata spre sursa, atunci se ignora
                        if(u==1){
                            continue;
                        }
                        for(Edge edge:adj[u]){	//se itereaza prin muchiile copil
                                                   
				//se pregateste o muchie noua
                                Edge newEdge=new Edge(edge.node,edge.cost,edge.type);

				//variabila prin care se verifica daca trebuie sa se adauge o muchie noua
                                int notify=0;
                                
				//se verifica toate costurile minime in nodul u (pentru fiecare tip de cale care e posibil sa intre in u)
                                for(int j=1; j<=paths; j++){
                   
				    //daca costul este Long.MAX_VALUE inseamna ca nu exista acest tip de cale care intra in acel nod
                                    if(dist[u][j]==Long.MAX_VALUE){
                                        continue;
                                    }
                                    
                                    int type=1<<(edge.type-1);	//tipul muchiei este convert intr-un numar de forma 2^(tip-1)
				    int a=j & type;		
				    int b=j | type;
				    //caz in care tipul muchiei nu exista in calea j (se ia in considerare penalizarea)
                                    if(a==0 && dist[edge.node][b]>dist[u][j]+edge.cost+tip[edge.type]){
                                        dist[edge.node][b]=dist[u][j]+edge.cost+tip[edge.type];
                                        if(dist[edge.node][b]<newEdge.cost){
                                            newEdge.cost=dist[edge.node][b];
                                        }
                                        notify=1;
                                    }
				    //caz in care tipul muchiei exista in calea j (nu se ia penalizarea in considerare)
                                    else if(a!=0 && dist[edge.node][b]>dist[u][j]+edge.cost){
                                        dist[edge.node][b]=dist[u][j]+edge.cost;
                                        if(dist[edge.node][b]<newEdge.cost){
                                            newEdge.cost=dist[edge.node][b];
                                        }
                                        notify=1;
                                        
                                    }
                                    
                                }
				//daca notify este setat la 1, atunci se adauga muchia cea noua in coada
                                if(notify==1){                                    
                                    pq.add(newEdge);                                    
                                }
                        }
                        
                    }
		    //se cauta costul minim pe ultima linie (care corespunde nodului destinatie)
                    long min=Long.MAX_VALUE;
                    for(int i=1; i<=paths; i++){
                        min=Math.min(min, dist[n][i]);
                    }
                    //daca costul minim este egal Long.MAX_VALUE, inseamna ca nu s-a gasit un drum spre nodul destinatie
                    if(min==Long.MAX_VALUE){
                        return -1;
                    }
                    else{
                        return min;
                    }
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
