#include <bits/stdc++.h>
#include "point.h"
using namespace std;

class Delaunay_tree;
class DT_node;
class DT_list;
class point;
typedef unsigned char ind; // used for flag and ind in array

class Delaunay_tree
{
private:
  // number of the current operation
  int nb;
  // the root of the delaunay_tree
  DT_node *root;

public:
  Delaunay_tree();
  ~Delaunay_tree();
  Delaunay_tree &operator+=(point *); // insertion
  void output(int);                   // prints Delaunay triangulation
};

class DT_flag
{
private:
  ind f;
  DT_flag() { f = (ind)0; }
  void infinite(int i) { f |= (ind)i; }
  void last_finite() { f |= 8; }
  void kill() { f |= 16; }
  ind is_infinite() { return f & 7; }
  ind is_last_finite() { return f & 8; }
  ind is_dead() { return f & 16; }
  ind is_degenerate() { return f & 32; }

public:
  friend class DT_node;
  friend class Delaunay_tree;
};

class DT_list
{
private:
  DT_list *next;
  DT_node *key;
  DT_list(DT_list *l, DT_node *k)
  {
    next = l;
    key = k;
  }
  ~DT_list();

public:
  friend class DT_node;
  friend class Delaunay_tree;
};

class DT_node
{
private:
  // the first vertex is the creator, that is finite
  // except for the root and its neighbors
  DT_flag flag;
  unsigned int nb;
  point *vertices[3];
  DT_node *neighbors[3];
  DT_list *sons;
  DT_node();               // initialize the root
  DT_node(DT_node *, ind); // initialize nowhere
  DT_node(DT_node *, point *, ind);
  // father, creator, direction of stepfather
  ind conflict(point *);
  DT_node *find_conflict(point *p);
  void output();
  ind cw_neighbor_ind(point *p)
  {
    return ((p == vertices[0]) ? 2 : ((p == vertices[1]) ? 0 : 1));
  }
  ind neighbor_ind(DT_node *n)
  {
    return ((neighbors[0] == n) ? 0 : ((neighbors[1] == n) ? 1 : 2));
  }

public:
  friend class Delaunay_tree;
};

//****************************************************************************80

int main()
{
  Delaunay_tree DT;
  int i = 0;
  double x;
  double y;

  while (cin >> x >> y)
  {
    DT += new point(x, y);
    i++;
    DT.output(i);
  }

  // DT.output ();

  exit(0);
}
//****************************************************************************80

DT_list::~DT_list()
//    DT_list::~DT_list is the annihilator for class DT_list.
{
  DT_list *p, *q;

  if (!next)
    return;

  for (p = this, q = this->next; q;)
  {
    p->next = NULL;
    delete p;
    p = q;
    q = q->next;
  }
}
//****************************************************************************80

ind DT_node::conflict(point *p)
{
  switch (flag.is_infinite())
  {
  case 4:
    return 0;
  case 3:
    return 1;
  case 2:
    return ((*p - *vertices[0]) * (*vertices[1] + *vertices[2]) >= 0);
  case 1:
    return ((flag.is_last_finite())
                ? (((*p - *vertices[2]) ^ (*vertices[2] - *vertices[0])) >= 0)
                : (((*p - *vertices[0]) ^ (*vertices[0] - *vertices[1])) >= 0));
  case 0:
    //
    //  compute the det 4*4 column: x,y,x**2+y**2,1 for p and vertices [0,1,2]
    //
    double x, y;
    double x0, y0;
    double x1, y1;
    double x2, y2;
    double z1, z2;
    double alpha, beta, gamma;
    x = p->X();
    y = p->Y();
    x0 = vertices[0]->X();
    y0 = vertices[0]->Y();
    x1 = vertices[1]->X();
    y1 = vertices[1]->Y();
    x2 = vertices[2]->X();
    y2 = vertices[2]->Y();
    x1 -= x0;
    y1 -= y0;
    x2 -= x0;
    y2 -= y0;
    x -= x0;
    y -= y0;
    z1 = (x1 * x1) + (y1 * y1);
    z2 = (x2 * x2) + (y2 * y2);
    alpha = (y1 * z2) - (z1 * y2);
    beta = (x2 * z1) - (x1 * z2);
    gamma = (x1 * y2) - (y1 * x2);
    return ((alpha * x) + (beta * y) + gamma * ((x * x) + (y * y)) <= 0);
  }
  return 0;
}
//****************************************************************************80

DT_node::DT_node()
{
  vertices[0] = new point(1e7, 0);
  vertices[1] = new point ( -0.5*1e7,  0.8660254*1e7 );
  vertices[2] = new point ( -0.5*1e7, -0.8660254*1e7 );

  flag.infinite(3);
  nb = 0;
  sons = NULL;
}
//****************************************************************************80

DT_node::DT_node(DT_node *root, ind i)
{
  vertices[0] = root->vertices[0];
  vertices[1] = root->vertices[1];
  vertices[2] = root->vertices[2];
  flag.infinite(4);
  nb = 0;
  sons = NULL;
  neighbors[i] = root;
  root->neighbors[i] = this;
}
//****************************************************************************80

DT_node::DT_node(DT_node *f, point *c, ind i)
//    The triangle is created in ccw order.
//    The circumdisk and flatness are not computed.
{
  switch (f->flag.is_infinite())
  {
  case 0:
    flag.infinite(0);
    break;

  case 1:
    if (f->flag.is_last_finite())
      flag.infinite((i == 1) ? 0 : 1);
    else
      flag.infinite((i == 2) ? 0 : 1);
    if (flag.is_infinite())
    {
      if (f->flag.is_last_finite())
      {
        if (i == 0)
          flag.last_finite();
      }
      else
      {
        if (i == 1)
          flag.last_finite();
      }
    }
    break;

  case 2:
    flag.infinite((i == 0) ? 2 : 1);
    if (i == 1)
      flag.last_finite();
    break;

  case 3:
    flag.infinite(2);
    break;
  }

  nb = 0;
  sons = NULL;
  f->sons = new DT_list(f->sons, this);
  f->neighbors[i]->sons = new DT_list(f->neighbors[i]->sons, this);
  f->neighbors[i]->neighbors[f->neighbors[i]->neighbor_ind(f)] = this;
  vertices[0] = c;
  neighbors[0] = f->neighbors[i];
  switch (i)
  {
  case 0:
    vertices[1] = f->vertices[1];
    vertices[2] = f->vertices[2];
    break;
  case 1:
    vertices[1] = f->vertices[2];
    vertices[2] = f->vertices[0];
    break;
  case 2:
    vertices[1] = f->vertices[0];
    vertices[2] = f->vertices[1];
    break;
  }
}
//****************************************************************************80

DT_node *DT_node::find_conflict(point *p)
{
  DT_list *l;
  DT_node *n;

  if (!conflict(p))
  {
    return NULL;
  }

  if (!flag.is_dead())
  {
    return this;
  }

  for (l = sons; l; l = l->next)
  {
    if (l->key->nb != nb)
    {
      l->key->nb = nb;
      n = l->key->find_conflict(p);
      if (n)
      {
        return n;
      }
    }
  }

  return NULL;
}
//****************************************************************************80

void DT_node::output()
{
  DT_list *l;

  if (flag.is_dead())
  {
    for (l = sons; l; l = l->next)
      if (l->key->nb != nb)
      {
        l->key->nb = nb;
        l->key->output();
      }
    return;
  }

  if (neighbors[0]->nb != nb)
    if (!flag.is_infinite())
      vertices[1]->lineto(vertices[2]);

  if (neighbors[1]->nb != nb)
  {
    if (!flag.is_infinite())
      vertices[2]->lineto(vertices[0]);
    else if ((flag.is_infinite() == 1) && (flag.is_last_finite()))
      vertices[2]->lineto(vertices[0]);
  }

  if (neighbors[2]->nb != nb)
  {
    if (!flag.is_infinite())
      vertices[0]->lineto(vertices[1]);
    else if ((flag.is_infinite() == 1) && (!flag.is_last_finite()))
      vertices[0]->lineto(vertices[1]);
  }
}
//****************************************************************************80

void Delaunay_tree::output(int i)
{
  cout << i << endl;
  root->nb = ++nb;
  root->output();
}

//****************************************************************************80
Delaunay_tree::Delaunay_tree()
//    Delaunay_tree::Delaunay_tree () initializes the Delaunay tree.
{
  nb = 0;
  root = new DT_node();
  new DT_node(root, 0);
  new DT_node(root, 1);
  new DT_node(root, 2);
  root->neighbors[0]->neighbors[1] = root->neighbors[1];
  root->neighbors[0]->neighbors[2] = root->neighbors[2];
  root->neighbors[1]->neighbors[0] = root->neighbors[0];
  root->neighbors[1]->neighbors[2] = root->neighbors[2];
  root->neighbors[2]->neighbors[0] = root->neighbors[0];
  root->neighbors[2]->neighbors[1] = root->neighbors[1];
}
//****************************************************************************80

Delaunay_tree::~Delaunay_tree()
{
  nb++;
}
//****************************************************************************80

Delaunay_tree &Delaunay_tree::operator+=(point *p)
{
  //  + is the insertion operator for class Delaunay_tree.
  DT_node *n;
  DT_node *created;
  DT_node *last;
  DT_node *first;
  point *q;
  point *r;
  ind i;

  root->nb = ++nb;

  if (!(n = root->find_conflict(p)))
  {
    return *this;
  }
  //
  // test if p is already inserted
  //
  for (i = 0; (int)i < 3 - (int)n->flag.is_infinite(); i++)
    if ((p->X() == n->vertices[i]->X()) && (p->Y() == n->vertices[i]->Y()))
      return *this;
  n->flag.kill();
  // we will turn cw around first vertex of n, till next triangle
  // is not in conflict
  q = n->vertices[0];
  while (n->neighbors[i = n->cw_neighbor_ind(q)]->conflict(p))
  {
    n = n->neighbors[i];
    n->flag.kill();
  }

  first = last = new DT_node(n, p, i);
  // we will turn cw around r, till next triangle is not in conflict
  r = n->vertices[(((int)i) + 2) % 3];
  while (1)
  {
    i = n->cw_neighbor_ind(r);
    if (n->neighbors[i]->flag.is_dead())
    {
      n = n->neighbors[i];
      continue;
    }
    if (n->neighbors[i]->conflict(p))
    {
      n = n->neighbors[i];
      n->flag.kill();
      continue;
    }
    break;
  }
  // n is killed by p
  // n->neighbors[i] is not in conflict with p
  // r is vertex i+1 of n
  while (1)
  {

    created = new DT_node(n, p, i);
    created->neighbors[2] = last;
    last->neighbors[1] = created;
    last = created;
    r = n->vertices[(((int)i) + 2) % 3]; // r turn in n ccw
    if (r == q)
      break;
    // we will turn cw around r, till next triangle is not in conflict
    while (1)
    {
      i = n->cw_neighbor_ind(r);
      if (n->neighbors[i]->flag.is_dead())
      {
        n = n->neighbors[i];
        continue;
      }
      if (n->neighbors[i]->conflict(p))
      {
        n = n->neighbors[i];
        n->flag.kill();
        continue;
      }
      break;
    }
  }
  first->neighbors[2] = last;
  last->neighbors[1] = first;
  return *this;
}