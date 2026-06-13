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
static Color atom_colors[119] = {
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

Frame *frames;     // all frames from the xyz file
static int fCap;   // allocated capacity for frames
int nFrames;       // number of frames
int currentFrame;  // currently displayed frame index
Bond *bonds;       // bond list for current frame
static int bCap;   // allocated capacity for bonds
int nBonds;        // number of bonds

int showBonds = 0;      // toggle bond rendering
int showLabels = 0;     // toggle element labels
float atomScale = 2.0f; // atom size multiplier
int playing = 0;        // animation state
float palyTimer = 0.0f; // seconds since last frame advance
float playDelay = 0.1f; // seconds between frames during playback

float zoom = 60.0f;       // pixels per angstrom
float panX = 0, panY = 0; // screen-space pan offset
float rot = 0.0f;         // rotation angle (radians)

/* realloc wrapper  */
static void *xrealloc(void *p, size_t n) {
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
    frames = xrealloc(frames, fCap * sizeof(Frame));
    memset(frames + nFrames, 0, (fCap - nFrames) * sizeof(Frame));
  }
  return &frames[nFrames++];
}

/* ensure a frame's atom array has room for `n` atoms  */
static void ensureAtoms(Frame *fr, int n) {
  if (n > fr->cap) {
    fr->atoms = xrealloc(fr->atoms, n * sizeof(Atom));
    fr->cap = n;
  }
  fr->nAtoms = n;
}

/* ensure the global bond array has room for `n` bonds  */
static void ensureBonds(int n) {
  if (n > bCap) {
    bonds = xrealloc(bonds, n * sizeof(Bond));
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
    do {
      c = fgetc(f);
    } while (c != EOF && c != '\n');
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
      fr->atoms[i].x = x;
      fr->atoms[i].y = y;
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
  float cx = 0, cy = 0;
  int n = fr->nAtoms;
  for (int i = 0; i < n; i++) {
    cx += fr->atoms[i].x;
    cy += fr->atoms[i].y;
  }
  cx /= n;
  cy /= n;
  for (int i = 0; i < n; i++) {
    fr->atoms[i].x -= cx;
    fr->atoms[i].y -= cy;
  }
}

/* detects bonds for a frame using a distance cutoff based on atom radii  */
void computeBonds(const Frame *fr) {
  nBonds = 0;
  int n = fr->nAtoms;
  int maxb = n * MAX_BONDS_PER_ATOM;
  ensureBonds(maxb);
  for (int i = 0; i < n; i++) {
    float ri = radii[abs(fr->atoms[i].q)] * atomScale;
    for (int j = i + 1; j < n; j++) {
      float rj = radii[abs(fr->atoms[j].q)] * atomScale;
      float dx = fr->atoms[i].x - fr->atoms[j].x;
      float dy = fr->atoms[i].y - fr->atoms[j].y;
      float dist = sqrtf(dx * dx + dy * dy);
      if (dist < BOND_SCALE * (ri + rj) && dist > 0.01f) {
        bonds[nBonds].a = i;
        bonds[nBonds].b = j;
        nBonds++;
      }
    }
  }
}

/* draws a filled circle with outline  */
static void drawAtom(float sx, float sy, float r, Color col) {
  if (r < 1.0f)
    r = 1.0f;
  DrawCircle(sx, sy, r, col);
  DrawCircleLines(sx, sy, r, BLACK);
}

/* draws a bond as a thick line segment  */
static void drawBondLine(float x1, float y1, float x2, float y2, float lw) {
  if (lw < 0.5f)
    lw = 0.5f;
  DrawLineEx((Vector2){x1, y1}, (Vector2){x2, y2}, lw,
             (Color){160, 160, 160, 255});
}

/* renders the current frame: scale and translate atoms, then draw  */
void drawMolecule(const Frame *fr) {
  int n = fr->nAtoms;
  int w = GetScreenWidth(), h = GetScreenHeight();
  float s = cosf(rot), c = sinf(rot);
  float sc = zoom * 0.32f; // size factor: maps radii from Å to screen pixels

  /* draw bonds first  */
  if (showBonds) {
    for (int i = 0; i < nBonds; i++) {
      int ai = bonds[i].a, bi = bonds[i].b;
      float x1 = fr->atoms[ai].x * s - fr->atoms[ai].y * c;
      float y1 = fr->atoms[ai].x * c + fr->atoms[ai].y * s;
      float x2 = fr->atoms[bi].x * s - fr->atoms[bi].y * c;
      float y2 = fr->atoms[bi].x * c + fr->atoms[bi].y * s;
      float lw = (radii[abs(fr->atoms[ai].q)] + radii[abs(fr->atoms[bi].q)]) *
                 sc * 0.3f;
      float sx1 = w / 2 + x1 * zoom + panX, sy1 = h / 2 + y1 * zoom + panY;
      float sx2 = w / 2 + x2 * zoom + panX, sy2 = h / 2 + y2 * zoom + panY;
      drawBondLine(sx1, sy1, sx2, sy2, lw);
    }
  }

  /* draw atoms  */
  for (int i = 0; i < n; i++) {
    int q = abs(fr->atoms[i].q);
    if (q > 118)
      q = 0;
    float x = fr->atoms[i].x * s - fr->atoms[i].y * c;
    float y = fr->atoms[i].x * c + fr->atoms[i].y * s;
    float sx = w / 2 + x * zoom + panX;
    float sy = h / 2 + y * zoom + panY;
    float r = radii[q] * atomScale * sc;
    drawAtom(sx, sy, r, atom_colors[q]);
    if (showLabels) {
      int fs = (int)(sc * 10);
      if (fs < 32)
        fs = 32;
      const char *label = names[q] ? names[q] : "?";
      int tw = MeasureText(label, fs);
      DrawText(label, sx - tw / 2, sy - fs / 2, fs, BLACK);
    }
  }
}
