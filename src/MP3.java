/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package MarcsApplet;


import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.InputStream;
import javazoom.jl.player.Player;
/**
 * 
 * @author landev
 */
public class MP3 {
    private String filename;
    private Player player; 

    // constructor that takes the name of an MP3 file
    /**
     * Constructor
     * @param filename the file to play
     */
    public MP3(String filename) {
        this.filename = filename;
    }

    /**
     * Closes the player
     */
    public void close() { if (player != null) player.close(); }

    // play the MP3 file to the sound card
    
    /**
     * For playing a stream in a seperate thread
     * @param is represents the InputStream to play
     */
    public void playStream(InputStream is)
    {
          try {
            BufferedInputStream bis = new BufferedInputStream(is);
            player = new Player(bis);
        }
        catch (Exception e) {
            System.out.println("Problem playing file " + filename);
            System.out.println(e);
        }

        // run in new thread to play in background
        new Thread() {
            public void run() {
                try { player.play(); }
                catch (Exception e) { System.out.println(e); }
            }
        }.start();
    }
    
    
    /**
     * Plays the file passed to the constructor
     */
    public void play() {
        try {
            FileInputStream fis     = new FileInputStream(filename);
            BufferedInputStream bis = new BufferedInputStream(fis);
            player = new Player(bis);
        }
        catch (Exception e) {
            System.out.println("Problem playing file " + filename);
            System.out.println(e);
        }

        // run in new thread to play in background
        new Thread() {
            public void run() {
                try { player.play(); }
                catch (Exception e) { System.out.println(e); }
            }
        }.start();
    }
}