/*
 * Created by SharpDevelop.
 * User: Marc Jr. Landolt
 * Date: 07.09.2006
 * Time: 21::05
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
package MarcsApplet;

import static java.lang.Math.*;
/**
 * From .NET Framework Ported Matrix Class written by Marc Landolt
 * it is supposed for calculating matrix operations
 * 
 * @author landev
 */
public class Matrix {

    /**
     * for easier debugging
     */
    protected static boolean debug = false;
    /**
     * the Matrix itselfe as an double array^2
     */
    protected double[][] mMatrix;

    /**
     * Constructor that loads an double array into the Matrix
     * @param m1 the Matrix data
     */
    public Matrix(double[][] m1) {
        mMatrix = new double[m1.length][];	// WICHTIG, SO WIRD EIN 2-Dimensonales Array inizialisiert...
        for (int i = 0; i < m1.length; i++) /* oder so: int[][] matrix2 = { new int[] {1,2,3}, new int[] {2,3,4}, new int[] {3,4,5};*/ {
            this.mMatrix[i] = new double[m1[i].length];
            for (int j = 0; j < m1[i].length; j++) {
                this.mMatrix[i][j] = m1[i][j];
            }
        }
    }

    /**
     * Prints the Matrix in a userfriendly way on the console
     * @param m1
     */
    public static void toConsole(double[][] m1) {
        if (m1 == null) {
            System.out.println("NULL");
            return;
        }
        for (int i = 0; i < m1.length; i++) {
            for (int j = 0; j < m1[i].length; j++) {
                System.out.format("%5d", round(m1[i][j]));
            }
            System.out.println();
        }

    }

    /**
     * Adds two identical Matrices m1+m2 (http://de.wikipedia.org/wiki/Matrix_%28Mathematik%29)
     * @param m1 the first Matrix
     * @param m2 the second Matrix
     * @return retruns null if m1(n1,m1) m2(n2,m2) | n1!=n2 and m1!=m2
     * otherwise the added matrix is returned
     */
    public static double[][] add(double[][] m1, double[][] m2) {
        if (m1.length != m2.length) {
            return null;
        }
        for (int i = 0; i < m1.length; i++) {
            if (m1[i].length != m2[i].length) {
                return null;
            }
        }
        double[][] resultat = new double[m1.length][];
        for (int i = 0; i < m1.length; i++) {
            resultat[i] = new double[m1[i].length];
            for (int j = 0; j < m1[i].length; j++) {
                if (debug) {
                    System.out.println(i + " " + j + " " + m1[i][j] + m2[i][j] + " " + m1[i][j] + " " + +m2[i][j]);
                }
                resultat[i][j] = m1[i][j] + m2[i][j];
            }
        }
        return resultat;
    }

    /**
     * Subtracts two identical Matrices m1+m2 (http://de.wikipedia.org/wiki/Matrix_%28Mathematik%29)
     * @param m1 the first Matrix
     * @param m2 the second Matrix
     * @return retruns null if m1(n1,m1) m2(n2,m2) | n1!=n2 and m1!=m2
     * otherwise the added matrix is returned
     */
    public static double[][] sub(double[][] m1, double[][] m2) {
        if (m1.length != m2.length) {
            return null;
        }
        for (int i = 0; i < m1.length; i++) {
            if (m1[i].length != m2[i].length) {
                return null;
            }
        }
        double[][] resultat = new double[m1.length][];
        for (int i = 0; i < m1.length; i++) {
            resultat[i] = new double[m1[i].length];
            for (int j = 0; j < m1[i].length; j++) {
                if (debug) {
                    System.out.println(i + " " + j + " " + m1[i][j] + m2[i][j] + " " + m1[i][j] + " " + +m2[i][j]);
                }
                resultat[i][j] = m1[i][j] - m2[i][j];
            }
        }
        return resultat;
    }

    /**
     * Multiplyes a matrix with a scalar (http://de.wikipedia.org/wiki/Matrix_%28Mathematik%29)
     * @param m1 the matrix to multiply
     * @param scalar the scalar to multiply with
     * @return returns the multiplied matrix
     */
    public static double[][] scalarMul(double[][] m1, double scalar) {
        double[][] resultat = new double[m1.length][];
        for (int i = 0; i < m1.length; i++) {
            resultat[i] = new double[m1[i].length];
            for (int j = 0; j < m1[i].length; j++) {
                if (debug) {
                    System.out.println(m1[i][j] * scalar);
                }
                resultat[i][j] = m1[i][j] * scalar;
            }
        }
        return resultat;
    }

    /**
     * Multiplies two matrices with each other, exactly: the first with the second
     * (http://de.wikipedia.org/wiki/Matrix_%28Mathematik%29)
     * @param m1 the first matrix
     * @param m2 the second matrix
     * @return the result of the multiplication or null if something went wrong, e.g.
     * m1(n1,m1) m2(n2,m2) m1!=n2
     */
    public static double[][] mul(double[][] m1, double[][] m2) {
        if (m1[0].length != m2.length) {
            return null;
        }
        double[][] resultat = new double[m1.length][];
        for (int i = 0; i < m1.length; i++) {
            resultat[i] = new double[m2[0].length];
            for (int j = 0; j < m2[0].length; j++) {
                for (int k = 0; k < m2.length; k++) {
                    resultat[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }
        return resultat;
    }

    /**
     * Transposes a matrix, which is nothing else than exchanging (mirroring) the
     * lower with the upper triangle matrix, this works only with quadratic matrices
     * @param m1 the matrix to transpose
     * @return the transposed matrix
     */
    public static double[][] transpone(double[][] m1) //TODO Ungleiche Transponieren
    {
        if (m1.length != m1[0].length) {
            return null;
        }
        double[][] resultat = new double[m1.length][];
        for (int i = 0; i < m1.length; i++) {
            resultat[i] = new double[m1[0].length];
            for (int j = 0; j < m1[0].length; j++) {
                resultat[i][j] = m1[j][i];
            }
        }
        return resultat;

    }

    /**
     * gives the determinant of a quadratic matrix, which is computed by the "Laplacschen Entwicklungssatz"
     * @param m1 the matrix to compute
     * @return the determinant which is a scalar
     */
    public static double det(double[][] m1) //throws java.lang.Exception
    {
        if (m1.length != m1[0].length) {
            return Double.MIN_VALUE; //throw new java.lang.Exception("Determinante nicht Quadratisch");
        }
        if (m1.length < 3) {
            return m1[0][0] * m1[1][1] - m1[0][1] * m1[1][0];
        }

        double a = 0;

        double resultat = 0;
        for (int i = 0; i < m1[0].length; i++) {
            a = (double) pow(-1, i + 0) * m1[0][i];
            resultat += a * det(subDet(m1, i, 0));
        }
        return resultat;
    }

    /**
     * Calculates a subdeterminat form a given matrix at position x/y
     * and is supposed to use recursively in the "Laplacschen Entwicklungssatz"
     * (http://de.wikipedia.org/wiki/Laplacescher_Entwicklungssatz#Laplacescher_Entwicklungssatz)
     * @param s1 the matrix to be subdeterminanted
     * @param xPos the x position
     * @param yPos the y position
     * @return returns the subdeterminant
     */
    public static double[][] subDet(double[][] s1, int xPos, int yPos) {
        int k = 0;
        int l = 0;
        double[][] resultat = new double[s1.length - 1][];
        for (int i = 0; i < s1.length; i++) {
            if (i != yPos) {
                resultat[l++] = new double[s1[0].length - 1];
                for (int j = 0; j < s1[0].length; j++) {
                    if (j != xPos) {
                        resultat[l - 1][k++] = s1[i][j];
                    }
                }
                k = 0;
            }
        }

        if (debug) {
            toConsole(resultat);
        }
        return resultat;
    }

    /**
     * the adjoint matrix (http://de.wikipedia.org/wiki/Adjunkte)
     * @param s1 the matrix to compute
     * @return the resulting matrix
     */
    public static double[][] adj(double[][] s1) {
        double[][] resultat = new double[s1.length][];
        for (int i = 0; i < s1.length; i++) {
            resultat[i] = new double[s1[0].length];
            for (int j = 0; j < s1[0].length; j++) {
                resultat[i][j] = (double) pow(-1, i + j) * det(subDet(s1, i, j));
            }

        }

        return resultat;
    }

    /**
     * Calculates the inverse matrix
     * (http://en.wikipedia.org/wiki/Invertible_matrix)
     * @param m1 the matrix to inverse
     * @return the inversed matrix
     */
    public static double[][] inverse(double[][] m1) {
        return scalarMul(adj(m1), 1 / det(m1));
    }

    /**
     * Generates an identity Matrix
     * (http://en.wikipedia.org/wiki/Identity_matrix)
     * @param column number of rows
     * @return returns the identity matix with n columns
     */
    public static double[][] identity(int column) {
        double[][] resultat = new double[column][];
        for (int i = 0; i < column; i++) {
            resultat[i] = new double[column];
            resultat[i][i] = 1;
        }
        return resultat;
    }

    /**
     * Rotates in the x-plane in a 3D room
     * (http://de.wikipedia.org/wiki/Rotationsmatrix)
     * @param angle the angle to rotate
     * @return the rotation identity matrix
     */
    public static double[][] xTurn(double angle) {
        double[][] drehmatrix = {new double[]{1, 0, 0},
            new double[]{0, cos(angle), -sin(angle)},
            new double[]{0, sin(angle), cos(angle)}
        };
        return drehmatrix;
    }

    /**
     * Rotates in the y-plane in a 3D room
     * (http://de.wikipedia.org/wiki/Rotationsmatrix)
     * @param angle the angle to rotate
     * @return the rotation identity matrix
     */
    public static double[][] yTurn(double angle) {
        double[][] drehmatrix = {new double[]{cos(angle), 0, sin(angle)},
            new double[]{0, 1, 0},
            new double[]{-sin(angle), 0, cos(angle)}
        };
        return drehmatrix;
    }

    /**
     * Rotates in the z-plane in a 3D room
     * (http://de.wikipedia.org/wiki/Rotationsmatrix)
     * @param angle the angle to rotate
     * @return the rotation identity matrix
     */
    public static double[][] zTurn(double angle) {
        double[][] drehmatrix = {new double[]{cos(angle), -sin(angle), 0},
            new double[]{sin(angle), cos(angle), 0},
            new double[]{0, 0, 1},
        };
        return drehmatrix;
    }

    /**
     * Rotates arround a given identity vector in the 3D room
     * (http://de.wikipedia.org/wiki/Rotationsmatrix)
     * @param rotationAxis the axis to rotate arround
     * @return the rotation identity matrix
     */
    public static double[][] turnFree(double[] rotationAxis)
    {
        return new double[3][3];
    }
    
    /**
     * a main method to test all the methods
     * @param args commandline arguments
     */
    public static void main(String[] args) {
        try {
            Thread.sleep(1000);
        } catch (Exception e) {
        }
        double[][] m1 = {new double[]{1, 0, -1, 0.5}, new double[]{-8, 4, 1, 323}, new double[]{-2, 1, 0, -0.342}, new double[]{2, 1, -4, -0.2}};
        System.out.println("=====Ausgangsmatrix======");
        toConsole(m1);
        System.out.println("========Adjunkte=========");
        toConsole(adj(m1));
        System.out.println("=========Inverse=========");
        toConsole(inverse(m1));
        System.out.println("====Ausgang * Inverse====");
        toConsole(mul(m1, inverse(m1)));
        System.out.println("=====Einheitsmatrix=====");
        toConsole(identity(5));
    }
}
