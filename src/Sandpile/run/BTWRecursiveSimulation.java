package run;

import btw.BTWRecursiveModel;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

/**
 * @author Bozhin Karaivanov
 * @version 0.1 September, 2012
 * 
 *          This program simulates the Bak–Tang–Wiesenfeld (BTW) sandpile
 *          model, where every grain is dropped in the central cell of a
 *          'nx'-by-'ny' matrix.
 * 
 *          The output of this program are graphical files (in PNG format) which
 *          contain picture of the matrix's content at every iteration after the
 *          process of relaxation of the matrix.
 * 
 *          The relaxation process is 'hidden' - it won't be generated pictures
 *          of the intermediate states during the relaxation. Because of that it
 *          is impossible to observe the avalanches, and we can see only the
 *          stable symmetric final states.
 * 
 *          This 'hidden' relaxation is implemented by recursive call of the
 *          private function 'update' in btw.BTWModel. There is a limitation of
 *          the recursion depth (maxRecursionOrder = 600) and this limit
 *          determines the maximal number of iterations (MAX_ITERATIONS = 8200).
 *          If we do more iterations the results will be (quasi)unpredictable;
 *          if we increment 'maxRecursionOrder' it is possible to occur stack
 *          overflow.
 */

public class BTWRecursiveSimulation {
	/**
	 * Colors which are used to plot particle number of any cell. The array
	 * index corresponds to this number of particles (grains).
	 */
	static Color color[] = { Color.white, Color.cyan, Color.blue, Color.green,
			Color.black, Color.orange, Color.red };
	/**
	 * Dimension of the used in the model matrix.
	 */
	static int nx = 51;
	static int ny = 51;
	static BTWRecursiveModel model = new BTWRecursiveModel(nx, ny);

	/**
	 * @param args
	 *            Not used.
	 */
	public static void main(String[] args) {
		String fileName;
		int MAX_ITERATIONS = 8200;
		for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
			model.Update();
			fileName = String.format("btwsim-%1$06d.png", iter);

			try {
				int width = 600, height = 600;
				int w = width / ny;
				int h = height / nx;

				BufferedImage bi = new BufferedImage(width + 10 * w, height
						+ 10 * h, BufferedImage.TYPE_INT_ARGB);
				Graphics2D g = bi.createGraphics();

				for (int i = 0; i < model.nx; i++) {
					for (int j = 0; j < model.ny; j++) {
						g.setColor(color[model.matrix[i][j] % 6]);
						g.fillRect(j * w, i * h, w, h);
					}
				}
				// Setting the legend
				for (int k = 0; k < 4; k++) {
					g.setColor(color[k]);
					g.fillRect(width - w, (2 + 2 * k) * h, 2 * w, 2 * h);
					g.drawString(Integer.toString(k), width + w + 3,
							(3 + 2 * k) * h);
				}

				ImageIO.write(bi, "PNG", new File(fileName));
				System.err.print("\r" + fileName + " is written.");
			} catch (IOException ie) {
				ie.printStackTrace();
			}

		}
		System.err.println("\nThis is the end");
	}

}