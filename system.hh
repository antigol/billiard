#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "ball.hh"
#include <vector>

class System
{
public:
  System();

  void simulate(double dt);
  void collision(Ball* a, Ball* b);

  double m;
  double I;
  double R;

  std::vector<Ball> balls;
};

#endif // SYSTEM_HH
