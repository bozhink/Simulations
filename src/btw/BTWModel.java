package btw;
/**
 * @author Bozhin Karaivanov
 * @version 0.2
 */
public class BTWModel {
    public int nx; // x-dimension
    public int ny; // y-dimension
    public int matrix[][];
    private int cx; // x-coordinate of central point
    private int cy; // y-coordinate of central point
    public int THRESHOLD = 4;

    public BTWModel(int nx, int ny) {
        this.nx = nx;
        this.ny = ny;
        matrix = new int[nx][ny];
        cx = (nx-1)/2+1;
        cy = (ny-1)/2+1;
    }
    
    private boolean ifContinue() {
        for (int i=0; i<nx; i++) {
            for (int j=0; j<ny; j++) {
                if (matrix[i][j] > THRESHOLD) {
                    return false;
                }
            }
        }
        return true;
    }

    public void Update() {
        if (ifContinue()) {
            matrix[cx][cy]++;
        }
        for (int x=0; x<nx; x++) {
            for (int y=0; y<ny; y++) {
                if (matrix[x][y] > THRESHOLD) {
                    int xp1 = (x+1+nx) % nx;
                    int xm1 = (x-1+nx) % nx;
                    int yp1 = (y+1+ny) % ny;
                    int ym1 = (y-1+ny) % ny;
                    matrix[x][y] -= THRESHOLD;
                    matrix[x][yp1]++;
                    matrix[x][ym1]++;
                    matrix[xp1][y]++;
                    matrix[xm1][y]++;
                }
            }
        }
    }
}
