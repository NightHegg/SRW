#ifndef VECTOR_SCHWARZ_HPP
#define VECTOR_SCHWARZ_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "vector.hpp"

using namespace std;

class VectorSchwarz : public Vector
{
private:
	std::vector<int> SchwarzNodes;
	int LeftBoundary, RightBoundary;
	bool UsingMethodSchwarz;

public:
	VectorSchwarz();
	VectorSchwarz(int i);

	void Construct(int i);

	void Decomposition(int Amount_Subdomains, double *Coef_Overflow);
	VectorSchwarz CreateAllocatedArray(int SchwarzStep);
	void ReturnAllocatedArrayResults(VectorSchwarz &yChosen, int SchwarzStep);
	void Equal_SchwarzNodes(VectorSchwarz &v);

	bool Condition_Schwarz();
	bool Compare_Boundary_Left(VectorSchwarz &v);
	bool Compare_Boundary_Right(VectorSchwarz &v);

	double ConvergenceL2(VectorSchwarz &yprev, VectorSchwarz &rr);

	double ScalarProduct(VectorSchwarz &a, VectorSchwarz &b);

	friend VectorSchwarz operator-(VectorSchwarz &a, VectorSchwarz &b);
	friend VectorSchwarz operator+(VectorSchwarz &a, VectorSchwarz &b);

	friend VectorSchwarz operator*(double val, VectorSchwarz &u);
};

VectorSchwarz::VectorSchwarz() : Vector::Vector()
{
	SchwarzNodes = std::vector<int>();
	LeftBoundary = RightBoundary = 0;
	UsingMethodSchwarz = false;
}

VectorSchwarz::VectorSchwarz(int i) : Vector(i)
{
	SchwarzNodes = std::vector<int>();
	LeftBoundary = 0;
	RightBoundary = iV - 1;
	UsingMethodSchwarz = false;
}

void VectorSchwarz::Construct(int i)
{
	Vector::Construct(i);
	LeftBoundary = 0;
	RightBoundary = iV - 1;
	UsingMethodSchwarz = false;
}

void VectorSchwarz::Decomposition(int Amount_Subdomains, double *Coef_Overflow)
{
	setlocale(LC_ALL, "Russian");
	int Count{0};
	int UserChoice{0};
	int Center_Subdomain{0};
	double TestValue1{0};
	double TestValue2{0};
	double CoefChosen{0};
	std::vector<double> CoefSuitable;
	std::vector<double> CoefVariants;
	for (int i = 1; i < 49; i++)
	{
		CoefVariants.push_back(0.01 * i);
	}
	double Length_Subdomain{V[iV / Amount_Subdomains] - V[0]};
	for (auto it : CoefVariants)
	{
		for (int i = 1; i < iV / Amount_Subdomains; i++)
		{
			if (fabs(V[i] - V[0] - it * Length_Subdomain) < 1e-15)
			{
				CoefSuitable.push_back(it);
			}
		}
	}
	if (CoefSuitable.size() > 1)
	{
		printf("There are few coefs for choice:\n");
		for (auto it : CoefSuitable)
		{
			printf("%d %.2f\n", ++Count, it);
		}
		printf("Choose one of coefs:\n");
		scanf_s("%d", &UserChoice);
		printf("\n");
		CoefChosen = CoefSuitable.at(UserChoice - 1);
		*Coef_Overflow = CoefChosen;
	}
	else if (CoefSuitable.size() == 1)
	{
		printf("There is only one coef for choice: %.2f\n", CoefSuitable.front());
		CoefChosen = CoefSuitable.front();
		*Coef_Overflow = CoefChosen;
	}
	else
	{
		printf("Coefs from 0.1 to 0.4 are inavailable!\n");
		system("PAUSE");
	}
	SchwarzNodes.push_back(0);
	for (int i = 1; i < Amount_Subdomains; i++)
	{
		Center_Subdomain = iV * i / Amount_Subdomains;
		TestValue1 = V[Center_Subdomain] - CoefChosen * Length_Subdomain;
		TestValue2 = V[Center_Subdomain] + CoefChosen * Length_Subdomain;
		for (int j = 0; j < iV; j++)
		{
			if (fabs(V[j] - TestValue1) < 1e-15 || fabs(V[j] - TestValue2) < 1e-15)
			{
				SchwarzNodes.push_back(j);
			}
		}
	}
	SchwarzNodes.push_back(iV - 1);
}

VectorSchwarz VectorSchwarz::CreateAllocatedArray(int SchwarzStep)
{
	int LeftB, RightB;
	if (SchwarzStep == 0)
	{
		LeftB = SchwarzNodes[SchwarzStep];
		RightB = SchwarzNodes[SchwarzStep + 2];
	}
	else if (SchwarzStep == (SchwarzNodes.size() / 2) - 1)
	{
		LeftB = SchwarzNodes[SchwarzStep * 2 - 1];
		RightB = SchwarzNodes[SchwarzStep * 2 + 1];
	}
	else
	{
		LeftB = SchwarzNodes[SchwarzStep * 2 - 1];
		RightB = SchwarzNodes[SchwarzStep * 2 + 2];
	}
	int AmountElements = RightB - LeftB + 1;
	VectorSchwarz A(AmountElements);
	for (int i = 0; i < A.iV; i++)
	{
		A.V[i] = V[i + LeftB];
	}
	A.LeftBoundary = LeftB;
	A.RightBoundary = RightB;
	return A;
}

void VectorSchwarz::ReturnAllocatedArrayResults(VectorSchwarz &yChosen, int SchwarzStep)
{
	for (int i = 0; i < yChosen.iV; i++)
	{
		V[i + yChosen.LeftBoundary] = yChosen.V[i];
	}
}

void VectorSchwarz::Equal_SchwarzNodes(VectorSchwarz &v)
{
	for (auto it : v.SchwarzNodes)
	{
		SchwarzNodes.push_back(it);
	}
	UsingMethodSchwarz = true;
}

bool VectorSchwarz::Condition_Schwarz()
{
	return (UsingMethodSchwarz == true ? true : false);
}

bool VectorSchwarz::Compare_Boundary_Left(VectorSchwarz &v)
{
	return (LeftBoundary == v.LeftBoundary ? true : false);
}

bool VectorSchwarz::Compare_Boundary_Right(VectorSchwarz &v)
{
	return (RightBoundary == v.RightBoundary ? true : false);
}

double VectorSchwarz::ConvergenceL2(VectorSchwarz &yprev, VectorSchwarz &rr)
{
	double s{0};
	double h = rr[1] - rr[0];
	double Length = rr[rr.iV - 1] - rr[0];
	double sum{0.0};
	for (int i = 0; i < iV; i++)
	{
		if (i == 0)
		{
			s = (rr.V[i + 1] - rr.V[i]) / 2;
		}
		else if (i == iV - 1)
		{
			s = (rr.V[i] - rr.V[i - 1]) / 2;
		}
		else
		{
			s = (((rr.V[i] - rr.V[i - 1]) / 2) + ((rr.V[i + 1] - rr.V[i]) / 2)) / 2;
		}
		sum += pow((V[i] - yprev.V[i]) / V[i], 2) * s * 1.0 / Length * 1.0;
	}
	return sqrt(sum);
}

double VectorSchwarz::ScalarProduct(VectorSchwarz &a, VectorSchwarz &b)
{
	double sum{0};
	for (int i = 0; i < a.GetSize(); i++)
	{
		sum += a.GetElement(i) * b.GetElement(i);
	}
	return sum;
}

VectorSchwarz operator-(VectorSchwarz &a, VectorSchwarz &b)
{
	VectorSchwarz c(a.GetSize());
	for (int i = 0; i < c.GetSize(); i++)
		c.V[i] = a[i] - b[i];
	return c;
}

VectorSchwarz operator+(VectorSchwarz &a, VectorSchwarz &b)
{
	VectorSchwarz c(a.GetSize());
	for (int i = 0; i < c.GetSize(); i++)
		c.V[i] = a[i] + b[i];
	return c;
}

VectorSchwarz operator*(double val, VectorSchwarz &u)
{
	VectorSchwarz v(u.GetSize());
	for (int i = 0; i < v.GetSize(); i++)
		v[i] = val * u[i];
	return v;
}

#endif