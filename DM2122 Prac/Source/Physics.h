#include <Vector3.h>
using namespace std;

class Physics
{
public:
	struct Rectangle;

	static bool SATRectangleRectangle(Rectangle r1, Rectangle r2);

	static bool IsOverlapping(Vector3 normal, Rectangle r1, Rectangle r2);
	
	static Vector3 GetNormal(Vector3 startPos, Vector3 endPos);

	static float DotProduct(Vector3 v1, Vector3 v2);

	static bool CheckCollision(Vector3 r1_FL, Vector3 r1_FR, Vector3 r1_BL, Vector3 r1_BR, Vector3 r2_FL, Vector3 r2_FR, Vector3 r2_BL, Vector3 r2_BR);

	static bool IsIntersectingAABB_OBB(Rectangle r1, Rectangle r2);

	//Intersection: AABB-AABB (Axis-Aligned Bounding Box) - rectangle-rectangle in 2d space with no orientation
	static bool IsIntersectingAABB(float r1_minX, float r1_maxX, float r1_minZ, float r1_maxZ, float r2_minX, float r2_maxX, float r2_minZ, float r2_maxZ);

	
};
