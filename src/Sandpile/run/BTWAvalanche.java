package run;

import btw.*;
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
 *          This program exports every state, including the states with
 *          sandpile-height higher than the threshold. Non-recursive method for
 *          state calculation is used, so that there is no limitation of
 *          iterations (number of exported pictures).
 */
public class BTWAvalanche {
	static Color color[] = { Color.white, Color.cyan, Color.blue, Color.green,
			Color.black, Color.yellow, Color.orange, Color.red, Color.magenta };

	static int nx = 51;
	static int ny = 51;
	static BTWModel model = new BTWModel(nx, ny);

	/**
	 * @param args
	 *            Not used
	 */
	public static void main(String[] args) {
		String fileName;
		int MAX_ITERATIONS = 10001;
		for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
			model.Update();
			fileName = String.format("btwav-%1$06d.png", iter);

			try {
				int width = 600, height = 600;
				int w = width / ny;
				int h = height / nx;

				BufferedImage bi = new BufferedImage(width, height,
						BufferedImage.TYPE_INT_ARGB);

				Graphics2D g = bi.createGraphics();

				for (int i = 0; i < model.nx; i++) {
					for (int j = 0; j < model.ny; j++) {
						g.setColor(color[model.matrix[i][j] % 6]);
						g.fillRect(j * w, i * h, w, h);
					}
				}
				ImageIO.write(bi, "PNG", new File(fileName));
				System.err.print("\r" + fileName + " is written.");
			} catch (IOException ie) {
			}
		}
		System.err.println("\nThis is the end");
	}

}
