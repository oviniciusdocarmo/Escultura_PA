#include <iostream>
#include "sculptor.h"
#include "voxel.h"

int main(void){


    Sculptor teste(50,50,50);
    teste.setColor(3, 4, 5, 1);
    teste.putVoxel(3,3,1);
    teste.putVoxel(3,3,2);
    teste.putVoxel(3,3,3);
    teste.cutVoxel(3,3,2);
    teste.putBox(0,10,0,10,0,10);
    teste.cutBox(4,6,4,6,4,6);
    teste.putSphere(25,25,25,5);
    teste.cutSphere(25,25,25,4);
    teste.putEllipsoid(40,40,40,3,4,5);
    teste.cutEllipsoid(40,40,40,5,4,3);
    teste.writeOFF("teste.off");

    /*
    Sculptor coxinha(30,30,30);
    coxinha.setColor(2,5,10,1);
    coxinha.putSphere(5,5,5,5);

    coxinha.putSphere(7,7,7,7);
    coxinha.writeOFF("coxinha.off");
    */

    return 0;

}
