#pragma once
#include "Weapon.h"

class Buster : public Weapon
{
public:
    Buster(WeaponComponent* owner);
    virtual ~Buster();

    virtual void Fire() override;
};
