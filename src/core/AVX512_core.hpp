#pragma once
#ifndef DD_AVX_CORE_HPP_
#define DD_AVX_CORE_HPP_

#include <immintrin.h>
#define SIMD_FUNC(NAME) _mm512_##NAME
using reg = __m512d;
struct dd_real;

namespace ddavx_core{
	const int SIMD_Length=8;
	class registers{
		public:
			double splitter = 134217729.0;
			reg sp = SIMD_FUNC(set1_pd)(&splitter);
			reg minus = SIMD_FUNC(set_pd)(-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0,-1.0);
			reg zeros = SIMD_FUNC(set_pd)(0,0,0,0,0,0,0,0);
			reg one,bh,bl,ch,cl,sh,sl,wh,wl,th,tl,p1,p2,t0,t1,t2,eh,t3; 
	};


	/////////////////////////////////////////////
	//core functions
	////////////////////////////////////////////
	
	//load
	inline reg load(const double& val){
		return SIMD_FUNC(loadu_pd)((double*)&(val)); 
	}

	//set
	inline reg set(const double a, const double b, const double c, const double d, const double e, const double f, const double g, const double h){
		reg ret = SIMD_FUNC(set_pd)(a, b, c, d, e, f, g, h);
		return ret;
	}

	//broadcast
	inline reg broadcast(const double a){
		reg ret = SIMD_FUNC(set1_pd)(&a); 
		return ret;
	}

	//store
	inline void store(double& ret, const reg val){
		SIMD_FUNC(storeu_pd)((double*)&(ret), val);
	}

	// add
	inline reg add(const reg a, const reg b){
		return SIMD_FUNC(add_pd)(a, b);
	}

	// mul
	inline reg mul(const reg a, const reg b){
		return SIMD_FUNC(mul_pd)(a, b);
	}

	// sub
	inline reg sub(const reg a, const reg b){
		return SIMD_FUNC(sub_pd)(a, b);
	}

	// div
	inline reg div(const reg a, const reg b){
		return SIMD_FUNC(div_pd)(a, b);
	}

	// fmadd
	inline reg fmadd(const reg a, const reg b, const reg c){
		return SIMD_FUNC(fmadd_pd)(a, b, c);
	}


	/////////////////////////////////////////////
	// special functions
	////////////////////////////////////////////
	
	// need to change for AVX512
	inline dd_real reduction(reg a_hi, reg a_lo){
		double hi[8];
		double lo[8];
		store(*hi, a_hi); 
		store(*lo, a_lo); 

		dd_real a; a.x[0] = hi[0]; a.x[1] = lo[0];
		dd_real b; b.x[0] = hi[1]; b.x[1] = lo[1];
		dd_real c; c.x[0] = hi[2]; c.x[1] = lo[2];
		dd_real d; d.x[0] = hi[3]; d.x[1] = lo[3];
		dd_real e; e.x[0] = hi[4]; e.x[1] = lo[4];
		dd_real f; f.x[0] = hi[5]; f.x[1] = lo[5];
		dd_real g; g.x[0] = hi[6]; g.x[1] = lo[6];
		dd_real h; h.x[0] = hi[7]; h.x[1] = lo[7];

		return a + b + c + d + e + f + g + h;
	}

	// need to change for AVX512
	inline void store(double& ret1, double& ret2, double& ret3, double& ret4, const reg val){
		double tmp[8];
		store(*tmp, val); 
		ret1 = tmp[0];
		ret2 = tmp[1];
		ret3 = tmp[2];
		ret4 = tmp[3];
		ret5 = tmp[4];
		ret6 = tmp[5];
		ret7 = tmp[6];
		ret8 = tmp[7];
	}

	inline void print(reg a){
		double tmp[SIMD_Length];
		store(*tmp, a); 
		for(int i=0; i<SIMD_Length; i++){
			printf("%f ", tmp[i]);
		}
		printf("\n");
	
	}

	//isie
	inline void get_isie(int size, int& is, int& ie){
		int maxid = omp_get_max_threads();
		int tid = omp_get_thread_num();

		if( tid < (size % maxid) ) 
		{ 
			ie = size / maxid+1; 
			is = ie * tid; 
		} 
		else 
		{ 
			ie = size / maxid; 
			is = ie * tid + size % maxid; 
		} 
		ie = ie + is;
	}
	inline void to_minus(double& val, registers& regs){
		reg reg = load(val);
 		reg = mul(reg, regs.minus);
 		store(val, reg);
	}

	inline void to_minus(reg& reg, registers& regs){
 		reg = mul(reg, regs.minus);
	}
}
#endif
