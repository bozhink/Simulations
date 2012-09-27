package btw;
/**
 * @author Bozhin Karaivanov
 * @version 0.1
 * September, 2012
 * 
 * This class contains the matrix and the most important
 * methods of the implementation of the Bak–Tang–Wiesenfeld (BTW) 
 * sandpile model.
 */

public class BTWModel {
    public int nx; // x-dimension
    public int ny; // y-dimension
    public int matrix[][];
    private int cx; // x-coordinate of central point
    private int cy; // y-coordinate of central point
    public int THRESHOLD = 4;
    /**
     * 
     * @param nx X-dimension of the matrix.
     * @param ny Y-dimension of the matrix.
     */
    public BTWModel(int nx, int ny) {
        this.nx = nx;
        this.ny = ny;
        matrix = new int[nx][ny];
        cx = (nx-1)/2+1;
        cy = (ny-1)/2+1;
    }
    /**
     * Limitation for the recursion depth.
     * This parameter is used to avoid the stack overflow error.
     */
    private int maxRecursionOrder = 600;
    /**
     * Recursive method to recalculate the matrix.
     */
    private void update(int x, int y, int recursionOrder) {
        if (recursionOrder < maxRecursionOrder) {
        if (matrix[x][y] > THRESHOLD) {
            int xp1 = (x+1+nx) % nx;
            int xm1 = (x-1+nx) % nx;
            int yp1 = (y+1+ny) % ny;
            int ym1 = (y-1+ny) % ny;
            matrix[x][y]-=4;
            matrix[x][yp1]++;
            update(x, yp1, recursionOrder+1);
            matrix[x][ym1]++;
            update(x, ym1, recursionOrder+1);
            matrix[xp1][y]++;
            update(xp1, y, recursionOrder+1);
            matrix[xm1][y]++;
            update(xm1, y, recursionOrder+1);
        }
        } else {
            System.err.println("update: Maximum recursion order exeeded.");
        }
    }
    /**
     * Initialization for the 'update' recursive method.
     */
    public void Update() {
        matrix[cx][cy]++;
        update(cx, cy, 0);
    }
}
