#include <cstdlib>
#include <iostream>
#include "sculptor.h"
#include <vector>
#include <fstream>
#include <cmath>

//Check if voxel is inside ellipsoid
bool isInsideEllipsoid(int i, int j, int k, int xcenter,int ycenter,int zcenter,int rx,int ry,int rz)
{
    return ((pow(i - xcenter, 2) / pow(rx, 2) + pow(j - ycenter, 2) / pow(ry, 2) + pow(k - zcenter, 2) / pow(rz, 2)) < 1);
};

/// @brief Class constructor.
///
/// Create and allocate space to the voxel matrix and set the color parameters to zero.
/// @param _nx,_ny,_nz Size of the voxel matrix.
Sculptor::Sculptor(int _nx, int _ny, int _nz)
{

    this->nx = _nx;
    this->ny = _ny;
    this->nz = _nz;
    this->r = 0.0;
    this->g = 0.0;
    this->b = 0.0;
    this->a = 0.0;
    v = new Voxel **[_nx];
    for (int i = 0; i < _nx; i++)
    {
        v[i] = new Voxel *[_ny];
        for (int j = 0; j < _ny; j++)
        {
            v[i][j] = new Voxel[_nz];
            for (int k = 0; k < _nz; k++)
            {
                this->v[i][j][k].r = 0.0;
                this->v[i][j][k].g = 0.0;
                this->v[i][j][k].b = 0.0;
                this->v[i][j][k].a = 0.0;
                this->v[i][j][k].isOn = false;
            }
        }
    }
}

/// @brief Class destructor.
///
/// deletes the class
Sculptor::~Sculptor()
{
    for (int i = 0; i < nx; i++)
    {

        for (int j = 0; j < ny; j++)
        {
            delete[] v[i][j];
        }
        delete[] v[i];
    }
    delete[] v;
}

///@brief set the current color
void Sculptor::setColor(float r, float g, float b, float alpha)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = alpha;
}

///@brief set the Voxel in the position (x,y,z) as on
void Sculptor::putVoxel(int x, int y, int z)
{
    this->v[x][y][z].isOn = true;
    this->v[x][y][z].r = this->r;
    this->v[x][y][z].g = this->g;
    this->v[x][y][z].b = this->b;
    this->v[x][y][z].a = this->a;
}

///@brief set the Voxel in the position (x,y,z) as off
void Sculptor::cutVoxel(int x, int y, int z)
{
    this->v[x][y][z].isOn = false;
}

///@brief put a box in the drawing
///
/// set the voxels on(true) inside the intervals [x0,x1], [y0,y1] and [z0,z1]
///@param x0 start of the x interval
///@param x1 end of the x interval
///@param y0 start of the y interval
///@param y1 end of the y interval
///@param z0 start of the z interval
///@param z1 end of the z interval
void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1)
{
    //This part avoids geting out of the matrix domain
    x0 = (x0 < 0) ? 0 : x0;
    x1 = (x1 > this->nx) ? this->nx : x1;
    y0 = (y0 < 0) ? 0 : y0;
    y1 = (y1 > this->ny) ? this->ny : y1;
    z0 = (z0 < 0) ? 0 : z0;
    z1 = (z1 > this->nz) ? this->nz : z1;

    //This part put the voxel in the Box
    for (int i = x0; i < x1; i++)
    {
        for (int j = y0; j < y1; j++)
        {
            for (int k = z0; k < z1; k++)
            {
                this->putVoxel(i, j, k);
            }
        }
    }
}

///@brief cut a box in the drawing
///
/// set the voxels off(false) inside the intervals [x0,x1], [y0,y1] and [z0,z1]
///@param x0 start of the x interval
///@param x1 end of the x interval
///@param y0 start of the y interval
///@param y1 end of the y interval
///@param z0 start of the z interval
///@param z1 end of the z interval
void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1)
{
    //This part avoids geting out of the matrix domain
    x0 = (x0 < 0) ? 0 : x0;
    x1 = (x1 > this->nx) ? this->nx : x1;
    y0 = (y0 < 0) ? 0 : y0;
    y1 = (y1 > this->ny) ? this->ny : y1;
    z0 = (z0 < 0) ? 0 : z0;
    z1 = (z1 > this->nz) ? this->nz : z1;

    //This part put the voxel in the Box
    for (int i = x0; i < x1; i++)
    {
        for (int j = y0; j < y1; j++)
        {
            for (int k = z0; k < z1; k++)
            {
                this->cutVoxel(i, j, k);
            }
        }
    }
}

///@brief Turns on all the voxel inside the sphere according to parameters
///@param xcenter,ycenter,zcenter sphere center
///@param radius sphere radius
///@see =putEllipsoid()
void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius)
{
    putEllipsoid(xcenter, ycenter, zcenter, radius, radius, radius);
}

///@brief Turns off all the voxel inside the sphere according to parameters
///@param xcenter,ycenter,zcenter sphere center
///@param radius sphere radius
///@see cutEllipsoid()
void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius)
{

    cutEllipsoid(xcenter, ycenter, zcenter, radius, radius, radius);
}

///@brief Turns on all the voxel inside the Ellipsoid according to parameters
///@param xcenter,ycenter,zcenter ellipsoid center
///@param rx,ry,rz ellipsoid axis
void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz)
{
    //This part avoids geting out of the matrix domain
    int x0, x1, y0, y1, z0, z1;
    x0 = (xcenter - rx < 0) ? 0 : xcenter - rx;
    x1 = (xcenter + rx > this->nx) ? this->nx : xcenter + rx;
    y0 = (ycenter - ry < 0) ? 0 : ycenter - ry;
    y1 = (ycenter + ry > this->ny) ? this->ny : ycenter + ry;
    z0 = (zcenter - rz < 0) ? 0 : zcenter - rz;
    z1 = (zcenter + rz > this->nz) ? this->nz : zcenter + rz;

    //This part iterates through a box to them check if it satisfies the ellipsoid equation
    for (int i = x0; i < x1; i++)
    {
        for (int j = y0; j < y1; j++)
        {
            for (int k = z0; k < z1; k++)
            {
                //Conditional statement that executes if the Voxel is inside the ellipsoid
                if (isInsideEllipsoid(i,j,k,xcenter,ycenter,zcenter,rx,rx,rz))
                {
                    this->putVoxel(i, j, k);
                }
            }
        }
    }
}

///@brief Turns off all the voxel inside the Ellipsoid according to parameters
///@param xcenter,ycenter,zcenter ellipsoid center
///@param rx,ry,rz ellipsoid axis
void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz)
{

    //This part avoids geting out of the matrix domain
    int x0, x1, y0, y1, z0, z1;
    x0 = (xcenter - rx < 0) ? 0 : xcenter - rx;
    x1 = (xcenter + rx > this->nx) ? this->nx : xcenter + rx;
    y0 = (ycenter - ry < 0) ? 0 : ycenter - ry;
    y1 = (ycenter + ry > this->ny) ? this->ny : ycenter + ry;
    z0 = (zcenter - rz < 0) ? 0 : zcenter - rz;
    z1 = (zcenter + rz > this->nz) ? this->nz : zcenter + rz;

    //This part iterates through a box to them check if it satisfies the ellipsoid equation
    for (int i = x0; i < x1; i++)
    {
        for (int j = y0; j < y1; j++)
        {
            for (int k = z0; k < z1; k++)
            {
                //Conditional statement that executes if the Voxel is inside the ellipsoid
                if (isInsideEllipsoid(i,j,k,xcenter,ycenter,zcenter,rx,rx,rz))
                {
                    this->cutVoxel(i, j, k);
                }
            }
        }
    }
}

///@brief save the sculpture created in the format .OFF
///
///@param filename name of the the fuction will save on
void Sculptor::writeOFF(char *filename)
{


    std::ofstream myFile(filename);

    //Stream starts
    if (!myFile.is_open())
    {
        std::cout << "file not open" << std::endl;
        exit(1);
    }
    //Count How many Voxels are on
    int vOn = 0, i, j, k;
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            for (k = 0; k < nz; k++)
            {
                if (this->v[i][j][k].isOn)
                {
                    vOn++;
                }
            }
        }
    }

    ////////////////////////////////
    //(over)Write in the OFF format
    ////////////////////////////////
    myFile << "OFF" << std::endl;
    myFile << 8 * vOn << " " << 6 * vOn << " 0" << std::endl;

    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            for (k = 0; k < nz; k++)
            {
                if (this->v[i][j][k].isOn)
                {
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
    int contVOn = 0;
    for (i = 0; i < nx; i++)
    {
        for (j = 0; j < ny; j++)
        {
            for (k = 0; k < nz; k++)
            {
                if (this->v[i][j][k].isOn)
                {
                    myFile << "4 " << contVOn * 8 + 0 << " " << contVOn * 8 + 3 << " " << contVOn * 8 + 2 << " " << contVOn * 8 + 1 << " " << this->v[i][j][k].r << " " << this->v[i][j][k].g << " " << this->v[i][j][k].b << " " << this->v[i][j][k].a << std::endl;
                    myFile << "4 " << contVOn * 8 + 4 << " " << contVOn * 8 + 5 << " " << contVOn * 8 + 6 << " " << contVOn * 8 + 7 << " " << this->v[i][j][k].r << " " << this->v[i][j][k].g << " " << this->v[i][j][k].b << " " << this->v[i][j][k].a << std::endl;
                    myFile << "4 " << contVOn * 8 + 0 << " " << contVOn * 8 + 1 << " " << contVOn * 8 + 5 << " " << contVOn * 8 + 4 << " " << this->v[i][j][k].r << " " << this->v[i][j][k].g << " " << this->v[i][j][k].b << " " << this->v[i][j][k].a << std::endl;
                    myFile << "4 " << contVOn * 8 + 0 << " " << contVOn * 8 + 4 << " " << contVOn * 8 + 7 << " " << contVOn * 8 + 3 << " " << this->v[i][j][k].r << " " << this->v[i][j][k].g << " " << this->v[i][j][k].b << " " << this->v[i][j][k].a << std::endl;
                    myFile << "4 " << contVOn * 8 + 3 << " " << contVOn * 8 + 7 << " " << contVOn * 8 + 6 << " " << contVOn * 8 + 2 << " " << this->v[i][j][k].r << " " << this->v[i][j][k].g << " " << this->v[i][j][k].b << " " << this->v[i][j][k].a << std::endl;
                    myFile << "4 " << contVOn * 8 + 1 << " " << contVOn * 8 + 2 << " " << contVOn * 8 + 6 << " " << contVOn * 8 + 5 << " " << this->v[i][j][k].r << " " << this->v[i][j][k].g << " " << this->v[i][j][k].b << " " << this->v[i][j][k].a << std::endl;
                    contVOn++;
                }
            }
        }
    }
    // Close the file
    myFile.close();
}
