#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "ball.hh"
#include <vector>

class System
{
public:
  System();

  void step(double dt);
  void simulate(double dt);
  bool isCollision(Ball* a, Ball* b);
  void collision(Ball* a, Ball* b);

  double m;
  double I;
  double R;

  std::vector<Ball> balls;
};

#endif // SYSTEM_HH
