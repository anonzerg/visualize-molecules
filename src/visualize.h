#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <raylib.h>
#include <raymath.h>
#include <stddef.h>

#define MAX_ATOMS 999        // max atoms per frame
#define MAX_BONDS_PER_ATOM 8 // max bonds any atom can have
#define BOND_SCALE 1.2       // bond detection: dist < BOND_SCALE * (ri + rj)
#define CHUNK 1024           // initial alloc size for frames

/* one atom: element index and 2D coordinates  */
typedef struct {
  int q;
  Vector3 pos;
} Atom;

/* one bond: indices of the two atoms in the current frame  */
typedef struct {
  int a, b;
} Bond;

/* one frame: dynamic atom array  */
typedef struct {
  Atom *atoms;
  int nAtoms, cap;
} Frame;

/* container for sorted atoms by depth  */
typedef struct {
  int idx;
  float depth;
  Vector2 center;
  float radius;
  Color color;
  int q;
} AtomSortItem;

extern Frame *frames;
extern int nFrames;
extern int currentFrame;
extern Bond *bonds;
extern int nBonds;

extern int showBonds;
extern int showLabels;
extern float atomScale;
extern int playing;
extern float playTimer;
extern float playDelay;

extern float zoom;
extern Vector2 pan;
extern float alpha;
extern float beta;

int xyz(const char *path);
void centerFrame(Frame *fr);
void computeBonds(const Frame *fr);
int sortByDepth(const void *a, const void *b);
void drawMolecule(const Frame *fr);
void freeAll(void);

#endif /* VISUALIZE_H  */
