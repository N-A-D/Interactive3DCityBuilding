#ifndef _CUBE_MESH_
#define _CUBE_MESH_

#include <stdbool.h>
#include "Vector3D.h"

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct CubeMesh
	{
		Vector3D center;
		Vector3D dim;
		float tx, ty, tz;		// Translatation Deltas
		float sfx, sfy, sfz;	// Scale Factors
		float angle;			// Angle around y-axis of cube coordinate system

		bool selected;

		// Material properties for drawing
		float mat_ambient[4];
		float mat_specular[4];
		float mat_diffuse[4];
		float mat_shininess[1];

		// Material properties if selected
		float highlightMat_ambient[4];
		float highlightMat_specular[4];
		float highlightMat_diffuse[4];
		float highlightMat_shininess[1];

	} CubeMesh;

#ifdef __cplusplus
}
#endif

#endif // !_CUBE_MESH_