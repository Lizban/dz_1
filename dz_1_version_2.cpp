#include <cstdio> 
#include <iostream>
#include <cmath>

using namespace std;

class Complex{
public:
    virtual double Re() const = 0;
    virtual double Im() const = 0;
    virtual void set(double Repart, double Impart) = 0;
    virtual void print() const = 0;
    virtual ~Complex() = default;
};

class AlComplex: public Complex{
private:
    double a;
    double b;
public:
    AlComplex(double a, double b)
        : a(a), b(b){
        }
    double Re() const override{
        return a;
    }
    double Im() const override{
        return b;
    }
    void set(double Repart, double Impart) override{
        a = Repart;
        b = Impart;
    }
    void print() const override{
        if (b > 0){
            cout << a << "+" << b << "i";
        }
        else if (b < 0){
            cout << a << "-" << -b << "i";
        }
        else {
            cout << a;
        }
    }
    AlComplex operator+(const AlComplex& other) const {
        return AlComplex(a + other.a, b + other.b);
    }
    AlComplex operator-(const AlComplex& other) const {
        return AlComplex(a - other.a, b - other.b);
    }
    AlComplex operator*(const AlComplex& other) const {
        return AlComplex(a * other.a - b * other.b, a * other.b + b * other.a);
    }
    AlComplex operator/(const AlComplex& other) const {
        double zm = other.a * other.a + other.b * other.b;
        return AlComplex(
            (a * other.a + b * other.b) / zm, (b * other.a - a * other.b) / zm
        );
    }
    AlComplex root() const {
        double r = sqrt(a * a + b * b);
        double x = sqrt((r + a) / 2);
        double y = sqrt((r - a) / 2);
        if (b < 0){
            y = -y;
        }
        return AlComplex(x, y);
    }
};

void uravnenie(double a, double b, double c, Complex& x1, Complex& x2){
        double D = b * b - a * c * 4;
        if (D >= 0) {
            double rootD = sqrt(D);
            double root1 = (-b + rootD) / (2 * a);
            double root2 = (-b - rootD) / (2 * a);
            x1.set(root1, 0);
            x2.set(root2, 0);
        }
        else {
            double Repart = -b / (2 * a);
            double Impart = sqrt(-D) / (2 * a);
            x1.set(Repart, Impart);
            x2.set(Repart, -Impart);
        }
    }

int main(){
    AlComplex n1(3, 4);
    AlComplex n2(1, 2);
    double a, b, c;
    cout << "n1 = ";
    n1.print();
    cout << endl;
    cout << "n2 = ";
    n2.print();
    cout << endl;
    AlComplex sm = n1 + n2;
    cout << "n1 + n2 = ";
    sm.print();
    cout << endl;
    AlComplex sb = n1 - n2;
    cout << "n1 - n2 = ";
    sb.print();
    cout << endl;
    AlComplex mp = n1 * n2;
    cout << "n1 * n2 = ";
    mp.print();
    cout << endl;
    AlComplex dv = n1 / n2;
    cout << "n1 / n2 = ";
    dv.print();
    cout << endl;
    cout << "Enter a, b, c: ";
    cin >> a >> b >> c;
    AlComplex x1(0, 0);
    AlComplex x2(0, 0);
    uravnenie(a, b, c, x1, x2);
    cout << "x1 = ";
    x1.print();
    cout << endl;
    cout << "x2 = ";
    x2.print();
    cout << endl;
    return 0;
}
