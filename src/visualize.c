#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "visualize.h"

/* van der Waals radii indexed by atomic number 0-118  */
static const double radii[119] = {
    0.0,   0.455, 0.260, 1.885, 1.365, 1.105, 0.910, 0.845, 0.780, 0.650, 0.520,
    2.340, 1.950, 1.625, 1.430, 1.300, 1.300, 1.300, 1.170, 2.860, 2.340, 2.080,
    1.820, 1.755, 1.820, 1.820, 1.820, 1.755, 1.755, 1.755, 1.755, 1.690, 1.625,
    1.495, 1.495, 1.495, 1.300, 3.055, 2.600, 2.340, 2.015, 1.885, 1.885, 1.755,
    1.690, 1.755, 1.820, 2.080, 2.015, 2.015, 1.885, 1.885, 1.820, 1.820, 1.690,
    3.055, 2.574, 2.197, 2.145, 2.145, 2.132, 2.145, 2.158, 2.405, 2.093, 2.067,
    2.067, 2.054, 2.041, 2.028, 2.210, 2.028, 1.872, 1.742, 1.690, 1.664, 1.638,
    1.638, 1.677, 1.742, 1.872, 2.015, 2.002, 1.976, 1.989, 1.989, 1.950, 3.510,
    2.899, 2.431, 2.314, 2.093, 1.820, 0.0,
};

/* element symbols indexed by atomic number  */
static const char *names[119] = {
    "",   "H",  "He", "Li", "Be", "B",  "C",  "N",  "O",  "F",  "Ne", "Na",
    "Mg", "Al", "Si", "P",  "S",  "Cl", "Ar", "K",  "Ca", "Sc", "Ti", "V",
    "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br",
    "Kr", "Rb", "Sr", "Y",  "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag",
    "Cd", "In", "Sn", "Sb", "Te", "I",  "Xe", "Cs", "Ba", "La", "Ce", "Pr",
    "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu",
    "Hf", "Ta", "W",  "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi",
    "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U",  "Np", "Pu", "Am",
    "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh",
    "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og",
};

/* CPK-inspired atom colours indexed by atomic number  */
static Color atomColors[119] = {
    {0, 0, 0, 255},       {191, 191, 191, 255}, {170, 255, 255, 255},
    {255, 0, 0, 255},     {0, 255, 0, 255},     {255, 221, 255, 255},
    {95, 95, 95, 255},    {31, 31, 191, 255},   {191, 31, 31, 255},
    {245, 255, 133, 255}, {170, 255, 255, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {191, 191, 191, 255}, {95, 95, 95, 255},
    {255, 204, 153, 255}, {255, 238, 17, 255},  {204, 255, 153, 255},
    {170, 255, 255, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {153, 85, 255, 255},  {187, 119, 51, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {255, 170, 17, 255},  {255, 170, 17, 255},  {170, 68, 0, 255},
    {170, 255, 255, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {0, 102, 119, 255},   {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 85, 0, 255},    {170, 0, 255, 255},
    {170, 255, 255, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255}, {170, 170, 170, 255}, {170, 170, 170, 255},
    {170, 170, 170, 255},
};

Frame *frames;    // all frames from the xyz file
static int fCap;  // allocated capacity for frames
int nFrames;      // number of frames
int currentFrame; // currently displayed frame index
Bond *bonds;      // bond list for current frame
static int bCap;  // allocated capacity for bonds
int nBonds;       // number of bonds

int showBonds = 0;      // toggle bond rendering
int showLabels = 0;     // toggle element labels
float atomScale = 2.0f; // atom size multiplier
int playing = 0;        // animation state
float playTimer = 0.0f; // seconds since last frame advance
float playDelay = 0.1f; // seconds between frames during playback

float zoom = 60.0f;              // pixels per angstrom
Vector2 pan = {0.0f, 0.0f};      // screen-space pan offset
float alpha = 0.0f, beta = 0.0f; // rotation angles (radians)

/* realloc wrapper  */
static void *_realloc(void *p, size_t n) {
  p = realloc(p, n);
  if (!p) {
    fprintf(stderr, "error: realloc failed (%zu)\n", n);
    exit(1);
  }
  return p;
}

/* resolve an element string to its atomic number  */
static int getElement(const char *s) {
  char buf[8];
  buf[0] = toupper(s[0]);
  int i = 1;
  for (; s[i] && i < 7; i++)
    buf[i] = tolower(s[i]);
  buf[i] = '\0';
  char *end;
  long q = strtol(s, &end, 10);
  if (end != s)
    return (int)q;
  for (int z = 1; z <= 118; z++) {
    if (names[z] && strcmp(buf, names[z]) == 0)
      return z;
  }
  return 0;
}

/* append a new empty frame, growing `frames` as needed  */
static Frame *addFrame(void) {
  if (nFrames >= fCap) {
    fCap = fCap ? fCap * 2 : CHUNK;
    frames = _realloc(frames, fCap * sizeof(Frame));
    memset(frames + nFrames, 0, (fCap - nFrames) * sizeof(Frame));
  }
  return &frames[nFrames++];
}

/* ensure a frame's atom array has room for `n` atoms  */
static void ensureAtoms(Frame *fr, int n) {
  if (n > fr->cap) {
    fr->atoms = _realloc(fr->atoms, n * sizeof(Atom));
    fr->cap = n;
  }
  fr->nAtoms = n;
}

/* ensure the global bond array has room for `n` bonds  */
static void ensureBonds(int n) {
  if (n > bCap) {
    bonds = _realloc(bonds, n * sizeof(Bond));
    bCap = n;
  }
}

/* you are free to go!  */
void freeAll(void) {
  for (int i = 0; i < nFrames; i++)
    free(frames[i].atoms);
  free(frames);
  frames = NULL;
  nFrames = fCap = 0;
  free(bonds);
  bonds = NULL;
  bCap = 0;
}

/* read a multi-frame xyz file into the `frames` array  */
/* format:
 *   <number of atoms>
 *   comment line
 *   <element> <x> <y> <z>
 */
int xyz(const char *path) {
  FILE *f = fopen(path, "r");
  if (!f) {
    fprintf(stderr, "error: cannot open %s\n", path);
    return 0;
  }

  int ret = 0;
  while (1) {
    int n;
    if (fscanf(f, "%d", &n) != 1) {
      ret = nFrames > 0;
      break;
    }

    /* eat my comment line  */
    int c;
    while ((c = fgetc(f)) != EOF && c != '\n')
      ;
    while ((c = fgetc(f)) != EOF && c != '\n')
      ;
    if (c == EOF) {
      ret = nFrames > 0;
      break;
    }

    if (n <= 0)
      continue;
    if (n > MAX_ATOMS)
      n = MAX_ATOMS;

    Frame *fr = addFrame();
    ensureAtoms(fr, n);

    char elem[8];
    float x, y, z;
    int count = 0;
    for (int i = 0; i < n; i++) {
      if (fscanf(f, "%7s%f%f%f", elem, &x, &y, &z) != 4)
        break;
      fr->atoms[i].q = getElement(elem);
      fr->atoms[i].pos = (Vector3){x, y, z};
      count++;
    }
    if (count == 0) {
      nFrames--;
      ret = nFrames > 0;
      break;
    }
    fr->nAtoms = count;
  }
  fclose(f);
  return ret;
}

/* translate atoms so their geometric centre is at the origin  */
void centerFrame(Frame *fr) {
  Vector3 center = Vector3Zero();
  int n = fr->nAtoms;
  for (int i = 0; i < n; i++) {
    center = Vector3Add(center, fr->atoms[i].pos);
  }
  center = Vector3Scale(center, 1.0f / n);
  for (int i = 0; i < n; i++) {
    fr->atoms[i].pos = Vector3Subtract(fr->atoms[i].pos, center);
  }
}

/* detects bonds for a frame using a distance cutoff based on atom radii  */
void computeBonds(const Frame *fr) {
  nBonds = 0;
  int n = fr->nAtoms;
  int maxBond = n * MAX_BONDS_PER_ATOM;
  ensureBonds(maxBond);
  for (int i = 0; i < n; i++) {
    float ri = radii[abs(fr->atoms[i].q)] * atomScale;
    for (int j = i + 1; j < n; j++) {
      float rj = radii[abs(fr->atoms[j].q)] * atomScale;
      float distance = Vector3Distance(fr->atoms[i].pos, fr->atoms[j].pos);
      if (distance < BOND_SCALE * (ri + rj) && distance > 0.01f) {
        bonds[nBonds].a = i;
        bonds[nBonds].b = j;
        nBonds++;
      }
    }
  }
}

/* rotate the whole system in 3D */
static Vector3 rotate3D(Vector3 pos, float alpha, float beta) {
  Matrix r = MatrixMultiply(MatrixRotateX(alpha), MatrixRotateY(beta));
  return Vector3Transform(pos, r);
}

/* draws a filled circle with outline  */
static void drawAtom(Vector2 center, float radius, Color color) {
  if (radius < 1.0f)
    radius = 1.0f;
  DrawCircleV(center, radius, color);
  DrawCircleLinesV(center, radius, BLACK);
}

/* draws a bond as a thick line segment  */
static void drawBondLine(Vector2 a, Vector2 b, float lineWidth) {
  if (lineWidth < 0.5f)
    lineWidth = 0.5f;
  DrawLineEx(a, b, lineWidth, GRAY);
}

/* compare pairs of atom by depth  */
int sortByDepth(const void *a, const void *b) {
  float da = ((const AtomSortItem *)a)->depth;
  float db = ((const AtomSortItem *)b)->depth;
  if (da < db)
    return -1;
  if (da > db)
    return 1;
  return 0;
}

/* draw lables on each atom  */
static void drawLabels(const AtomSortItem *items, int n, int fontSize) {
  for (int i = 0; i < n; i++) {
    const char *label = names[items[i].q] ? names[items[i].q] : "?";
    int textWidth = MeasureText(label, fontSize);
    DrawText(label, items[i].center.x - textWidth / 2,
             items[i].center.y - fontSize / 2, fontSize, BLACK);
  }
}

/* renders the current frame: scale and translate atoms, then draw  */
void drawMolecule(const Frame *fr) {
  int n = fr->nAtoms;
  int w = GetScreenWidth(), h = GetScreenHeight();
  float sc = zoom * 0.32f;

  AtomSortItem *items = malloc(n * sizeof(AtomSortItem));
  if (!items)
    return;

  for (int i = 0; i < n; i++) {
    int q = abs(fr->atoms[i].q);
    if (q > 118)
      q = 0;
    Vector3 rot = rotate3D(fr->atoms[i].pos, alpha, beta);
    items[i].idx = i;
    items[i].depth = rot.z;
    items[i].center.x = w / 2 + rot.x * zoom + pan.x;
    items[i].center.y = h / 2 + rot.y * zoom + pan.y;
    items[i].radius = radii[q] * atomScale * sc;
    items[i].color = atomColors[q];
    items[i].q = q;
  }

  qsort(items, n, sizeof(AtomSortItem), sortByDepth);

  /* draw bonds first  */
  if (showBonds) {
    for (int i = 0; i < nBonds; i++) {
      int ai = bonds[i].a, bi = bonds[i].b;
      Vector3 a = rotate3D(fr->atoms[ai].pos, alpha, beta);
      Vector3 b = rotate3D(fr->atoms[bi].pos, alpha, beta);
      float lw = (radii[abs(fr->atoms[ai].q)] + radii[abs(fr->atoms[bi].q)]) *
                 sc * 0.3f;
      Vector2 centerA, centerB;
      centerA.x = w / 2 + a.x * zoom + pan.x;
      centerA.y = h / 2 + a.y * zoom + pan.y;
      centerB.x = w / 2 + b.x * zoom + pan.x;
      centerB.y = h / 2 + b.y * zoom + pan.y;
      drawBondLine(centerA, centerB, lw);
    }
  }

  /* draw atoms  */
  for (int i = 0; i < n; i++) {
    drawAtom(items[i].center, items[i].radius, items[i].color);
  }
  if (showLabels) {
    int fontSize = (int)(sc * 10);
    if (fontSize < 32)
      fontSize = 32;
    drawLabels(items, n, fontSize);
  }

  free(items);
}
