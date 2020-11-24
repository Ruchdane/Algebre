#include "matrice.h"

#pragma region Utiliter
int PlusouMoin(int i)
{
	/*i%2 = 0 <=> -1*/
	/*i%2 = 1 <=>  1*/
	return (2 * (i % 2) - 1);
}
#pragma endregion

#pragma region Allocation
float** AlloueTableau(int lin, int col)
{
	int i;
	float** tableau = malloc(lin * sizeof(float*));
	if (tableau == NULL)
	{
		printf("tableau non alloue");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < lin; i++)
	{
		tableau[i] = calloc(col, sizeof(float));
		if (tableau[i] == NULL)
		{
			printf("tableau non alloue");
			exit(EXIT_FAILURE);
		}
	}
	return tableau;
}

Matrice* AlloueMatrice(int lin, int col)
{
	Matrice* matrice = malloc(sizeof(Matrice));
	if (matrice == NULL)
	{
		printf("matrice non alloue");
		exit(EXIT_FAILURE);
	}
	matrice->lin = lin;
	matrice->col = col;
	matrice->matrice = AlloueTableau(matrice->lin, matrice->col);
	return matrice;
}

void LibereTableau(float** tableau, int lin)
{
	int i;
	for (i = 0; i < lin; i++)
		free(tableau[i]);
	free(tableau);
}

void Liberematrice(Matrice* matrice)
{
	LibereTableau(matrice->matrice, matrice->lin);
	free(matrice);
}
#pragma endregion

#pragma region Operation

void MultiplyLine(Matrice* matrice, float value, int line)
{
	int i;
	for (i = 0; i < matrice->col; i++)
		matrice->matrice[line][i] *= value;
}

void Multiply(Matrice* matrice, float value)
{
	int i=0;
	for (; i < matrice->lin; i++)
		MultiplyLine(matrice, value, i);
}

void Add(Matrice* matrice, int destination, int line, float facteurDestination, float facteurLine)
{
	int i;
	float** T = matrice->matrice;
	for (i = 0; i < matrice->col; i++)
	{
		if(facteurDestination==1)
			T[destination][i] += facteurLine * T[line][i];
		else
		T[destination][i] = facteurDestination * T[destination][i] + facteurLine * T[line][i];
	}
}

void Switch(Matrice* matrice, int destination, int line)
{
	float *tmp,** T = matrice->matrice;
	tmp = T[line];
	T[line] = T[destination];
	T[destination]= tmp;
}

int Pivot(float* T, int col)
{
	int i = 0;
	for (; i < col; i++)
		if (T[i])
			break;
	return i;
}

int Creux(float* T, int col)
{
	int i = 0, result = 0;
	for (; i < col; i++)
		if (T[i] == 0)
			result++;
	return result;
}

float Determinant(int n, float** matrice)
{
	int i;
	float det = 0, ** sousMatrice=NULL;
	if (n == 1)
		return matrice[0][0];
	else
	{
		for (i = 0; i != n; i++)
		{
			sousMatrice = SousMatrice(n, matrice, 0, i);
			det += PlusouMoin(i) * matrice[0][i] * Determinant(n - 1, sousMatrice);
		}
	}
	LibereTableau(sousMatrice, n - 1);
	return det;
}

void Echelonner(Matrice* matrice)
{
	int i, j, k;
	float** T = matrice->matrice;
	//Tri par selection suivant la valeur fu pivot 
	//Pour ceux qui ont le meme pivaot on prend le plus creux
	for (i = 1; i < matrice->lin; i++)
		for (j = i; j > 0; j--)
		{
			k = Pivot(T[j], matrice->col) - Pivot(T[j - 1], matrice->col);
			if (k < 0 || k == 0 && Creux(T[j], matrice->col) > Creux(T[j - 1], matrice->col))
				Switch(matrice, j, j - 1);
			else
				break;
		}

	for (i = 0; i < matrice->col; i++)
	{

		k = Pivot(T[i], matrice->col);
		if (k < matrice->col)
		{
			if (T[i][k] && T[i][k] != 1)
				MultiplyLine(matrice, 1 / T[i][k], i);
			for (j = i + 1; j < matrice->lin; j++)
				if (T[j][k] != 0)
					Add(matrice, j, i, 1, -T[j][k]);
		}
	}
}


#pragma endregion

#pragma region MatriceOutputOperation

float** SousMatrice(int n, float** matrice, int ls, int cs)
{
	float** sousMatrice = AlloueTableau(n - 1, n - 1);
	int lsm /*ligne de la sous matrice*/, csm /*colone de la sous matrice*/,
		lm /*ligne de la matrice*/, cm /*colone de la matrice*/;

	for (lsm = 0, lm = 0; lsm < n - 1; lsm++, lm++)
		for (csm = 0, cm = 0; csm < n - 1; csm++, cm++)
		{
			if (cm == cs) cm++;
			if (lm == ls) lm++;
			sousMatrice[lsm][csm] = matrice[lm][cm];
		}
	return sousMatrice;
}

Matrice* InverseDet(Matrice matrice)
{
	int i,j;
	float** T = matrice.matrice,det;
	Matrice* result = NULL;
	if (matrice.col != matrice.lin)
		return result;
	result = AlloueMatrice(matrice.lin, matrice.col);
	if (result == NULL)
		return result;
	det = Determinant(matrice.lin, T);
	if(det!=0)
		for (i = 0; i < matrice.lin; i++)
			for (j = 0; j < matrice.col; j++)
				result->matrice[j][i] =  ( 1 / det ) * PlusouMoin(i+j) * Determinant(matrice.lin - 1,SousMatrice( matrice.lin ,T, i, j));
	else
	{
		Liberematrice(result);
		result = NULL;
	}
	return result;
}

Matrice* Produit(Matrice A, Matrice B)
{
	Matrice* result = NULL;
	int i, j, k;
	if (A.col != B.lin)
		return result;
	result = AlloueMatrice(A.lin, B.col);
	for (i = 0; i < result->lin; i++)
		for (j = 0; j < result->col; j++)
			for (k = 0; k < A.col; k++)
				result->matrice[i][j] += A.matrice[i][k] * B.matrice[k][j];
	return result;
}


#pragma endregion

#pragma region InputOperation

Matrice* InitMatrice()
{
	#pragma warning(disable : 4996)
	int i, j;
	Matrice* matrice;
	printf("Dimension de la matrice \n\tnombre de Line :");
	scanf("%d", &i);
	printf("\tnombre de Colonne :");
	scanf("%d", &j);
	matrice = AlloueMatrice(i, j);
	for (i = 0; i != matrice->lin; i++)
		for (j = 0; j != matrice->col; j++)
		{
			scanf("%f", &matrice->matrice[i][j]);
		}
	return matrice;
}

void PrintMatrice(Matrice matrice)
{
	int i, j;
	for (i = 0; i != matrice.lin; i++, printf("\n"))
		for (j = 0; j != matrice.col; j++)
			printf("%f ", matrice.matrice[i][j]);
}
#pragma endregion
