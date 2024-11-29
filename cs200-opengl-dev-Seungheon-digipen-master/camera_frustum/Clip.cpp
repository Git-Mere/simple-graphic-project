/*!************************************************************************
\file Clip.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Clipping Programming Assignment
\date 11-29-2023
\brief
Implement function about Clip
**************************************************************************/

#include "Clip.h"

/*!*****************************************************************************
\brief
	modify input vertices to clip vertices

\param vertices
	input value

\return
  if makeing intersection point is true
  otherwise false
*******************************************************************************/
bool Clip::operator()(std::vector<Hcoord>& vertices)
{
	std::vector<Hcoord> first_vec = vertices;
	std::vector<Hcoord> store_vec = vertices;

	for (const HalfSpace& space : half_spaces) {
		std::vector<float> dot_result;
		size_t size = store_vec.size();
		Hcoord last_point;
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
				Hcoord inter_point;
				Hcoord inter_point2;
				if (i == 0) {
					t = dot_result[size - 1] / (dot_result[size - 1] - dot_result[i]);
					inter_point = ((1 - t) * store_vec[size - 1]) + (t * store_vec[i]);

					t2 = dot_result[i] / (dot_result[i] - dot_result[i + 1]);
					inter_point2 = ((1 - t2) * store_vec[i]) + (t2 * store_vec[i + 1]);

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
					inter_point = ((1 - t) * store_vec[i - 1]) + (t * store_vec[i]);

					t2 = dot_result[i] / (dot_result[i] - dot_result[0]);
					inter_point2 = ((1 - t2) * store_vec[i]) + (t2 * store_vec[0]);

					if (dot_result[i - 1] < 0) {
						temp_vertices.push_back(inter_point);
					}
					if (dot_result[0] < 0) {
						temp_vertices.push_back(inter_point2);
					}
				}
				else {
					t = dot_result[i - 1] / (dot_result[i - 1] - dot_result[i]);
					inter_point = ((1 - t) * store_vec[i - 1]) + (t * store_vec[i]);

					t2 = dot_result[i] / (dot_result[i] - dot_result[i + 1]);
					inter_point2 = ((1 - t2) * store_vec[i]) + (t2 * store_vec[i + 1]);

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
