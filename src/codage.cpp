/* 
 * File:   codage.cpp
 * Author: Gildas
 *
 * Created on 20 novembre 2013, 14:59
 */

#include <cstdlib>
#include <iostream>

using namespace std;

/* Function to calculate the average of 4 neighbors pixels
 *
 */
float moyenne(float** raw, int x, int y){
    float moyenne = raw[x][y] + raw[x+1][y] + raw[x][y+1] + raw[x+1][y+1];
    
    moyenne = moyenne/4;
    
    return moyenne;
}


/* Function to code 
 *
 */
void divResolution(float** raw, int width, int height) {
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
void decodeDivResolution(float** compress, int width, int height) {
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
    int width = 4, height = 4;
   
    cout << "codage matrice 4x4" << endl;
    float** mat;
    mat = new float*[height];			// nombre de lignes
    int indice = 0;				// utilisé juste pour remplir le tableau de valeurs
    
    for(int i=0; i<height; i++){		// pour chaque ligne : width colonnes
	mat[i] = new float[width];
	
	for(int j=0; j<width; j++, indice++)	// remplissage du tableau (0 a 15)
	    mat[i][j] = indice;
    }
    
    divResolution(mat, 4, 4);
    
    cout << endl << "décodage matrice 4x4" << endl;
    decodeDivResolution(mat, 4, 4);
        
    cout << "FIN !" << endl;
    
    return 0;
}

