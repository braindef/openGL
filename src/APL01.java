package MarcsApplet;

import static java.lang.Math.PI;
import static java.lang.Math.cos;
import static java.lang.Math.sin;

import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Composite;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.InputStream;
import java.net.URL;

import javax.imageio.ImageIO;
import javax.swing.JApplet;

public class APL01 extends JApplet {

	   private BufferedImage bimg;
	    MP3 mp3;

	    
	    public void init() {
	        setBackground(Color.white);
	        start = System.currentTimeMillis();

	        InputStream stream = this.getClass().getResourceAsStream("Chavez - Ninja bolognese.mp3");   //
	        mp3 = new MP3(null);
	        mp3.playStream(stream);
	        try {
	            fgImage = getImage(new URL(this.getClass().getResource("background.jpg").toURI().toString()));
	        } catch (Exception ee) {
	            System.out.println("Image load error");
	        }
	  
	        for (int i = 0; i<100; i++)
	        {
	         if(getParameter("param"+i) != null) {
	        	 credits[i]=getParameter("param"+i);
	        }
	        else
	        	 credits[i]="";
	        }
	        
	        Runner runner = new Runner(this);

	    }
	    
	    protected static double[][] dmy = Matrix.yTurn(0.03);
	    protected static double[][] dmz = Matrix.zTurn(0.02);
	    protected static double[][] dmx = Matrix.xTurn(0.025);
	    protected static double[] center = Cube.P00;
	    public static double scale = 0.7;
	    public static double i = 1;
	    public double start;
	    public int time;
	    String[] credits = new String[100];
	    
	    Font H1 = new Font("Helvetica", Font.PLAIN, 16);
	    Font body = new Font("Helvetica", Font.PLAIN, 12);

	    public void drawDemo(int pWidth, int pHeight, Graphics2D g2) {
	        addAlphaImage(g2);
	        
	        scale = 1 + sin(PI / 2 + (double) (System.currentTimeMillis() - start) / 271 * 2) * 0.5;   //275
	        g2.setColor(new Color(190, 190, 30));
	        int nx = 40;
	        int ny = 20;
	        Dimension d = getSize();
	        int rx = d.width / nx;
	        int ry = d.height / ny;

	        for (int x = 0; x < nx; x++) {
	            for (int y = 0; y < ny; y++) {
	                System.currentTimeMillis();
	                g2.setColor(new Color(255, 255, (int) (127 * (cos((x + y)) + 1))));
	                g2.drawOval(x * rx, 20 + y * ry,
	                        5 + (int) (rx * sin((x - nx / 2) + (y - ny / 2) * System.currentTimeMillis() / 1000.0)),
	                        5 + (int) (ry * sin((x - nx / 2) + (y - ny / 2) * System.currentTimeMillis() / 1000.0)));

	            }
	        }
	        g2.translate(110, 120);
	        Cube.points = Matrix.mul(Cube.points, dmy);
	        Cube.points = Matrix.mul(Cube.points, dmz);
	        Cube.points = Matrix.mul(Cube.points, dmz);
	        Cube.points = Matrix.mul(Cube.points, dmx);
	        double[][] punkte = Matrix.scalarMul(Cube.points, 1 * cos(i * 0.04));

	        for (int l = 0; l < Cube.W.length; l++) {
	            Cube.W[l] = Matrix.mul(Cube.W[l], dmy);
	            Cube.W[l] = Matrix.mul(Cube.W[l], dmz);
	            Cube.W[l] = Matrix.mul(Cube.W[l], dmx);
	            g2.setColor(Color.cyan);
	            g2.drawLine((int) ((float) (center[0] + Cube.W[l][0][0] + Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[1] + Cube.W[l][0][1] + Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[0] + Cube.W[l][1][0] + Cube.W[l][1][2] * 0.1)),
	                    (int) ((float) (center[1] + Cube.W[l][1][1] + Cube.W[l][1][2] * 0.1)));
	            g2.setColor(Color.blue);
	            g2.drawLine((int) ((float) (center[0] + 0.98 * Cube.W[l][0][0] + Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[1] + 0.98 * Cube.W[l][0][1] + Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[0] + 0.98 * Cube.W[l][1][0] + Cube.W[l][1][2] * 0.1)),
	                    (int) ((float) (center[1] + 0.98 * Cube.W[l][1][1] + Cube.W[l][1][2] * 0.1)));
	            g2.setColor(Color.ORANGE);
	            g2.drawLine((int) ((float) (center[0] + scale * Cube.W[l][0][0] + scale * Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[1] + scale * Cube.W[l][0][1] + scale * Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[0] + scale * Cube.W[l][1][0] + scale * Cube.W[l][1][2] * 0.1)),
	                    (int) ((float) (center[1] + scale * Cube.W[l][1][1] + scale * Cube.W[l][1][2] * 0.1)));
	            g2.setColor(Color.red);
	            g2.drawLine((int) ((float) (center[0] + 0.97 * scale * Cube.W[l][0][0] + scale * Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[1] + 0.97 * scale * Cube.W[l][0][1] + scale * Cube.W[l][0][2] * 0.1)),
	                    (int) ((float) (center[0] + 0.97 * scale * Cube.W[l][1][0] + scale * Cube.W[l][1][2] * 0.1)),
	                    (int) ((float) (center[1] + 0.97 * scale * Cube.W[l][1][1] + scale * Cube.W[l][1][2] * 0.1)));
	        }



	        time = (int) (System.currentTimeMillis() - start);
	        g2.setColor(Color.BLACK);
	        g2.setFont(body);
	        for (int i = 0; i < credits.length; i++) {
	            g2.drawString(credits[i], (float) (130 + 10 * sin(i / 3.0 + time / 300.0)), (10 + i * 16 - (time / 30)) % (550 / 29 * credits.length) + 100 + (float) (sin(time / 100.0) * 5));

	        }
	        
	    }

	    
	    public Graphics2D createGraphics2D(int pWidth, int pHeight) {
	        Graphics2D g2 = null;
	        if (bimg == null || bimg.getWidth() != pWidth || bimg.getHeight() != pHeight) {
	            bimg = (BufferedImage) createImage(pWidth, pHeight);

	        }
	        g2 = bimg.createGraphics();
	        g2.setBackground(getBackground());
	        g2.clearRect(0, 0, pWidth, pHeight);
	        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
	                RenderingHints.VALUE_ANTIALIAS_ON);
	        return g2;
	    }
	    
	    
	    public void paint(Graphics g) {
	        Dimension d = getSize();
	        //step(d.width, d.height);
	        Graphics2D g2 = createGraphics2D(d.width, d.height);
	        drawDemo(d.width, d.height, g2);
	        //g2.dispose();
	        g.drawImage(bimg, 0, 0, this);
	    }

	    Image fgImage = null;

	    public void addAlphaImage(Graphics2D g2d) {

	        // if a foreground image exists, paint it
	        if (fgImage != null) {
	            int imageW = fgImage.getWidth(null);
	            int imageH = fgImage.getHeight(null);

	            Graphics g = this.getGraphics();
	            // we need to cast to Graphics2D for this operation
	            //Graphics2D g2d = (Graphics2D)g;

	            // create the composite to use for the translucency
	            AlphaComposite comp = AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 0.5f);

	            // save the old composite
	            Composite oldComp = g2d.getComposite();

	            // set the translucent composite
	            g2d.setComposite(comp);

	            // calculate the x and y positions to paint at
	            //int xloc = getComponent().getWidth() - imageW - 5;
	            //int yloc = getComponent().getHeight() - imageH - 5;

	            // paint the image using the new composite
	            g2d.drawImage(fgImage, 0, 0, null);
	            g2d.drawImage(fgImage, fgImage.getWidth(null), 0, null);
	            
	            // restore the original composite
	            g2d.setComposite(oldComp);

	        }
	    }

	    protected static Image getImag(URL imageURL) {
	        Image image = null;

	        try {
	            // use ImageIO to read in the image
	            image = ImageIO.read(imageURL);
	        } catch (Exception ioe) {
	            ioe.printStackTrace();
	        }

	        return image;
	    }
	    
}
