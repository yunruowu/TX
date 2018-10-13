#include<stdio.h>
#include<x86intrin.h>
int main(int argc,char* agrgv[])
{
  __m128 a;
  __m128 b;
  float fa[4] = {3.0,2.0,1.0,-1.0};
  float fb[4] = {3.0,2.0,1.0,-1.0};
  float *fp;
  a = _mm_load_ps(&fa[0]);
  b = _mm_load_ps(&fb[0]);
  __m128 c = _mm_add_ps(a,b);
  fp = (float*)(&c);
  printf("vector:(%lf,%lf,%lf,%lf)\n",fp[0],fp[1],fp[2],fp[3]);
  return 0;
}
