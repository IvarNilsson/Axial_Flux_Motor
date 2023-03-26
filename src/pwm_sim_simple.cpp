#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

#define pi 3.14159265358979323846

double sample_sin(double t, double amplitude, double freq, double phase) {
    return amplitude * sin(2 * pi * freq * t + phase);
};

double sample_tri(double t, double amplitude, double freq) {
    double res = 0.0;
    double fullPeriodTime = 1.0 / freq;
    double localTime = fmod(t, fullPeriodTime);

    double value = localTime / fullPeriodTime;

    if (value < 0.25) {
        res = value * 4;
    } else if (value < 0.75) {
        res = 2.0 - (value * 4.0);
    } else {
        res = value * 4 - 4.0;
    }

    return amplitude * res;
};

int main() {
    printf("%s \n", "Hellow");

    double amplitude_sin = 3.3;
    double amplitude_tri = 3.3;

    double phase1 = 0;
    double phase2 = -2 * pi / 3;
    double phase3 = -4 * pi / 3;
    // double freq_sin = 10;
    // double freq_tri = 200;

    double freq_sin = 2;
    double freq_tri = 10;

    double sine1;
    double sine2;
    double sine3;
    double tri;

    FILE *s1 = fopen("data/sin1.txt", "w");
    FILE *s2 = fopen("data/sin2.txt", "w");
    FILE *s3 = fopen("data/sin3.txt", "w");

    FILE *t1 = fopen("data/tri1.txt", "w");

    FILE *pwm1 = fopen("data/pwm1.txt", "w");
    FILE *pwm2 = fopen("data/pwm2.txt", "w");
    FILE *pwm3 = fopen("data/pwm3.txt", "w");

    for (double i = 0; i < 1; i = i + 0.000001) {
        sine1 = sample_sin(i, amplitude_sin, freq_sin, phase1);
        fprintf(s1, "%lf \n", sine1);
        sine2 = sample_sin(i, amplitude_sin, freq_sin, phase2);
        fprintf(s2, "%lf \n", sine2);
        sine3 = sample_sin(i, amplitude_sin, freq_sin, phase3);
        fprintf(s3, "%lf \n", sine3);

        tri = sample_tri(i, amplitude_tri, freq_tri);
        fprintf(t1, "%lf \n", tri);

        if (sine1 > tri) {
            fprintf(pwm1, "%lf \n", (double)1);
        } else {
            fprintf(pwm1, "%lf \n", (double)0);
        }

        if (sine2 > tri) {
            fprintf(pwm2, "%lf \n", (double)1);
        } else {
            fprintf(pwm2, "%lf \n", (double)0);
        }

        if (sine3 > tri) {
            fprintf(pwm3, "%lf \n", (double)1);
        } else {
            fprintf(pwm3, "%lf \n", (double)0);
        }
    }

    return 0;
}
