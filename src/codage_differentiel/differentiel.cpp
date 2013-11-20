#include<iostream>

using namespace std;

int const width = 6;
int const height = 5;
int matrix[height][width] = {{125,212,22,30,58,136}, {15,112,38,110,236,114}, {125,112,58,106,95,22}, {68,145,36,234,45,36}, {36,25,14,215,31,115}};

void diff_encode(int taille_blocs, int mat[][6], int width, int height) {
	int delta = 0;
	int moy = 0;
	int sum = 0;
	int k = 0;
	int i = 0;
	int widthmoy = 0;
	int differences[height][width];
	int imagedecodee[height][width];
	int m = 0;

	// Calcul de la dimension de la matrice des moyennes
	int wmodulo = width%taille_blocs;
	if(wmodulo == 0) widthmoy = width/taille_blocs;
	else widthmoy = width/taille_blocs+1;

	int moyennes[height][widthmoy];

	// Calcul de la matrice des moyennes
	while(k < height) {
		sum += mat[k][i];
		i++;
		if(i == taille_blocs) {
			moy = sum/taille_blocs;
			moyennes[k][m] = moy;
			m++;
			sum = 0;
		}
		if(i == width) {
			moy = sum/(i%taille_blocs);
			moyennes[k][m] = moy;
			m = 0;
			sum = 0;
			k++;
			i = 0;
		}
	}

	k = 0;
	i = 0;	

	cout << endl;

	// Calcul de la matrice différentielle
	while(k < height) {
		differences[k][i] = mat[k][i] - moyennes[k][i/taille_blocs];
		i++;
		if(i == width) {
			k++;
			i = 0;
		}
	}

	cout << "Matrice des pixels" << endl;
	// Affichage de la matrice des pixels
	for(int x = 0; x < height; x++) {
		for(int y = 0; y < width; y++) {
			cout << mat[x][y] << " ";
		}
		cout << endl;
	}

	cout << endl;

	cout << "Matrice des moyennes" << endl;
	// Affichage de la matrice des moyennes
	for(int x = 0; x < height; x++) {
		for(int y = 0; y < widthmoy; y++) {
			cout << moyennes[x][y] << " ";
		}
		cout << endl;
	}

	cout << endl;

	cout << "Matrice des différences" << endl;
	// Affichage de la matrice des differences
	for(int x = 0; x < height; x++) {
		for(int y = 0; y < width; y++) {
			cout << differences[x][y] << " ";
		}
		cout << endl;
	}

	cout << endl;

	k = 0;
	i = 0;

	// DECODAGE
	// Calcul de la matrice image
	while(k < height) {
		imagedecodee[k][i] = differences[k][i] + moyennes[k][i/taille_blocs];
		i++;
		if(i == width) {
			k++;
			i = 0;
		}
	}

	cout << "Matrice image décodée" << endl;
	// Affichage de la matrice image
	for(int x = 0; x < height; x++) {
		for(int y = 0; y < width; y++) {
			cout << imagedecodee[x][y] << " ";
		}
		cout << endl;
	}
	
}

/*int main() {
	diff_encode(4, matrix, width, height);
}*/
