#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include "visualize.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s in.xyz\n", argv[0]);
    return 1;
  }

  if (!xyz(argv[1])) {
    fprintf(stderr, "error: failed to load %s\n", argv[1]);
    return 1;
  }

  /* centre every frame on the origin  */
  for (int i = 0; i < nFrames; i++)
    centerFrame(&frames[i]);

  /* determine zoom from the first frame's extent  */
  const Frame *fr = &frames[0];
  double maxd = 0;
  for (int i = 0; i < fr->nAtoms; i++) {
    double d =
        sqrt(fr->atoms[i].x * fr->atoms[i].x + fr->atoms[i].y * fr->atoms[i].y +
             fr->atoms[i].z * fr->atoms[i].z);
    if (d > maxd)
      maxd = d;
  }
  if (maxd < 1)
    maxd = 1;
  computeBonds(fr);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(1920, 1080, "Visualize Molecules");
  SetTargetFPS(60);

  zoom = fmin(GetScreenWidth(), GetScreenHeight()) / (maxd * 3.5f);

  /* ---- main loop ---- */
  while (!WindowShouldClose()) {
    /* ---- keyboard input ---- */
    if (IsKeyPressed(KEY_B))
      showBonds = !showBonds;
    if (IsKeyPressed(KEY_L))
      showLabels = !showLabels;
    if (IsKeyPressed(KEY_EQUAL) || IsKeyPressed(KEY_KP_ADD)) {
      atomScale *= 1.1f;
      computeBonds(&frames[currentFrame]);
    }
    if (IsKeyPressed(KEY_MINUS) || IsKeyPressed(KEY_KP_SUBTRACT)) {
      atomScale /= 1.1f;
      computeBonds(&frames[currentFrame]);
    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
      playing = 0;
      if (currentFrame < nFrames - 1)
        currentFrame++;
      computeBonds(&frames[currentFrame]);
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
      playing = 0;
      if (currentFrame > 0)
        currentFrame--;
      computeBonds(&frames[currentFrame]);
    }
    if (IsKeyPressed(KEY_ZERO)) {
      currentFrame = 0;
      playing = 0;
      computeBonds(&frames[currentFrame]);
    }
    if (IsKeyPressed(KEY_P)) {
      playing = !playing;
      playTimer = 0;
    }

    /* WASD panning  */
    {
      float ps = 5.0f;
      if (IsKeyDown(KEY_W))
        panY -= ps;
      if (IsKeyDown(KEY_S))
        panY += ps;
      if (IsKeyDown(KEY_A))
        panX -= ps;
      if (IsKeyDown(KEY_D))
        panX += ps;
    }

    /* ---- auto-play ---- */
    if (playing && nFrames > 1) {
      playTimer += GetFrameTime();

      if (playTimer >= playDelay) {
        playTimer = 0;
        if (++currentFrame >= nFrames)
          currentFrame = 0;
        computeBonds(&frames[currentFrame]);
      }
    }

    fr = &frames[currentFrame];

    /* ---- mouse ---- */
    float wheel = GetMouseWheelMove();
    if (wheel != 0)
      zoom *= (wheel > 0) ? 1.1f : 0.9f;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      Vector2 delta = GetMouseDelta();
      rotX += delta.x * 0.005f;
      rotY += delta.y * 0.005f;
    }

    /* ---- draw ---- */
    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawMolecule(fr);

    char buf[256];
    snprintf(buf, sizeof(buf), "%04d / %04d  Atoms: %03d  Bonds: %03d%s",
             currentFrame + 1, nFrames, fr->nAtoms, nBonds,
             playing ? "  [Playing]" : "  [Paused]");
    DrawText(buf, 10, 10, 36, DARKGRAY);
    DrawText("[B] bonds, [L] labels, [R] reset, [+/-] size, "
             "[WASD] panning, [Enter] step, [0] first frame, [P] play/pause, "
             "[Mouse Drag] Rotate",
             10, 52, 36, DARKGRAY);

    EndDrawing();
  }

  freeAll();
  CloseWindow();
  return 0;
}
