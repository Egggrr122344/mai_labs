#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

double heronFormula(int a, int b, int c) {
    double s = (a + b + c) / 2.0;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

int main() {
    int N;
    cin >> N;

    vector<int> segments(N);
    for (int i = 0; i < N; i++) {
        cin >> segments[i];
    }

    sort(segments.begin(), segments.end());

    double maxArea = 0.0;
    vector<int> bestTriangle(3);

    // Ищем тройки отрезков, которые могут образовать треугольник
    for (int i = N - 1; i >= 2; i--) {
        int a = segments[i - 2];
        int b = segments[i - 1];
        int c = segments[i];

        // Проверка треугольного неравенства
        if (a + b > c) {
            double area = heronFormula(a, b, c);
            if (area > maxArea) {
                maxArea = area;
                bestTriangle = {a, b, c};
            }
        }
    }

    if (maxArea > 0) {
        cout << fixed << setprecision(3) << maxArea << endl;
        for (int side : bestTriangle) {
            cout << side << " ";
        }
        cout << endl;
    } else {
        cout << 0 << endl;
    }

    return 0;
}
