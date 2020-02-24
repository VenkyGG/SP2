#include <Vector3.h>
using namespace std;

class Physics
{
public:
	struct Rectangle
	{
		//Corners
		Vector3 FL, FR, BL, BR;

		Rectangle(Vector3 FL, Vector3 FR, Vector3 BL, Vector3 BR)
		{
			this->FL = FL;
			this->FR = FR;
			this->BL = BL;
			this->BR = BR;
		}
	};
	static bool IsIntersectingOBBRectangleRectangle(
		Vector3 r1_FL, Vector3 r1_FR, Vector3 r1_BL, Vector3 r1_BR,
		Vector3 r2_FL, Vector3 r2_FR, Vector3 r2_BL, Vector3 r2_BR)
	{
		bool isIntersecting = false;

		//Create the rectangles
		Rectangle r1 = Rectangle(r1_FL, r1_FR, r1_BL, r1_BR);
		Rectangle r2 = Rectangle(r2_FL, r2_FR, r2_BL, r2_BR);

		//Find out if the rectangles are intersecting by approximating them with rectangles 
		//with no rotation and then use AABB intersection
		//Will make it faster if the probability that the rectangles are intersecting is low
		if (!IsIntersectingAABB_OBB(r1, r2))
		{
			return isIntersecting;
		}

		//Find out if the rectangles are intersecting by using the Separating Axis Theorem (SAT)
		isIntersecting = SATRectangleRectangle(r1, r2);

		return isIntersecting;
	}

	//Find out if the rectangles are intersecting by using AABB
	static bool IsIntersectingAABB_OBB(Rectangle r1, Rectangle r2)
	{
		bool isIntersecting = false;

		//Find the min/max values for the AABB algorithm
		float r1_minX = Math::Min(r1.FL.x, Math::Min(r1.FR.x, Math::Min(r1.BL.x, r1.BR.x)));
		float r1_maxX = Math::Max(r1.FL.x, Math::Max(r1.FR.x, Math::Max(r1.BL.x, r1.BR.x)));

		float r2_minX = Math::Min(r2.FL.x, Math::Min(r2.FR.x, Math::Min(r2.BL.x, r2.BR.x)));
		float r2_maxX = Math::Max(r2.FL.x, Math::Max(r2.FR.x, Math::Max(r2.BL.x, r2.BR.x)));

		float r1_minZ = Math::Min(r1.FL.z, Math::Min(r1.FR.z, Math::Min(r1.BL.z, r1.BR.z)));
		float r1_maxZ = Math::Max(r1.FL.z, Math::Max(r1.FR.z, Math::Max(r1.BL.z, r1.BR.z)));

		float r2_minZ = Math::Min(r2.FL.z, Math::Min(r2.FR.z, Math::Min(r2.BL.z, r2.BR.z)));
		float r2_maxZ = Math::Max(r2.FL.z, Math::Max(r2.FR.z, Math::Max(r2.BL.z, r2.BR.z)));

		if (IsIntersectingAABB(r1_minX, r1_maxX, r1_minZ, r1_maxZ, r2_minX, r2_maxX, r2_minZ, r2_maxZ))
		{
			isIntersecting = true;
		}

		return isIntersecting;
	}

	//Intersection: AABB-AABB (Axis-Aligned Bounding Box) - rectangle-rectangle in 2d space with no orientation
	static bool IsIntersectingAABB(
		float r1_minX, float r1_maxX, float r1_minZ, float r1_maxZ,
		float r2_minX, float r2_maxX, float r2_minZ, float r2_maxZ)
	{
		//If the min of one box in one dimension is greater than the max of another box then the boxes are not intersecting
		//They have to intersect in 2 dimensions. We have to test if box 1 is to the left or box 2 and vice versa
		bool isIntersecting = true;

		//X axis
		if (r1_minX > r2_maxX)
		{
			isIntersecting = false;
		}
		else if (r2_minX > r1_maxX)
		{
			isIntersecting = false;
		}
		//Z axis
		else if (r1_minZ > r2_maxZ)
		{
			isIntersecting = false;
		}
		else if (r2_minZ > r1_maxZ)
		{
			isIntersecting = false;
		}


		return isIntersecting;
	}

	//Find out if 2 rectangles with orientation are intersecting by using the SAT algorithm
	static bool SATRectangleRectangle(Rectangle r1, Rectangle r2)
	{
		bool isIntersecting = false;

		//We have just 4 normals because the other 4 normals are the same but in another direction
		//So we only need a maximum of 4 tests if we have rectangles
		//It is enough if one side is not overlapping, if so we know the rectangles are not intersecting

		//Test 1
		Vector3 normal1 = GetNormal(r1.BL, r1.FL);

		if (!IsOverlapping(normal1, r1, r2))
		{
			//No intersection is possible!
			return isIntersecting;
		}

		//Test 2
		Vector3 normal2 = GetNormal(r1.FL, r1.FR);

		if (!IsOverlapping(normal2, r1, r2))
		{
			return isIntersecting;
		}

		//Test 3
		Vector3 normal3 = GetNormal(r2.BL, r2.FL);

		if (!IsOverlapping(normal3, r1, r2))
		{
			return isIntersecting;
		}

		//Test 4
		Vector3 normal4 = GetNormal(r2.FL, r2.FR);

		if (!IsOverlapping(normal4, r1, r2))
		{
			return isIntersecting;
		}

		//If we have come this far, then we know all sides are overlapping
		//So the rectangles are intersecting!
		isIntersecting = true;

		return isIntersecting;
	}

	//Is this side overlapping?
	static bool IsOverlapping(Vector3 normal, Rectangle r1, Rectangle r2)
	{
		bool isOverlapping = false;

		//Project the corners of rectangle 1 onto the normal
		float dot1 = DotProduct(normal, r1.FL);
		float dot2 = DotProduct(normal, r1.FR);
		float dot3 = DotProduct(normal, r1.BL);
		float dot4 = DotProduct(normal, r1.BR);

		//Find the range
		float min1 = Math::Min(dot1, Math::Min(dot2, Math::Min(dot3, dot4)));
		float max1 = Math::Max(dot1, Math::Max(dot2, Math::Max(dot3, dot4)));


		//Project the corners of rectangle 2 onto the normal
		float dot5 = DotProduct(normal, r2.FL);
		float dot6 = DotProduct(normal, r2.FR);
		float dot7 = DotProduct(normal, r2.BL);
		float dot8 = DotProduct(normal, r2.BR);

		//Find the range
		float min2 = Math::Min(dot5, Math::Min(dot6, Math::Min(dot7, dot8)));
		float max2 = Math::Max(dot5, Math::Max(dot6, Math::Max(dot7, dot8)));


		//Are the ranges overlapping?
		if (min1 <= max2 && min2 <= max1)
		{
			isOverlapping = true;
		}

		return isOverlapping;
	}

	//Get the normal from 2 points. This normal is pointing left in the direction start -> end
	//But it doesn't matter in which direction the normal is pointing as long as you have the same
	//algorithm for all edges
	static Vector3 GetNormal(Vector3 startPos, Vector3 endPos)
	{
		//The direction
		Vector3 dir = endPos - startPos;

		//The normal, just flip x and z and make one negative (don't need to normalize it)
		Vector3 normal = Vector3(-dir.z, dir.y, dir.x);

		//Draw the normal from the center of the rectangle's side
		

		return normal;
	}

	//Get the dot product
	//p - the vector we want to project
	//u - the unit vector p is being projected on
	//proj_p_on_u = Vector3.Dot(p, u) * u;
	//But we only need to project a point, so just Vector3.Dot(p, u)
	static float DotProduct(Vector3 v1, Vector3 v2)
	{
		//2d space
		float dotProduct = v1.x * v2.x + v1.z * v2.z;

		return dotProduct;
	}

	//To create a rectangle
	
};
