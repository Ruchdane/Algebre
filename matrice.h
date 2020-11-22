#ifndef MATRICERUCHDANE
#define MATRICERUCHDANE

#include <stdio.h>
#include <stdlib.h>  

typedef struct Matrice
{
	int col;
	int lin;
	float** matrice;
}Matrice;

float** AlloueTableau(int nombreDeligne, int nombreDeColonne);

Matrice* AlloueMatrice(int lin, int col);

void LibereTableau(float** tableau, int lin);

void Liberematrice(Matrice* matrice);

void MultiplyLine(Matrice* matrice, float value, int line);


void Multiply(Matrice* matrice, float value);

void Add(Matrice* matrice, int destination, int line, float facteurDestination, float facteurLine);

void Switch(Matrice* matrice, int destination, int line);

int Pivot(float* T, int col);

int Creux(float* T, int col);

float Determinant(int n, float** matrice);

float** SousMatrice(int n, float** matrice, int ls, int cs);

Matrice* InitMatrice();

void PrintMarice(Matrice matrice);


#endif // !MATRICERUCHDANE