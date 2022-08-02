#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include "univers.h"

class Personnage : public Univers
{

public:
    explicit Personnage(QObject *parent) : Univers(parent) {}

    int level = 1;
    int life = 100;
    bool failed = false;

};

#endif // PERSONNAGE_H
