/*!************************************************************************
\file Interpolate.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Interpolation Programming Assignment
\date 12-12-2023
\brief
Implement function about Interpolate
**************************************************************************/
#include "Interpolate.h"

float TexturedCoord::color_scale = 1.0f;
Texture* TexturedCoord::texture = nullptr;

/*!*****************************************************************************
\brief
	Fill triangle

\param r
	Raster object
\param P
	Point 1
\param Q
	Point 2
\param R
	Point 3

*******************************************************************************/
void FillTriangle(Raster& r, const TexturedCoord& P, const TexturedCoord& Q, const TexturedCoord& R) {
	TexturedCoord nP = TexturedCoord(Hcoord{ P.x / P.w, P.y / P.w, P.z / P.w, P.w / P.w }, P.u, P.v);
	TexturedCoord nQ = TexturedCoord(Hcoord{ Q.x / Q.w, Q.y / Q.w, Q.z / Q.w, Q.w / Q.w }, Q.u, Q.v);
	TexturedCoord nR = TexturedCoord(Hcoord{ R.x / R.w, R.y / R.w, R.z / R.w, R.w / R.w }, R.u, R.v);

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
					float z		= w * nP.z + u * nQ.z + v * nR.z;
					float tex_u = w * nP.u + u * nQ.u + v * nR.u;
					float tex_v = w * nP.v + u * nQ.v + v * nR.v;
					Vector color = TexturedCoord::texture->uvToRGB(tex_u, tex_v);
					r.SetColor(static_cast<unsigned char>(color.x), static_cast<unsigned char>(color.y), static_cast<unsigned char>(color.z));
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


/*!*****************************************************************************
\brief
	Clip texture coord

\param vertices
	First coordinates vector

\return
	if makeing intersection point is true
	otherwise false
*******************************************************************************/
bool TextureClip::operator()(std::vector<TexturedCoord>& vertices)
{
	std::vector<TexturedCoord> first_vec = vertices;
	std::vector<TexturedCoord> store_vec = vertices;

	for (const HalfSpace& space : half_spaces) {
		std::vector<float> dot_result;
		size_t size = store_vec.size();
		TexturedCoord last_point;
		bool last = false;
		int positive_three = 0;
		for (int i = 0; i < size; i++) {
			float tmp = dot(space, store_vec[i]);
			if (tmp > 0) { positive_three++; }
			dot_result.push_back(tmp);
		}
		if (positive_three == size) {
			return false;
		}

		for (int i = 0; i < size; i++) {
			if (dot_result[i] <= 0) {
				temp_vertices.push_back(store_vec[i]);
			}
			else {
				float t = 0;
				float t2 = 0;
				TexturedCoord inter_point;
				TexturedCoord inter_point2;
				if (i == 0) {
					t = dot_result[size - 1] / (dot_result[size - 1] - dot_result[i]);
					float u = ((1 - t) * store_vec[size - 1].u) + (t * store_vec[i].u);
					float v = ((1 - t) * store_vec[size - 1].v) + (t * store_vec[i].v);
					inter_point = TexturedCoord((store_vec[size - 1]) + (t *(store_vec[i] - store_vec[size - 1])), u, v);


					
					t2 = dot_result[i] / (dot_result[i] - dot_result[i + 1]);
					u = ((1 - t2) * store_vec[i].u) + (t2 * store_vec[i + 1].u);
					v = ((1 - t2) * store_vec[i].v) + (t2 * store_vec[i + 1].v);
					inter_point2 = TexturedCoord(store_vec[i] + (t2 * (store_vec[i + 1] - store_vec[i])), u, v);

					if (dot_result[size - 1] < 0) {
						last_point = inter_point;
						last = true;
					}

					if (dot_result[i + 1] < 0) {
						temp_vertices.push_back(inter_point2);
					}
				}
				else if (i == size - 1) {
					t = dot_result[i - 1] / (dot_result[i - 1] - dot_result[i]);
					float u = ((1 - t) * store_vec[i - 1].u) + (t * store_vec[i].u);
					float v = ((1 - t) * store_vec[i - 1].v) + (t * store_vec[i].v);
					inter_point = TexturedCoord(store_vec[i - 1] + (t * (store_vec[i] - store_vec[i - 1])), u, v);

					
					t2 = dot_result[i] / (dot_result[i] - dot_result[0]);
					u = ((1 - t2) * store_vec[i].u) + (t2 * store_vec[0].u);
					v = ((1 - t2) * store_vec[i].v) + (t2 * store_vec[0].v);
					inter_point2 = TexturedCoord(store_vec[i] + (t2 * (store_vec[0] - store_vec[i])), u, v);

					if (dot_result[i - 1] < 0) {
						temp_vertices.push_back(inter_point);
					}
					if (dot_result[0] < 0) {
						temp_vertices.push_back(inter_point2);
					}
				}
				else {
					t = dot_result[i - 1] / (dot_result[i - 1] - dot_result[i]);
					float u = ((1 - t) * store_vec[i - 1].u) + (t * store_vec[i].u);
					float v = ((1 - t) * store_vec[i - 1].v) + (t * store_vec[i].v);
					inter_point = TexturedCoord(store_vec[i - 1] + (t * (store_vec[i] - store_vec[i - 1])), u, v);

					t2 = dot_result[i] / (dot_result[i] - dot_result[i + 1]);
					u = ((1 - t2) * store_vec[i].u) + (t2 * store_vec[i + 1].u);
					v = ((1 - t2) * store_vec[i].v) + (t2 * store_vec[i + 1].v);
					inter_point2 = TexturedCoord(store_vec[i] + (t2 * (store_vec[i + 1] - store_vec[i])), u, v);

					if (dot_result[i - 1] < 0) {
						temp_vertices.push_back(inter_point);
					}
					if (dot_result[i + 1] < 0) {
						temp_vertices.push_back(inter_point2);
					}
				}
			}
		}
		if (last) {
			temp_vertices.push_back(last_point);
		}
		store_vec = temp_vertices;
		temp_vertices.clear();
	}


	vertices = store_vec;

	return true;
}
