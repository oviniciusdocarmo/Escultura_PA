#include <iostream>
#include "sculptor.h"
#include <cmath>
#include <fstream>
#include <vector>
#include <cstdlib>

//Construtores e destrutores
Sculptor::Sculptor(){
    nx = 0;
    ny = 0;
    nz = 0;
}
Sculptor::Sculptor(int _nx, int _ny, int _nz){

    nx = _nx;
    ny = _ny;
    nz = _nz;
    r = 0.0;
    g = 0.0;
    b = 0.0;
    a = 0.0;
    v = new Voxel **[_nx];
    for (int i = 0; i < _nx; i++){
        v[i] = new Voxel *[_ny];
        for (int j = 0; j < _ny; j++){
            v[i][j] = new Voxel[_nz];
            for (int k = 0; k < _nz; k++){
                v[i][j][k].r = 0.0;
                v[i][j][k].g = 0.0;
                v[i][j][k].b = 0.0;
                v[i][j][k].a = 0.0;
                v[i][j][k].isOn = false;
            }
        }
    }
}
Sculptor::~Sculptor(){
    for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
            delete[] v[i][j];
        }
        delete[] v[i];
    }
    delete[] v;
}
void Sculptor::setColor(float _r, float _g, float _b, float alpha){
    r = _r;
    g = _g;
    b = _b;
    a = alpha;
}
//Ativa o voxel na posição (x,y,z) (fazendo isOn = true) e atribui ao mesmo a cor atual de desenho
void Sculptor::putVoxel(int x, int y, int z){

    v[x][y][z].r = r;
    v[x][y][z].g = g;
    v[x][y][z].b = b;
    v[x][y][z].a = a;
    v[x][y][z].isOn = true;

}
//Desativa o voxel na posição (x,y,z) (fazendo isOn = false)
void Sculptor::cutVoxel(int x, int y, int z){
    v[x][y][z].isOn = false;
}

//Ativa todos os voxels no intervalo x∈[x0,x1], y∈[y0,y1], z∈[z0,z1] e atribui aos mesmos a cor atual de desenho
void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1){
    //Evitando sair do domínio
    x0 = (x0 < 0) ? 0 : x0;
    x1 = (x1 > nx) ? nx : x1;
    y0 = (y0 < 0) ? 0 : y0;
    y1 = (y1 > ny) ? ny : y1;
    z0 = (z0 < 0) ? 0 : z0;
    z1 = (z1 > nz) ? nz : z1;

    //Voxel vx(r,g,b,a,true);
    for(int i=x0; i<x1; i++){
        for(int j=y0; j<y1; j++){
            for(int k=z0; k<z1; k++){
                putVoxel(i,j,k);
            }
        }
    }
}
//Desativa todos os voxels no intervalo x∈[x0,x1], y∈[y0,y1], z∈[z0,z1] e atribui aos mesmos a cor atual de desenho
void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1){
    //Evitando sair do domínio
    x0 = (x0 < 0) ? 0 : x0;
    x1 = (x1 > nx) ? nx : x1;
    y0 = (y0 < 0) ? 0 : y0;
    y1 = (y1 > ny) ? ny : y1;
    z0 = (z0 < 0) ? 0 : z0;
    z1 = (z1 > nz) ? nz : z1;

    //Voxel vx(r,g,b,a,false);
    for(int i=x0; i<x1; i++){
        for(int j=y0; j<y1; j++){
            for(int k=z0; k<z1; k++){
                cutVoxel(i,j,k);
            }
        }
    }
}
//voxels que satisfazem à equação
bool Sculptor::SatisfEquacaoEllipsoid(int i, int j, int k, int xcenter,int ycenter,int zcenter,int rx,int ry,int rz){
    return ((pow(i - xcenter, 2) / pow(rx, 2) + pow(j - ycenter, 2) / pow(ry, 2) + pow(k - zcenter, 2) / pow(rz, 2)) < 1);
};

//Ativa todos os voxels que satisfazem à equação da esfera e atribui aos mesmos a cor atual de desenho (r,g,b,a)
void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius){
    putEllipsoid(xcenter, ycenter, zcenter, radius, radius, radius);
}
//Desativa todos os voxels que satisfazem à equação da esfera
void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius){
    cutEllipsoid(xcenter, ycenter, zcenter, radius, radius, radius);
}
void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    //Evitando sair do domínio
    int x0, x1, y0, y1, z0, z1;
    x0 = (xcenter - rx < 0) ? 0 : xcenter - rx;
    x1 = (xcenter + rx > nx) ? nx : xcenter + rx;
    y0 = (ycenter - ry < 0) ? 0 : ycenter - ry;
    y1 = (ycenter + ry > ny) ? ny : ycenter + ry;
    z0 = (zcenter - rz < 0) ? 0 : zcenter - rz;
    z1 = (zcenter + rz > nz) ? nz : zcenter + rz;


    for (int i = x0; i < x1; i++){
        for (int j = y0; j < y1; j++){
            for (int k = z0; k < z1; k++){
                if (SatisfEquacaoEllipsoid(i,j,k,xcenter,ycenter,zcenter,rx,rx,rz)){
                    putVoxel(i, j, k);
                }
            }
        }
    }
}
void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){
    //Evitando sair do domínio
    int x0, x1, y0, y1, z0, z1;
    x0 = (xcenter - rx < 0) ? 0 : xcenter - rx;
    x1 = (xcenter + rx > nx) ? nx : xcenter + rx;
    y0 = (ycenter - ry < 0) ? 0 : ycenter - ry;
    y1 = (ycenter + ry > ny) ? ny : ycenter + ry;
    z0 = (zcenter - rz < 0) ? 0 : zcenter - rz;
    z1 = (zcenter + rz > nz) ? nz : zcenter + rz;


    for (int i = x0; i < x1; i++){
        for (int j = y0; j < y1; j++){
            for (int k = z0; k < z1; k++){
                if (SatisfEquacaoEllipsoid(i,j,k,xcenter,ycenter,zcenter,rx,rx,rz)){
                    cutVoxel(i, j, k);
                }
            }
        }
    }
}

//grava a escultura no formato OFF no arquivo filename
void Sculptor::writeOFF(const char* filename){

    std::ofstream myFile(filename);

    if (!myFile.is_open()){
        std::cout << "O arquivo não foi aberto" << std::endl;
        exit(1);
    }

    int vAtivo = 0, i, j, k;
    for (i = 0; i < nx; i++){
        for (j = 0; j < ny; j++){
            for (k = 0; k < nz; k++){
                if (v[i][j][k].isOn){
                    vAtivo++;
                }
            }
        }
    }


    myFile << "OFF" << std::endl;
    myFile << 8 * vAtivo << " " << 6 * vAtivo << " 0" << std::endl;

    for (i = 0; i < nx; i++){
        for (j = 0; j < ny; j++){
            for (k = 0; k < nz; k++){
                if (v[i][j][k].isOn){
                    // mais stream
                    myFile << i - 0.5 << " " << j + 0.5 << " " << k - 0.5 << std::endl;
                    myFile << i - 0.5 << " " << j - 0.5 << " " << k - 0.5 << std::endl;
                    myFile << i + 0.5 << " " << j - 0.5 << " " << k - 0.5 << std::endl;
                    myFile << i + 0.5 << " " << j + 0.5 << " " << k - 0.5 << std::endl;
                    myFile << i - 0.5 << " " << j + 0.5 << " " << k + 0.5 << std::endl;
                    myFile << i - 0.5 << " " << j - 0.5 << " " << k + 0.5 << std::endl;
                    myFile << i + 0.5 << " " << j - 0.5 << " " << k + 0.5 << std::endl;
                    myFile << i + 0.5 << " " << j + 0.5 << " " << k + 0.5 << std::endl;
                }
            }
        }
    }
    int contVAtivo = 0;
    for (i = 0; i < nx; i++){
        for (j = 0; j < ny; j++){
            for (k = 0; k < nz; k++){
                if (v[i][j][k].isOn){
                    myFile << "4 " << contVAtivo * 8 + 0 << " " << contVAtivo * 8 + 3 << " " << contVAtivo * 8 + 2 << " " << contVAtivo * 8 + 1 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    myFile << "4 " << contVAtivo * 8 + 4 << " " << contVAtivo * 8 + 5 << " " << contVAtivo * 8 + 6 << " " << contVAtivo * 8 + 7 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    myFile << "4 " << contVAtivo * 8 + 0 << " " << contVAtivo * 8 + 1 << " " << contVAtivo * 8 + 5 << " " << contVAtivo * 8 + 4 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    myFile << "4 " << contVAtivo * 8 + 0 << " " << contVAtivo * 8 + 4 << " " << contVAtivo * 8 + 7 << " " << contVAtivo * 8 + 3 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    myFile << "4 " << contVAtivo * 8 + 3 << " " << contVAtivo * 8 + 7 << " " << contVAtivo * 8 + 6 << " " << contVAtivo * 8 + 2 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    myFile << "4 " << contVAtivo * 8 + 1 << " " << contVAtivo * 8 + 2 << " " << contVAtivo * 8 + 6 << " " << contVAtivo * 8 + 5 << " " << v[i][j][k].r << " " << v[i][j][k].g << " " << v[i][j][k].b << " " << v[i][j][k].a << std::endl;
                    contVAtivo++;
                }
            }
        }
    }

    myFile.close();
}
