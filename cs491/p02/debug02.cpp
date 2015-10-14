#include "mat4.h"

int
main( int argc, char *argv[ ] )
{
  Mat4 I;
  I.Print( "I = " );

  Mat4 M(  1.,  2.,  3.,  4.,   5.,  6.,  7.,  8.,   9., 10., 11., 12.,  13., 14., 15., 16. );
  M.Print( "M = " );

  Mat4 M2;
  M2 = M;
  M2.Print( "M2 = " );

  M2.SetRotateX(30);
  M2.Print( "RotateX(30) = ");
  M2.SetRotateY(30);
  M2.Print( "RotateY(30) = ");
  M2.SetRotateZ(30);
  M2.Print( "RotateZ(30) = ");
  M2.SetTranslate(10,20,30);
  M2.Print( "Translate(10,20,30) = ");
  M2.SetScale(10,20,30);
  M2.Print( "Scale(10,20,30) = ");

  Mat4 P = M * M;
  P.Print( "P = " );

        Vec3 V( 1., 2., 3. );
        V.Print( "V =" );
        Vec3 V1 = P * V;
        V1.Print( "V1 =" );

  return 0;
}
