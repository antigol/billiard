#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "ball.hh"
#include "wall.hh"
#include <vector>

class System
{
public:
  System();

  void step(double dt);
  void simulate(double dt);
  bool is_collision_ball(Ball* a, Ball* b);
  bool is_collision_wall(Ball* a, Wall* w);
  void collision_ball(Ball* a, Ball* b);
  void collision_wall(Ball* a, Wall* w);

  double m;
  double I;
  double R;

  std::vector<Ball> balls;
  std::vector<Wall> walls;
};

#endif // SYSTEM_HH
