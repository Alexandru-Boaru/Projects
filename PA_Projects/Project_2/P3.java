
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


public class P3 {
    static class Task{
        public final static String INPUT_FILE = "p3.in";
	public final static String OUTPUT_FILE = "p3.out";
        public static final int NMAX = 201;
                int n;
                int m;
                int t;
                int source;
                ArrayList<Edge> adj[] = new ArrayList[NMAX];	//structura in care se salveaza muchiile grafului
                long dist[][];					//matricea de distante
                public PriorityQueue<Edge> pq;			//coada de muchii
                long tip[][];					//matricea in care se stocheaza penalizarile
                
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
                            return node+" "+cost+" "+type;
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
				source = 1;
                                dist= new long[n+1][t+1];
                                Arrays.fill(dist[1],0);	//se populeaza linia care corespunde nodului sursa cu 0
                                for(int i=2; i<=n; i++)	//restul liniilor se populeaza cu Long.MAX_VALUE
                                    Arrays.fill(dist[i],Long.MAX_VALUE);
                                tip=new long[t+1][t+1];
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
                                    for(int j=1; j<=t; j++){
                                        tip[i][j]=sc.nextLong();
                                    }
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
		    //se adauga nodul sursa in coada sub forma unei muchii cu cost 0 si de tip nedefinit (nu are importanta)
                    pq.add(new Edge(source,0,-1));

                    while(!pq.isEmpty()){
			//se scoate o muchie din coada
                        Edge e=pq.poll();

			//nodul spre care muchia este directionata
			int u = e.node;

			//se viziteaza toate muchiile care pleaca din nodul u
                        for(Edge edge:adj[u]){ 

				//se pregateste o muchie noua
                                Edge newEdge=new Edge(edge.node,edge.cost,edge.type);

				//variabila prin care se verifica daca trebuie sa se adauge o muchie noua
                                int notify=0;

				//se verifica toate costurile minime in nodul u (pentru fiecare tip de muchie care e posibil sa intre in u)
                                for(int j=1; j<=t; j++){

				    //daca costul este Long.MAX_VALUE inseamna ca nu exista o muchie cu acel tip care intra in acel nod
                                    if(dist[u][j]==Long.MAX_VALUE){
                                        continue;
                                    }

				    //daca se gaseste un cost mai mic decat cel curent, atunci se actualizeaza dist[edge.node][edge.type],
				    //se adauga in newEdge costul minim si notify este setat la 1
                                    if(dist[edge.node][edge.type]>dist[u][j]+edge.cost+tip[edge.type][j]){
                                        dist[edge.node][edge.type]=dist[u][j]+edge.cost+tip[edge.type][j];
                                        newEdge.cost=dist[edge.node][edge.type];
                                        notify =1;
                                        }
                                }

				//daca notify este setat la 1, atunci se adauga muchia cea noua in coada
                                if(notify==1)
                                    pq.add(newEdge);
                                

                        }
                    }
		    //se cauta costul minim pe ultima linie (care corespunde nodului destinatie)
                    long min=Long.MAX_VALUE;
                    for(int i=1; i<=t; i++){
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
