package MarcsApplet;

/*
 * Created by SharpDevelop.
 * User: Marc Jr. Landolt
 * Date: 22.08.2006
 * Time: 07::46
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
//nonsense comment
/// <summary>
/// Description of wuerfel.
/// </summary>

/**
 * A model of a Cube, first there are edge points, they are aggreagatet to lines,
 * and the lines are aggregatet to a cube built out of lines
 * @author landev
 */
public class Cube {

    /**
     * 
     */
    public static double[] P00 = {0, 0, 0};                //Eckpunkte
    public static double[] P01 = {-50, -50, -50};
    public static double[] P02 = {50, -50, -50};
    public static double[] P03 = {50, 50, -50};
    public static double[] P04 = {-50, 50, -50};

    public static double[] P11 = {-50, -50, 50};
    public static double[] P12 = {50, -50, 50};
    public static double[] P13 = {50, 50, 50};
    public static double[] P14 = {-50, 50, 50};

    public static double[][] points = {P01, P02, P03, P04, P11, P12, P13, P14};

    public static double[][] K01 = {P01, P02};              //Linien Zwischen Punkten
    public static double[][] K02 = {P02, P03};
    public static double[][] K03 = {P03, P04};
    public static double[][] K04 = {P04, P01};
    public static double[][] K05 = {P11, P12};
    public static double[][] K06 = {P12, P13};
    public static double[][] K07 = {P13, P14};
    public static double[][] K08 = {P14, P11};
    public static double[][] K09 = {P01, P11};
    public static double[][] K10 = {P02, P12};
    public static double[][] K11 = {P03, P13};
    public static double[][] K12 = {P04, P14};
    public static double[][][] W = {K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12};       //Kompletter Würfel bestehend aus Linien
}

