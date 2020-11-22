#include "matrice.h"

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
		tableau[i] = malloc(col * sizeof(float));
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
	int i;
	for (; i < matrice->lin; i++)
		MultiplyLine(matrice, value, i);
}

void Add(Matrice* matrice, int destination, int line, float facteurDestination, float facteurLine)
{
	int i;
	float** T = matrice->matrice;
	for (i = 0; i < matrice->col; i++)
		T[destination][i] = facteurDestination * T[destination][i] + facteurLine * T[destination][i];
}

void Switch(Matrice* matrice, int destination, int line)
{
	float *tmp,** T = matrice->matrice;
	tmp = T[line];
	T[line] = T[destination];
	T[destination]= tmp;
}

int pivot(float* T, int col)
{
	int i = 0;
	for (; i < col; i++)
		if (T[i])
			break;
	return i;
}

int creux(float* T, int col)
{
	int i = 0, result = 0;
	for (; i < col; i++)
		if (T[i] == 0)
			result++;
	return result;
}

float determinant(int n, float** matrice)
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
			det += (2*(i % 2) - 1) * matrice[0][i] * determinant(n - 1, sousMatrice);
			/*i=0 <=> -1 else 1*/
		}
	}
	LibereTableau(sousMatrice, n - 1);
	return det;
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

#pragma endregion

#pragma region InputOperation

Matrice* InitMatrice()
{
	#pragma warning(disable : 4996)
	int i, j;
	Matrice* matrice;
	printf("Dimension de la matrice \n\tnombre de Line :");
	scanf_s("%d", &j);
	printf("\tnombre de Colonne :");
	scanf("%d", &i);
	matrice = AlloueMatrice(i, j);
	for (i = 0; i != matrice->lin; i++)
		for (j = 0; j != matrice->col; j++)
		{
			scanf("%f", &matrice->matrice[i][j]);
		}
	return matrice;
}

void PrintMarice(Matrice matrice)
{
	int i, j;
	for (i = 0; i != matrice.lin; i++, printf("\n"))
		for (j = 0; j != matrice.col; j++)
			printf("%f ", matrice.matrice[i][j]);
}
#pragma endregion
