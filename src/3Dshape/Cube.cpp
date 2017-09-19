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
    public double[] P00 = {0, 0, 0};                //Eckpunkte
    public double[] P01 = {-0.5f, -0.5f, -0.5f};
    public double[] P02 = { 0.5f, -0.5f, -0.5f};
    public double[] P03 = { 0.5f,  0.5f, -0.5f};
    public double[] P04 = {-0.5f,  0.5f, -0.5f};

    public double[] P11 = {-0.5f, -0.5f, 0.5f};
    public double[] P12 = { 0.5f, -0.5f, 0.5f};
    public double[] P13 = { 0.5f,  0.5f, 0.5f};
    public double[] P14 = {-0.5f,  0.5f, 0.5f};

    public double[][] points = {P01, P02, P03, P04, P11, P12, P13, P14};

    public double[][] K01 = {P01, P02};              //Linien Zwischen Punkten
    public double[][] K02 = {P02, P03};
    public double[][] K03 = {P03, P04};
    public double[][] K04 = {P04, P01};
    public double[][] K05 = {P11, P12};
    public double[][] K06 = {P12, P13};
    public double[][] K07 = {P13, P14};
    public double[][] K08 = {P14, P11};
    public double[][] K09 = {P01, P11};
    public double[][] K10 = {P02, P12};
    public double[][] K11 = {P03, P13};
    public double[][] K12 = {P04, P14};
    public double[][][] W = {K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11, K12};       //Kompletter Würfel bestehend aus Linien
}

