/*!************************************************************************
\file Projection.cpp
\author Seungheon Jeon
\par DP email: seungheon.jeon\@digipen.edu
\par Course: CS200/fall
\par Transform Projections Programming Assignment
\date 11-17-2023
\brief
Implement function about Projection
**************************************************************************/
#include "Projection.h"

/*!*****************************************************************************
\brief
    Make Camera To World affine
\param cam
    camera object
*******************************************************************************/
Affine CameraToWorld(const Camera& cam)
{
    Affine result(cam.Right(), cam.Up(), cam.Back(), cam.Eye());
    return result;
}

/*!*****************************************************************************
\brief
    Make World To Camera affine
\param cam
    camera object
*******************************************************************************/
Affine WorldToCamera(const Camera& cam)
{
    return inverse(CameraToWorld(cam));
}

/*!*****************************************************************************
\brief
    Make Camera To NDC matrix
\param cam
    camera object
*******************************************************************************/
Matrix CameraToNDC(const Camera& cam)
{
    Vector resource = cam.ViewportGeometry();
    float near = cam.NearDistance();
    float far = cam.FarDistance();

    Matrix result;
    result[0][0] = 2 * resource.z / resource.x;
    result[1][1] = 2 * resource.z / resource.y;
    result[2][2] = (near + far) / (near - far);
    result[2][3] = 2 * near * far / (near - far);
    result[3][2] = -1;

    return result;
}
