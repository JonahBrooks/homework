// CS491 Project 1
// Jonah Brooks
// 09/30/2015
// Derived from skeleton code provided.

#include "vec3.h"

Vec3::Vec3( float _x, float _y, float _z )
{
  x = _x;
  y = _y;
  z = _z;
}

Vec3&
Vec3::operator=( const Vec3& rhs )
{
  this->x = rhs.x;
  this->y = rhs.y;
  this->z = rhs.z;
  return *this;
}

Vec3
Vec3::operator+( const Vec3& that )
{
  // "this" is a pointer to this class's vec3  (this->name)
  // "that" is a reference to the other vector (that.name)
  Vec3 result;
  result.x = this->x + that.x;
  result.y = this->y + that.y;
  result.z = this->z + that.z;
  return result;
}

Vec3
Vec3::operator-( const Vec3& that )
{
  // "this" is a pointer to this class's vec3  (this->name)
  // "that" is a reference to the other vector (that.name)
  Vec3 result;
  result.x = this->x - that.x;
  result.y = this->y - that.y;
  result.z = this->z - that.z;
  return result;
}

Vec3
Vec3::operator-( )
{
  // "this" is a pointer to this class's vec3  (this->name)
  // there is no "that" here
  Vec3 result;
  result.x = -(this->x);
  result.y = -(this->y);
  result.z = -(this->z);
  return result;
}

Vec3
Vec3::Cross( Vec3& that )
{
  // "this" is a pointer to this class's vec3  (this->name)
  // "that" is a reference to the other vector (that.name)
  Vec3 result;
  result.x = this->y * that.z - this->z * that.y;
  result.y = this->z * that.x - this->x * that.z;
  result.z = this->x * that.y - this->y * that.x;
  return result;
}

float
Vec3::Dot( Vec3& that )
{
  // "this" is a pointer to this class's vec3  (this->name)
  // "that" is a reference to the other vector (that.name)
  float d = this->x * that.x + this->y * that.y + this->z * that.z;
  return d;
}

float
Vec3::Length( )
{
  // "this" is a pointer to this class's vec3  (this->name)
  float len = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
  // Alternatively, float len = sqrt(SQR(this->x) + SQR(this->y) + SQR(this->z));
  return len;
}

void
Vec3::Print( char *str, FILE *fp )
{
  fprintf( fp, "%s [ %8.3f %8.3f %8.3f ]\n", str, this->x, this->y, this->z );
}

Vec3
Vec3::Unit( )
{
  // "this" is a pointer to this class's vec3  (this->name)
  Vec3 result;
  float mag;
  mag = this->Length();
  result.x = x / mag;
  result.y = y / mag;
  result.z = z / mag;
  return result;
}

