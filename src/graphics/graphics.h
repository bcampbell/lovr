#include "graphics/font.h"
#include "graphics/material.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "math/math.h"
#include "lib/glfw.h"
#include <stdbool.h>

#pragma once

#define MAX_VIEWS 4
#define MAX_TRANSFORMS 60
#define INTERNAL_TRANSFORMS 4
#define DEFAULT_SHADER_COUNT 4
#define MAX_TEXTURES 16

typedef enum {
  BLEND_ALPHA,
  BLEND_ADD,
  BLEND_SUBTRACT,
  BLEND_MULTIPLY,
  BLEND_LIGHTEN,
  BLEND_DARKEN,
  BLEND_SCREEN,
  BLEND_REPLACE
} BlendMode;

typedef enum {
  BLEND_ALPHA_MULTIPLY,
  BLEND_PREMULTIPLIED
} BlendAlphaMode;

typedef enum {
  DRAW_MODE_FILL,
  DRAW_MODE_LINE
} DrawMode;

typedef enum {
  ARC_MODE_PIE,
  ARC_MODE_OPEN,
  ARC_MODE_CLOSED
} ArcMode;

typedef enum {
  WINDING_CLOCKWISE = GL_CW,
  WINDING_COUNTERCLOCKWISE = GL_CCW
} Winding;

typedef enum {
  COMPARE_NONE = 0,
  COMPARE_EQUAL = GL_EQUAL,
  COMPARE_NOT_EQUAL = GL_NOTEQUAL,
  COMPARE_LESS = GL_LESS,
  COMPARE_LEQUAL = GL_LEQUAL,
  COMPARE_GEQUAL = GL_GEQUAL,
  COMPARE_GREATER = GL_GREATER
} CompareMode;

typedef enum {
  MATRIX_MODEL,
  MATRIX_VIEW
} MatrixType;

typedef struct {
  bool initialized;
  float pointSizes[2];
  int textureSize;
  int textureMSAA;
  float textureAnisotropy;
} GraphicsLimits;

typedef struct {
  int framebuffer;
  float projection[16];
  int viewport[4];
} View;

typedef struct {
  GLFWwindow* window;
  Shader* defaultShaders[DEFAULT_SHADER_COUNT];
  DefaultShader defaultShader;
  Material* defaultMaterial;
  Font* defaultFont;
  Texture* defaultTexture;
  float transforms[MAX_TRANSFORMS + INTERNAL_TRANSFORMS][2][16];
  int transform;
  Color backgroundColor;
  BlendMode blendMode;
  BlendAlphaMode blendAlphaMode;
  Color color;
  bool culling;
  TextureFilter defaultFilter;
  CompareMode depthTest;
  Font* font;
  bool gammaCorrect;
  GraphicsLimits limits;
  float lineWidth;
  float pointSize;
  Shader* shader;
  Winding winding;
  bool wireframe;
  uint32_t streamVAO;
  uint32_t streamVBO;
  uint32_t streamIBO;
  vec_float_t streamData;
  vec_uint_t streamIndices;
  View views[MAX_VIEWS];
  int view;
  Texture* textures[MAX_TEXTURES];
  uint32_t program;
  uint32_t vertexArray;
  uint32_t vertexBuffer;
  uint32_t indexBuffer;
} GraphicsState;

// Base
void lovrGraphicsInit();
void lovrGraphicsDestroy();
void lovrGraphicsReset();
void lovrGraphicsClear(bool color, bool depth);
void lovrGraphicsPresent();
void lovrGraphicsPrepare(Material* material, float* pose);
void lovrGraphicsCreateWindow(int w, int h, bool fullscreen, int msaa, const char* title, const char* icon);
int lovrGraphicsGetWidth();
int lovrGraphicsGetHeight();

// State
Color lovrGraphicsGetBackgroundColor();
void lovrGraphicsSetBackgroundColor(Color color);
void lovrGraphicsGetBlendMode(BlendMode* mode, BlendAlphaMode* alphaMode);
void lovrGraphicsSetBlendMode(BlendMode mode, BlendAlphaMode alphaMode);
Color lovrGraphicsGetColor();
void lovrGraphicsSetColor(Color color);
bool lovrGraphicsIsCullingEnabled();
void lovrGraphicsSetCullingEnabled(bool culling);
TextureFilter lovrGraphicsGetDefaultFilter();
void lovrGraphicsSetDefaultFilter(TextureFilter filter);
CompareMode lovrGraphicsGetDepthTest();
void lovrGraphicsSetDepthTest(CompareMode depthTest);
Font* lovrGraphicsGetFont();
void lovrGraphicsSetFont(Font* font);
bool lovrGraphicsIsGammaCorrect();
void lovrGraphicsSetGammaCorrect(bool gammaCorrect);
GraphicsLimits lovrGraphicsGetLimits();
float lovrGraphicsGetLineWidth();
void lovrGraphicsSetLineWidth(float width);
float lovrGraphicsGetPointSize();
void lovrGraphicsSetPointSize(float size);
Shader* lovrGraphicsGetShader();
void lovrGraphicsSetShader(Shader* shader);
Winding lovrGraphicsGetWinding();
void lovrGraphicsSetWinding(Winding winding);
bool lovrGraphicsIsWireframe();
void lovrGraphicsSetWireframe(bool wireframe);

// Transforms
void lovrGraphicsPush();
void lovrGraphicsPop();
void lovrGraphicsOrigin();
void lovrGraphicsTranslate(MatrixType type, float x, float y, float z);
void lovrGraphicsRotate(MatrixType type, float angle, float ax, float ay, float az);
void lovrGraphicsScale(MatrixType type, float x, float y, float z);
void lovrGraphicsMatrixTransform(MatrixType type, mat4 transform);

// Primitives
void lovrGraphicsPoints(float* points, int count);
void lovrGraphicsLine(float* points, int count);
void lovrGraphicsTriangle(DrawMode mode, Material* material, float* points);
void lovrGraphicsPlane(DrawMode mode, Material* material, mat4 transform);
void lovrGraphicsPlaneFullscreen(Texture* texture);
void lovrGraphicsBox(DrawMode mode, Material* material, mat4 transform);
void lovrGraphicsArc(DrawMode mode, ArcMode, Material* material, mat4 transform, float theta1, float theta2, int segments);
void lovrGraphicsCircle(DrawMode mode, Material* material, mat4 transform, int segments);
void lovrGraphicsCylinder(Material* material, float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2, bool capped, int segments);
void lovrGraphicsSphere(Material* material, mat4 transform, int segments);
void lovrGraphicsSkybox(Texture* texture, float angle, float ax, float ay, float az);
void lovrGraphicsPrint(const char* str, mat4 transform, float wrap, HorizontalAlign halign, VerticalAlign valign);

// Internal State
void lovrGraphicsPushView();
void lovrGraphicsPopView();
mat4 lovrGraphicsGetProjection();
void lovrGraphicsSetProjection(mat4 projection);
void lovrGraphicsSetViewport(int x, int y, int w, int h);
void lovrGraphicsBindFramebuffer(int framebuffer);
Texture* lovrGraphicsGetTexture(int slot);
void lovrGraphicsBindTexture(Texture* texture, TextureType type, int slot);
Material* lovrGraphicsGetDefaultMaterial();
void lovrGraphicsSetDefaultShader(DefaultShader defaultShader);
Shader* lovrGraphicsGetActiveShader();
void lovrGraphicsUseProgram(uint32_t program);
void lovrGraphicsBindVertexArray(uint32_t vao);
void lovrGraphicsBindVertexBuffer(uint32_t vbo);
void lovrGraphicsBindIndexBuffer(uint32_t ibo);
