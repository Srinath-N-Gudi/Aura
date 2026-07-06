#pragma once
#include <cmath>
#include <cstdint>
#include <algorithm>

static const int SN_g2d[12][2] = {
    {1,1},{-1,1},{1,-1},{-1,-1},
    {1,0},{-1,0},{0,1},{0,-1},
    {1,1},{-1,1},{1,-1},{-1,-1}
};

static const int SN_g3d[12][3] = {
    {1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
    {1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
    {0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1}
};

class SimplexNoise {
public:
    SimplexNoise(int seed = 0) {
        perm = new uint8_t[512];
        uint8_t p[256];
        for (int i = 0; i < 256; i++)
            p[i] = (uint8_t)i;
        uint32_t s = (uint32_t)seed;
        for (int i = 255; i > 0; i--) {
            s = s * 1664525 + 1013904223;
            int j = (s >> 16) % (i + 1);
            std::swap(p[i], p[j]);
        }
        for (int i = 0; i < 512; i++)
            perm[i] = p[i & 255];
    }
    ~SimplexNoise() { delete[] perm; }

    double noise2D(double xin, double yin) const {
        const double F2 = 0.5 * (std::sqrt(3.0) - 1.0);
        const double G2 = (3.0 - std::sqrt(3.0)) / 6.0;
        double s = (xin + yin) * F2;
        int i = fastFloor(xin + s);
        int j = fastFloor(yin + s);
        double t = (i + j) * G2;
        double x0 = xin - (i - t);
        double y0 = yin - (j - t);
        int i1, j1;
        if (x0 > y0) { i1 = 1; j1 = 0; } else { i1 = 0; j1 = 1; }
        double x1 = x0 - i1 + G2, y1 = y0 - j1 + G2;
        double x2 = x0 - 1.0 + 2.0 * G2, y2 = y0 - 1.0 + 2.0 * G2;
        int ii = i & 255, jj = j & 255;
        int gi0 = perm[ii + perm[jj]] % 12;
        int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
        int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;
        double n0 = 0, n1 = 0, n2 = 0;
        double t0 = 0.5 - x0*x0 - y0*y0;
        if (t0 >= 0) { t0 *= t0; n0 = t0*t0 * dot2(SN_g2d[gi0], x0, y0); }
        double t1 = 0.5 - x1*x1 - y1*y1;
        if (t1 >= 0) { t1 *= t1; n1 = t1*t1 * dot2(SN_g2d[gi1], x1, y1); }
        double t2 = 0.5 - x2*x2 - y2*y2;
        if (t2 >= 0) { t2 *= t2; n2 = t2*t2 * dot2(SN_g2d[gi2], x2, y2); }
        return 70.0 * (n0 + n1 + n2);
    }

    double noise3D(double xin, double yin, double zin) const {
        const double F3 = 1.0 / 3.0;
        const double G3 = 1.0 / 6.0;
        double s = (xin + yin + zin) * F3;
        int i = fastFloor(xin + s), j = fastFloor(yin + s), k = fastFloor(zin + s);
        double t = (i + j + k) * G3;
        double x0 = xin - (i - t), y0 = yin - (j - t), z0 = zin - (k - t);
        int i1,j1,k1,i2,j2,k2;
        if (x0>=y0) {
            if (y0>=z0)      {i1=1;j1=0;k1=0;i2=1;j2=1;k2=0;}
            else if (x0>=z0) {i1=1;j1=0;k1=0;i2=1;j2=0;k2=1;}
            else             {i1=0;j1=0;k1=1;i2=1;j2=0;k2=1;}
        } else {
            if (y0<z0)       {i1=0;j1=0;k1=1;i2=0;j2=1;k2=1;}
            else if (x0<z0)  {i1=0;j1=1;k1=0;i2=0;j2=1;k2=1;}
            else             {i1=0;j1=1;k1=0;i2=1;j2=1;k2=0;}
        }
        double x1=x0-i1+G3, y1=y0-j1+G3, z1=z0-k1+G3;
        double x2=x0-i2+2*G3, y2=y0-j2+2*G3, z2=z0-k2+2*G3;
        double x3=x0-1+3*G3, y3=y0-1+3*G3, z3=z0-1+3*G3;
        int ii=i&255, jj=j&255, kk=k&255;
        int gi0=perm[ii+perm[jj+perm[kk]]]%12;
        int gi1=perm[ii+i1+perm[jj+j1+perm[kk+k1]]]%12;
        int gi2=perm[ii+i2+perm[jj+j2+perm[kk+k2]]]%12;
        int gi3=perm[ii+1+perm[jj+1+perm[kk+1]]]%12;
        double n0=0,n1=0,n2=0,n3=0;
        double t0=0.6-x0*x0-y0*y0-z0*z0;
        if(t0>=0){t0*=t0;n0=t0*t0*dot3(SN_g3d[gi0],x0,y0,z0);}
        double t1=0.6-x1*x1-y1*y1-z1*z1;
        if(t1>=0){t1*=t1;n1=t1*t1*dot3(SN_g3d[gi1],x1,y1,z1);}
        double t2=0.6-x2*x2-y2*y2-z2*z2;
        if(t2>=0){t2*=t2;n2=t2*t2*dot3(SN_g3d[gi2],x2,y2,z2);}
        double t3=0.6-x3*x3-y3*y3-z3*z3;
        if(t3>=0){t3*=t3;n3=t3*t3*dot3(SN_g3d[gi3],x3,y3,z3);}
        return 32.0*(n0+n1+n2+n3);
    }

    double octave2D(double x, double y, int octaves, double persistence = 0.5, double lacunarity = 2.0) const {
        double total = 0, frequency = 1, amplitude = 1, maxValue = 0;
        for (int i = 0; i < octaves; i++) {
            total += noise2D(x * frequency, y * frequency) * amplitude;
            maxValue += amplitude;
            amplitude *= persistence;
            frequency *= lacunarity;
        }
        return total / maxValue;
    }

private:
    uint8_t* perm;
    static int fastFloor(double x) { int xi = (int)x; return x < xi ? xi - 1 : xi; }
    static double dot2(const int g[], double x, double y) { return g[0]*x + g[1]*y; }
    static double dot3(const int g[], double x, double y, double z) { return g[0]*x + g[1]*y + g[2]*z; }
};
