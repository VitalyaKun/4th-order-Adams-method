#include <iostream>
#include <fstream>
#include <cmath>
#include <conio.h>

using namespace std;

double fxy(double x, double y)
{
    return 2.1 * y + x + 0.25;
}

double yExt(double* x, double* y, int i, double h)
{
    return h / 24. * (55 * fxy(x[i], y[i]) - 59 * fxy(x[i-1], y[i-1]) + 37 * fxy(x[i-2], y[i-2]) - 9 * fxy(x[i-3], y[i-3]));
}

double yInt(double* x, double* y, int i, double h)
{
    return h / 24. * (9 * fxy(x[i+1], y[i+1]) + 19 * fxy(x[i], y[i]) - 5 * fxy(x[i-1], y[i-1]) + fxy(x[i-2], y[i-2]));
}

int main()
{   
    std::ifstream in("in.txt");
    double a, b, h, y0, eps;
    in >> a >> b >> h >> y0 >> eps;

    int n = (b - a) / h + 1;
    double* y = new double[n + 6];
    double* y1 = new double[n];
    double* x = new double[n + 5];

    double f0, f1, f2, X = a, pow2 = 1, yE, yI, temp;
    double t = 0;

    for(int i = 0; i < n + 5; ++i)
    {
        x[i] = 0.1 * t;
        ++t;
    }

    //Эйлер
    for(int i = 1; i < n; ++i)
    {
        y[i] = y[i - 1] + h * fxy(a + h * (i - 1) + h / 2, y[i - 1] + h / 2 * fxy(X, y[i]));
        X += h;
    }

    do
    {
        for(int i = 0; i < n; ++i)
        {
            y1[i] = y[i];
        }

        y[0] = y0;
        h /= 2;
        pow2 *= 2;
        X = a;

        for(int i = 1; i < n; i++)
        {
            y[i] = y[i - 1] + h * fxy(a + h * (i - 1) * (pow2 - 1) + h / 2, y[i - 1] + h / 2 * fxy(X, y[i]));
            
            for(int j = 1; j < pow2 - 1; ++j)
            {
                X += h;
                y[i] += h * fxy(a + h * ((i - 1) * (pow2 - 1) + j - 1), y[i]);
            }
        }
    } while(std::fabs(y[n - 1] - y1[n - 1]) > eps);

    //Адамс
    pow2 = 1;
    h = 0.1;

    do
	{
	    for(int i = 0; i < 5; ++i)
	    {
	        yE = yExt(x,y,n+i-1,h);
	        y[n+i] = y[n-1+i] + yE;
	        yI = yInt(x,y,n+i-1,h);
	        
	        y[n+i] = y[n-1+i] + yI;
	        yI = yInt(x,y,n+i-1,h);
	        y[n+i] = y[n-1+i] + yI;
	            
	        for(int j = 1; j < pow2-1; ++j)
	        {
	            yE = yExt(x,y,n+i,h);
	            y[n+i+1] = y[n+i] + yE;
	            yI = yInt(x,y,n+i,h);
	            
	            y[n+i+1] = y[n+i] + yI;
	            yI = yInt(x,y,n+i,h);
	            y[n+i] = y[n+i] + yI;
	        }
	    }
    
    h/=2;
    pow2 *=2;
	} while(fabs(yE - yI) >= eps);

	// Вывод результатов
	cout << "Runge-Kutta:"<< endl;
	
	for(int i = 0; i < n; ++i)
	{
	    cout << x[i] << "	" << y[i] << endl;
	}
	
	cout << "Adams:"<< endl;
	
	for(int i = n; i < n+5; ++i)
	{
	    cout << x[i] << "	" << y[i] << endl;
	}
	
	// Освобождаем динамически выделенную память
	delete [] x;
	delete [] y;
	delete [] y1;
	
	// Ожидание нажатия клавиши перед выходом
	getch();
	
	return 0;
	}
