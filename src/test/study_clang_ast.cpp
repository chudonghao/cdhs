extern double sin(double);
extern double foo(double, double, double);

struct Foo;

struct Foo {
  int foo;
  float foo2;
};

int main(int argc, char *argv[]) {
  int v1;
  int v2 = 1;
  float v3;
  float v4 = 1.;
  float v5 = 2.;

  Foo foo_;
  foo_.foo = foo_.foo2;
  *&foo_.foo2;

  v1 = v2 * v2 + v4 * v5;

  v1 = v2 + sin(v4);

  if (v1 + v2) {
    int v1;
  } else if (v1 + v2)
    int v1;

  for (int i = 0, j; i < 10; ++i, ++j) {
    int v1;
  }

  for (int i = 0, j; i < 10; ++i, ++j)
    int v1;

  v1 = v1 ? v1 : (float)v1;

  double v6 = ++v1 + v2++ * (6. + 6.f);

  const char *v7 = "111";

  int v8[10];

  v8[0] = v8[1] = v8[2];

  for (;;)
    ;

  v1 = v1 ? v2 : v3 ? v4 : v5;

  v1 = 1, 2, 3, 4;

  foo(1, 2, 3);

  return 0;
}
