/*!************************************************************************
\file RasterUtilities.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Z Buffer Programming Assignment
\date 11-25-2023
\brief
Implement function about Rasterization
**************************************************************************/

#include "RasterUtilities.h"
#include <cmath>

/*!*****************************************************************************
\brief
	Clear buffer
\param r
	object that have buffer
\param z
	default z value
*******************************************************************************/
void ClearBuffers(Raster& r, float z)
{
	int x = r.Width();
	int y = r.Height();
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			r.GotoPoint(i, j);
			r.WritePixel();
			r.WriteZ(z);
		}
	}

}

void FillTriangle(Raster& r, const Hcoord& P, const Hcoord& Q, const Hcoord& R) {
    Hcoord nP = Hcoord{ P.x / P.w, P.y / P.w, P.z / P.w, P.w / P.w };
    Hcoord nQ = Hcoord{ Q.x / Q.w, Q.y / Q.w, Q.z / Q.w, Q.w / Q.w };
    Hcoord nR = Hcoord{ R.x / R.w, R.y / R.w, R.z / R.w, R.w / R.w };

    int minX = static_cast<int>(std::min({ nP.x, nQ.x, nR.x }));
    int maxX = static_cast<int>(std::max({ nP.x, nQ.x, nR.x }));
    int minY = static_cast<int>(std::min({ nP.y, nQ.y, nR.y }));
    int maxY = static_cast<int>(std::max({ nP.y, nQ.y, nR.y }));

    Hcoord PQ = nQ - nP;
    Hcoord PR = nR - nP;

    float determinant = (PQ.x * PR.y) - (PR.x * PQ.y);

    for (int x = minX; x <= maxX; ++x)
    {
        for (int y = minY; y <= maxY; ++y)
        {
            Hcoord I(static_cast<float>(x), static_cast<float>(y));

            Hcoord IP = I - nP;

            if (determinant != 0) {
                float u = ((IP.x * PR.y) - (PR.x * IP.y)) / determinant;
                float v = ((PQ.x * IP.y) - (IP.x * PQ.y)) / determinant;
                float w = 1.0f - u - v;

                if ((u >= 0) && (v >= 0) && (w >= 0)) {
                    float z = w * nP.z + u * nQ.z + v * nR.z;

                    r.GotoPoint(x, y);
                    if (z < r.GetZ()) {
                        r.WritePixel();
                        r.WriteZ(z);
                    }
                }
            }
        }
    }
}

//namespace helper
//{
//	struct Formula {
//		float a = 0;
//		float b = 0;
//		float c = 0;
//		bool tl = false;
//	};
//	/*!*****************************************************************************
//\brief
//	make line equation
//\param one
//	Point 1
//\param two
//	Point 2
//\return
//	line equation
//*******************************************************************************/
//	Formula create_edge(Hcoord one, Hcoord two) {
//		Formula result;
//		result.a = (one.y - two.y);
//		result.b = -(one.x - two.x);
//		result.c = one.x * two.y - two.x * one.y;
//
//		result.tl = (result.a != 0.0f) ? (result.a > 0.f) : (result.b < 0.f);
//		return result;
//	}
//
//	/*!*****************************************************************************
//\brief
//	check point is inside of equation
//\param E
//	line equation
//\param P
//	Point
//\return
//	check result
//*******************************************************************************/
//	bool PointInEdgeTopLeft(Formula E, Vector P) {
//		float eval = E.a * P.x + E.b * P.y + E.c;
//		return (eval > 0 || (eval == 0 && E.tl)) ? true : false;
//	}
//
//	/*!*****************************************************************************
//\brief
//	make interporation value
//\param P
//	vertex 1
//\param Q
//	vertex 2
//\param R
//	vertex 3
//\param x
//	x position
//\param y
//	y position
//\return
//	interporation value
//*******************************************************************************/
//	float Interporation(const Hcoord& P, const Hcoord& Q, const Hcoord& R, int x, int y) {
//		Hcoord a = P;
//		Hcoord b = Q;
//		Hcoord c = R;
//
//		Vector normal_v = cross((a - c), (b - c));
//		float d = (normal_v.x * a.x) + (normal_v.y * a.y) + (normal_v.z * a.z);
//		float result = (d - (normal_v.x * x) - (normal_v.y * y)) / normal_v.z;
//		return result;
//
//		
//	}
//}

/*!*****************************************************************************
\brief
	Fill Triangle
\param r
	object that have buffer
\param P
	vertex 1
\param Q
	vertex 2
\param R
	vertex 3
*******************************************************************************/
//void FillTriangle(Raster& r, const Hcoord& P, const Hcoord& Q, const Hcoord& R)
//{
//	Hcoord small = P.y < Q.y ? (P.y < R.y ? P : R) : (Q.y < R.y ? Q : R);
//	Hcoord mid = P.y < Q.y ? (P.y < R.y ? (Q.y < R.y ? Q : R) : P) : (Q.y < R.y ? (P.y < R.y ? P : R) : Q);
//	Hcoord big = P.y < Q.y ? (Q.y < R.y ? R : Q) : (P.y < R.y ? R : P);
//
//	helper::Formula E0;
//	helper::Formula E1;
//	helper::Formula E2;
//
//	small = 1 / small.w * small;
//	mid = 1 / mid.w * mid;
//	big = 1 / big.w * big;
//
//	float small_mid_m1 = (mid.x - small.x) / (mid.y - small.y);
//	float small_big_m2 = (big.x - small.x) / (big.y - small.y);
//
//	float big_mid_m1 = (mid.x - big.x) / (mid.y - big.y);
//	float big_small_m2 = (small.x - big.x) / (small.y - big.y);
//
//	float xmin = 0;
//	float xmax = 0;
//
//	int jmin = 0;
//	int jmax = 0;
//
//	jmin = static_cast<int>(ceil(small.y));
//	jmax = static_cast<int>(floor(mid.y));
//
//	///lower portion
//	for (int j = jmin; j <= jmax; j++) {
//		if (small_mid_m1 < small_big_m2) {
//			xmax = small.x + (small_big_m2 * (j - small.y));
//			xmin = small.x + (small_mid_m1 * (j - small.y));
//			E0 = helper::create_edge(small, big);
//			E1 = helper::create_edge(big, mid);
//			E2 = helper::create_edge(mid, small);
//		}
//		else {
//			xmax = small.x + (small_mid_m1 * (j - small.y));
//			xmin = small.x + (small_big_m2 * (j - small.y));
//			E0 = helper::create_edge(small, mid);
//			E1 = helper::create_edge(mid, big);
//			E2 = helper::create_edge(big, small);
//		}
//
//		int imin = static_cast<int>(ceil(xmin));
//		int imax = static_cast<int>(floor(xmax));
//
//		r.GotoPoint(imin, j);
//		for (int i = imin; i <= imax; i++) {
//			Vector tmp(static_cast<float>(i), static_cast<float>(j));
//			if (PointInEdgeTopLeft(E0, tmp) &&
//				PointInEdgeTopLeft(E1, tmp) &&
//				PointInEdgeTopLeft(E2, tmp)) {
//				float z = helper::Interporation(small, mid, big, i, j);
//				if (z < r.GetZ()) {
//					r.WritePixel();
//					r.WriteZ(z);
//				}
//			}
//			r.IncrementX();
//
//		}
//	}
//
//	///higher portion
//	jmin = static_cast<int>(ceil(mid.y));
//	jmax = static_cast<int>(floor(big.y));
//
//	for (int j = jmin; j <= jmax; j++) {
//		if (big_small_m2 < big_mid_m1) {
//			xmin = big.x + (big_mid_m1 * (j - big.y));
//			xmax = big.x + (big_small_m2 * (j - big.y));
//			E0 = helper::create_edge(small, big);
//			E1 = helper::create_edge(big, mid);
//			E2 = helper::create_edge(mid, small);
//		}
//		else {
//			xmin = big.x + (big_small_m2 * (j - big.y));
//			xmax = big.x + (big_mid_m1 * (j - big.y));
//			E0 = helper::create_edge(small, mid);
//			E1 = helper::create_edge(mid, big);
//			E2 = helper::create_edge(big, small);
//		}
//
//		int imin = static_cast<int>(ceil(xmin));
//		int imax = static_cast<int>(floor(xmax));
//
//		r.GotoPoint(imin, j);
//		for (int i = imin; i <= imax; i++) {
//			Vector tmp(static_cast<float>(i), static_cast<float>(j));
//			if (PointInEdgeTopLeft(E0, tmp) &&
//				PointInEdgeTopLeft(E1, tmp) &&
//				PointInEdgeTopLeft(E2, tmp)) {
//				float z = helper::Interporation(small, mid, big, i, j);
//				if (z < r.GetZ()) {
//					r.WritePixel();
//					r.WriteZ(z);
//				}
//			}
//			r.IncrementX();
//		}
//	}
//
//}