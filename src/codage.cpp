/* 
 * File:   codage.cpp
 * Author: Gildas
 *
 * Created on 23 octobre 2013, 14:46
 */

#include <cstdlib>
#include <iostream>

using namespace std;

/* Function to calculate the average of 4 neighbors pixels
 *
 */
float moyenne(int raw[][4], int x, int y){
    float moyenne = raw[x][y] + raw[x+1][y] + raw[x][y+1] + raw[x+1][y+1];
    
    moyenne = moyenne/4;
    
    return moyenne;
}


/* Function to code 
 *
 */
void divResolution(int raw[][4], int width, int height) {
    float compress[width/2][height/2];

    for (int i=0; i<=width-1; i=i+2)
    {
        for (int j=0; j<=height-1; j=j+2)
        {	    
            if( i != 0 && j != 0)
                compress[i/2][j/2] = moyenne(raw, i, j);
            else if( i == 0 )
                compress[0][j/2] = moyenne(raw, 0, j);
	    else
		compress[i/2][0] = moyenne(raw, i, 0);
        }
    }

    for(int i=0; i<width; i++){
        for (int j=0; j<height; j++){
            cout << "\t|\t" << raw[i][j]  ;
        }
	cout << "\t|" << endl;
    }
    
    cout << endl << endl;
    
    for(int i=0; i<2; i++){
        for (int j=0; j<2; j++){
            cout << "\t|\t" << compress[i][j]  ;
        }
	cout << "\t|" << endl;
    }
}    



/* Function to decode  
 *
 */
void decodeDivResolution(float compress[][3], int width, int height) {
    float raw[width*2][height*2];

    for (int i=0; i<=width-1; i++)
    {
        for (int j=0; j<=height-1; j++)
        {	    
            raw[2*i][2*j] = compress[i][j];
	    raw[(2*i)+1][2*j] = compress[i][j];
	    raw[2*i][(2*j)+1] = compress[i][j];
	    raw[(2*i)+1][(2*j)+1] = compress[i][j];
        }
    }

    
    for(int i=0; i<width; i++){
        for (int j=0; j<height; j++){
            cout << "\t|\t" << compress[i][j]  ;
        }
	cout << "\t|" << endl;
    }
    
    cout << endl << endl;
    
    for(int i=0; i<width*2; i++){
        for (int j=0; j<height*2; j++){
            cout << "\t|\t" << raw[i][j]  ;
        }
	cout << "\t|" << endl;
    }
}   




/*
 * Main
 */
int main(int argc, char** argv) {
    cout << "Matrices" << endl;
    
    // Matrice de test en dur
    cout << "codage matrice 4x4" << endl;
    int mat[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}}; 
    divResolution(mat, 4, 4);
    
    cout << endl << "décodage matrice 2x3" << endl;
    float mat2[3][3] = {{3.5,5.5,7.5},{11.5,13.5,15.5},{8,9,10}};
    decodeDivResolution(mat2, 3, 3);
    
    //decodeDivResolution(mat, 4, 4);
    
    cout << "FIN !" << endl;
    
    return 0;
}

