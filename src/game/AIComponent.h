#pragma once

class Tank;

class AIComponent
{
public:
    AIComponent(Tank* pParentTank);

    void Update(const double delta);

private:
    Tank* pParentTank_;
};