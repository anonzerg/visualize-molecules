#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <raylib.h>
#include <stddef.h>

#define MAX_ATOMS 999        // max atoms per frame
#define MAX_BONDS_PER_ATOM 8 // max bonds any atom can have
#define BOND_SCALE 1.2       // bond detection: dist < BOND_SCALE * (ri + rj)
#define CHUNK 1024           // initial alloc size for frames

/* one atom: element index and 2D coordinates  */
typedef struct {
  int q;
  float x, y;
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

extern Frame *frames;
extern int nFrames;
extern int currentFrame;
extern Bond *bonds;
extern int nBonds;

extern int showBonds;
extern int showLabels;
extern float atomScale;
extern int playing;
extern float palyTimer;
extern float playDelay;

extern float zoom;
extern float panX;
extern float panY;
extern float rot;

int xyz(const char *path);
void centerFrame(Frame *fr);
void computeBonds(const Frame *fr);
void drawMolecule(const Frame *fr);
void freeAll(void);

#endif /* VISUALIZE_H  */
