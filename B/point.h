#include <bits/stdc++.h>
using namespace std;
class point
{
  private :
    double x;
    double y;
  public :
    point ( double xx, double yy ) { x=xx; y=yy; }
    double X ( void ) {return x;}
    double Y ( void ) {return y;}

    void lineto ( point *p )
    //    point::lineto writes information about a line to the PostScript file.
    { 
    cout << this->x << " " 
        << this->y << " " 
        << p->x   << " "
        << p->y   << "\n";
    }

    friend point operator + (point a, point b)
    { 
      return point ( a.x+b.x, a.y+b.y );
    }

    friend point operator - (point a, point b)
    { 
      return point ( a.x-b.x, a.y-b.y );
    }
    friend double operator * (point a, point b)
    { 
      return a.x*b.x + a.y*b.y;
    }
    friend double operator ^ (point a, point b)
    { 
      return a.x*b.y - a.y*b.x;
    }
};