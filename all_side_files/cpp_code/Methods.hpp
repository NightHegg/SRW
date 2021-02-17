#ifndef METHODS_HPP
#define METHODS_HPP

#include <cmath>
#include <string>
#include <iostream>
#include "classes.hpp"

using namespace std;

void TridiogonalMatrixAlgorithmRight(MatrixSchwarz &A, VectorSchwarz &F, VectorSchwarz &y)
{
	int N = A.GetSize_i();
	double denom;
	double *Alpha = new double[N - 1];
	double *Beta = new double[N];
	Alpha[0] = (-A[0][1]) / (A[0][0]);
	Beta[0] = (F[0]) / (A[0][0]);

	for (int i = 1; i < N - 1; i++)
	{
		denom = A[i][i] + (A[i][i - 1] * Alpha[i - 1]);
		Alpha[i] = (-A[i][i + 1]) / (denom * 1.0);
	}
	for (int i = 1; i < N; i++)
	{
		denom = A[i][i] + (A[i][i - 1] * Alpha[i - 1]);
		Beta[i] = (F[i] - A[i][i - 1] * Beta[i - 1]) / (denom * 1.0);
	}
	y[N - 1] = Beta[N - 1];
	for (int i = N - 2; i >= 0; i--)
	{
		y[i] = (y[i + 1] * Alpha[i]) + Beta[i];
	}
}

void TridiogonalMatrixAlgorithmLeft(MatrixSchwarz &A, VectorSchwarz &F, VectorSchwarz &y)
{
	int N = A.GetSize_i();
	double denom;
	double *Dzeta = new double[N];
	double *Eta = new double[N];

	Dzeta[N - 1] = (-A[N - 1][N - 2]) / (A[N - 1][N - 1]);
	Eta[N - 1] = F[N - 1] / (A[N - 1][N - 1]);

	for (int i = N - 2; i > 0; i--)
	{
		denom = A[i][i] + (Dzeta[i + 1] * A[i][i + 1]);
		Dzeta[i] = -A[i][i - 1] / (denom * 1.0);
	}
	for (int i = N - 2; i >= 0; i--)
	{
		denom = A[i][i] + (Dzeta[i + 1] * A[i][i + 1]);
		Eta[i] = (F[i] - Eta[i + 1] * A[i][i + 1]) / (denom * 1.0);
	}
	y[0] = Eta[0];
	for (int i = 1; i < N; i++)
	{
		y[i] = (Dzeta[i] * y[i - 1]) + Eta[i];
	}
}

void GaussianElimination(MatrixSchwarz &A, VectorSchwarz &F, VectorSchwarz &y)
{
	double buf{0}, sum{0};
	int N = A.GetSize_i();
	for (int i = 0; i < N; i++)
	{
		buf = A[i][i];
		for (int j = 0; j < N; j++)
		{
			A[i][j] /= buf;
		}
		F[i] /= buf;
		for (int k = i + 1; k < N; k++)
		{
			buf = A[k][i];
			for (int j = i; j < N; j++)
			{
				A[k][j] -= A[i][j] * buf;
			}
			F[k] -= F[i] * buf;
		}
	}
	y[N - 1] = F[N - 1];

	for (int i = N - 2; i >= 0; i--)
	{
			for (int j = i + 1; j < N; j++)
			{
				sum += A[i][j] * y[j];
			}
			y[i] = F[i] - sum;
			sum = 0;
	}
}

void ConjugateGradientMethod(MatrixSchwarz &A, VectorSchwarz &b, VectorSchwarz &y)
{
	int iter{0};
	double alpha{0}, beta{0};
	VectorSchwarz yPrevious(y.GetSize());
	y.Fill(0);
	VectorSchwarz r, rPrevious, p, tmp, tmp2;
	r = b - A * y;

	p = r;
	do
	{
		alpha = (y.ScalarProduct(r, r)) / (y.ScalarProduct(A * p, p));

		yPrevious = y;
		y = y + alpha * p;

		rPrevious = r;
		tmp = A * p;

		r = r - alpha * tmp;

		beta = y.ScalarProduct(r, r) / y.ScalarProduct(rPrevious, rPrevious);
		p = r + beta * p;

		iter++;
	} while (iter < 5);
}

#endif