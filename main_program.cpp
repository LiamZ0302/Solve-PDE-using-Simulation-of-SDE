#include<iostream>
#include<cmath>
#include<ctime>
#include<random>
#include<vector>
#include<fstream>
using namespace std;

double integration(double x0, double y0, double dt)
{
	random_device generator;
	vector<double> X;
	vector<double> Y;
	double x = x0;
	double y = y0;
	double dW1 = 0;
	double dW2 = 0;
	X.push_back(x);
	Y.push_back(y);
	
	normal_distribution<double> dist(0, sqrt(dt));

	while(1)
	{
		
		dW1 = dist(generator);
		dW2 = dist(generator);
		x += (x * dt + dW1);
		y += (y * dt + dW2);

		if (x * x + y * y > 1)
		{
			break;
		}
		else
		{
			X.push_back(x);
			Y.push_back(y);
		}
	}

	double integral = 0;
	while(!X.empty() && !Y.empty())
	{
		x = X.back();
		y = Y.back();
		integral += (x * x + y * y + 1) * dt;
		X.pop_back();
		Y.pop_back();
	}

	return integral;
}

double evaluation(double x0, double y0, int N, double dt)//N为MC次数
{
	double sum_integral = 0;
	double E_integral = 0;
	int i = 0;
	for (i = 0; i < N; ++i)
		sum_integral += integration(x0, y0, dt);
	E_integral = sum_integral / (i + 1);

	return 0.5 - E_integral;
}

int main()
{
 	
	random_device generator;
	uniform_real_distribution<double> dis_r(0, 1);
	uniform_real_distribution<double> dis_theta(0, 2 * M_PI);
	double r = sqrt(dis_r(generator));
	double theta = dis_theta(generator);
	double x0 = r * cos(theta);
	double y0 = r * sin(theta);
	double result = 0;
	double rel_error = 0;
	double truth = 0;
	double dts[10] = {1e-6, 9e-6, 2.5e-5, 4.9e-5, 8.1e-5, 1.21e-4, 1.69e-4, 2.25e-4, 2.89e-4, 3.61e-4};
	ofstream fout("output.txt");


	for (int i = 0; i < 10; ++i)
	{
		truth = (x0 * x0 + y0 * y0) / 2;
		result = evaluation(x0, y0, 10000, dts[i]);
		rel_error = abs(result - truth)/abs(truth);
		cout <<"step size:" << dts[i]<<endl;
		fout << dts[i] << " ";
		cout <<"coordinate:("<< x0 <<","<< y0 <<")"<<endl;
		cout <<"simulation result:"<< result << endl;
		cout <<"true value:"<< truth << endl;

		cout <<"relative error"<< rel_error <<endl;
		fout << rel_error <<" "<<endl;
	}
	fout.close();
	return 0;
}