import org.la4j.*;

public class Matrice {
    public static void main(String[] args) {
        Matrix m1 = Matrix.identity(3);
        Matrix m2 = Matrix.identity(3);

        m1 = m1.add(m2);

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                System.out.print(m1.get(row, col) + " ");
            }
            System.out.println();
        }
    }
}