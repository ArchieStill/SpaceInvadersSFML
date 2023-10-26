#ifndef VECTOR2_H
#define VECTOR2_H


struct Vector2
{
  // construction
  Vector2(float x_, float y_);
  Vector2(const Vector2& rhs);

  // operations
  Vector2 operator*(float scalar);
  Vector2& operator=(const Vector2& rhs);
  void normalise();

  // data
  float x = 0;
  float y = 0;

  void aliensVector();
  void aliensGravityVector();
  void invertVectorX();
  void lasersVector();
  void backgroundVector();
};

#endif // VECTOR2_H