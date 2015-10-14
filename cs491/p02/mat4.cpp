#include "vec3.h"
#include "mat4.h"
#include <string.h>

  // Jonah Brooks
  // CS491
  // Project 2A

Mat4::Mat4( )
{
  SetIdentity( );
}


Mat4::Mat4( float a, float b, float c, float d, float e, float f, float g, float h,
    float i, float j, float k, float l, float n, float o, float p, float q )
{
  m[0][0] = a;  m[0][1] = b;  m[0][2] = c;  m[0][3] = d;
  m[1][0] = e;  m[1][1] = f;  m[1][2] = g;  m[1][3] = h;
  m[2][0] = i;  m[2][1] = j;  m[2][2] = k;  m[2][3] = l;
  m[3][0] = n;  m[3][1] = o;  m[3][2] = p;  m[3][3] = q;
}

Mat4&
Mat4::operator=( const Mat4& that )
{
  // "this" is a pointer to this class's mat4  (this->name)
  // "that" is a reference to the other matrix (that.name)

  memcpy(this->m, &that.m, sizeof(float)*16);
  return *this;
}

Mat4
Mat4::operator*( Mat4& that )
{
  // "this" is a pointer to this class's mat4  (this->name)
  // "that" is a reference to the other matrix (that.name)

  // Create convenience pointers to simplify the function

  float (*A)[4] = this->m;
  float (*B)[4] = that.m;

  // Unroll the entire nested loop for speed
  // Otherwise it would be:
  // for(int i = 0; i < 4; i++)
  //  for(int j = 0; j < 4; j++)
  //    for(int k = 0; k < 4; k++)
  //      result.m[i][j] += A[i][k] * B[k][j];
  // Or that shuffled around to maximize cache hits for row-major locality

  // Of interest, this was produced by copying Mat4::Mat4() and using vim find/replace
  // :39,43s/result.m\(\[\d\]\)\(\[\d\]\) = \w;
  //  /result.m\1\2 = A\1[0] * B[0]\2 + A\1[1] * B[1]\2 + A\1[2] * B[2]\2 + A\1[3] * B[3]\2;\r/g

  Mat4 result;
  result.m[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0] + A[0][2] * B[2][0] + A[0][3] * B[3][0];
  result.m[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1] + A[0][2] * B[2][1] + A[0][3] * B[3][1];
  result.m[0][2] = A[0][0] * B[0][2] + A[0][1] * B[1][2] + A[0][2] * B[2][2] + A[0][3] * B[3][2];
  result.m[0][3] = A[0][0] * B[0][3] + A[0][1] * B[1][3] + A[0][2] * B[2][3] + A[0][3] * B[3][3];

  result.m[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0] + A[1][2] * B[2][0] + A[1][3] * B[3][0];
  result.m[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1] + A[1][2] * B[2][1] + A[1][3] * B[3][1];
  result.m[1][2] = A[1][0] * B[0][2] + A[1][1] * B[1][2] + A[1][2] * B[2][2] + A[1][3] * B[3][2];
  result.m[1][3] = A[1][0] * B[0][3] + A[1][1] * B[1][3] + A[1][2] * B[2][3] + A[1][3] * B[3][3];

  result.m[2][0] = A[2][0] * B[0][0] + A[2][1] * B[1][0] + A[2][2] * B[2][0] + A[2][3] * B[3][0];
  result.m[2][1] = A[2][0] * B[0][1] + A[2][1] * B[1][1] + A[2][2] * B[2][1] + A[2][3] * B[3][1];
  result.m[2][2] = A[2][0] * B[0][2] + A[2][1] * B[1][2] + A[2][2] * B[2][2] + A[2][3] * B[3][2];
  result.m[2][3] = A[2][0] * B[0][3] + A[2][1] * B[1][3] + A[2][2] * B[2][3] + A[2][3] * B[3][3];

  result.m[3][0] = A[3][0] * B[0][0] + A[3][1] * B[1][0] + A[3][2] * B[2][0] + A[3][3] * B[3][0];
  result.m[3][1] = A[3][0] * B[0][1] + A[3][1] * B[1][1] + A[3][2] * B[2][1] + A[3][3] * B[3][1];
  result.m[3][2] = A[3][0] * B[0][2] + A[3][1] * B[1][2] + A[3][2] * B[2][2] + A[3][3] * B[3][2];
  result.m[3][3] = A[3][0] * B[0][3] + A[3][1] * B[1][3] + A[3][2] * B[2][3] + A[3][3] * B[3][3];

  return result;
}

Vec3
Mat4::operator*( Vec3& that )
{
  // "this" is a pointer to this class's mat4  (this->name)
  // "that" is a reference to the vector (that.name)

  Vec3 result;
  float (*A)[4] = this->m;
  Vec3 *B = &that;
  result.x = A[0][0] * B->x + A[0][1] * B->y + A[0][2] * B->z + A[0][3];
  result.y = A[1][0] * B->x + A[1][1] * B->y + A[1][2] * B->z + A[1][3];
  result.z = A[2][0] * B->x + A[2][1] * B->y + A[2][2] * B->z + A[2][3];
  return result;
}

void
Mat4::Print( char *str, FILE *fp )
{
  // "this" is a pointer to this class's mat4  (this->name)

  fprintf( fp, "%s \n", str );
  fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[0][0], this->m[0][1], this->m[0][2], this->m[0][3] );
  fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[1][0], this->m[1][1], this->m[1][2], this->m[1][3] );
  fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[2][0], this->m[2][1], this->m[2][2], this->m[2][3] );
  fprintf( fp, "\t%8.2f  %8.2f  %8.2f  %8.2f\n", this->m[3][0], this->m[3][1], this->m[3][2], this->m[3][3] );
}

void
Mat4::SetIdentity( )
{
  // "this" is a pointer to this class's mat4  (this->name)

  this->m[0][0] = 1;  this->m[0][1] = 0;  this->m[0][2] = 0;  this->m[0][3] = 0;
  this->m[1][0] = 0;  this->m[1][1] = 1;  this->m[1][2] = 0;  this->m[1][3] = 0;
  this->m[2][0] = 0;  this->m[2][1] = 0;  this->m[2][2] = 1;  this->m[2][3] = 0;
  this->m[3][0] = 0;  this->m[3][1] = 0;  this->m[3][2] = 0;  this->m[3][3] = 1;
}

  // ** 2B: **

void
Mat4::SetRotateX( float deg )
{
  // "this" is a pointer to this class's mat4  (this->name)

  //this->m ???
  this->SetIdentity();
  printf("deg: %f, D2R: %f, D2R*deg: %f, cos(): %f, sin(): %f\n",deg,D2R,D2R*deg,cos(D2R*deg),sin(D2R*deg));
  this->m[1][1] = cos(D2R*(deg));  this->m[1][2] = -sin(D2R*(deg)); 
  this->m[2][1] = sin(D2R*(deg));  this->m[2][2] = cos(D2R*(deg));  
}

void
Mat4::SetRotateY( float deg )
{
  // "this" is a pointer to this class's mat4  (this->name)

  //this->m ???
  this->SetIdentity();
  this->m[0][0] = cos(D2R*(deg));  this->m[0][2] = sin(D2R*(deg)); 
  this->m[2][0] = -sin(D2R*(deg));  this->m[2][2] = cos(D2R*(deg));  
}

void
Mat4::SetRotateZ( float deg )
{
  // "this" is a pointer to this class's mat4  (this->name)

  //this->m ???
  this->SetIdentity();
  this->m[0][0] = cos(D2R*(deg));  this->m[0][1] = -sin(D2R*(deg)); 
  this->m[1][1] = sin(D2R*(deg));  this->m[1][1] = cos(D2R*(deg));  
}



void
Mat4::SetScale( float sx, float sy, float sz )
{
  // "this" is a pointer to this class's mat4  (this->name)

  //this->m ???
  this->SetIdentity();
  this->m[0][0] = sx;  
  this->m[1][1] = sy; 
  this->m[2][2] = sz; 
}

void
Mat4::SetTranslate( float tx, float ty, float tz )
{
  // "this" is a pointer to this class's mat4  (this->name)

  //this->m ???
  this->SetIdentity();
  this->m[0][3] = tx;  
  this->m[1][3] = ty; 
  this->m[2][3] = tz; 
}
