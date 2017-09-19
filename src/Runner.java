package MarcsApplet;

public class Runner extends Thread {

	APL01 mParent=null;
	public Runner(APL01 parent)
	{
		mParent = parent;
		this.start();
	}
    public void run() {
        Thread me = Thread.currentThread();
        while (true) {
            mParent.repaint();
            try {
                me.sleep(10);
            } catch (InterruptedException e) {
                break;
            }
        }
        me = null;
    }
}
