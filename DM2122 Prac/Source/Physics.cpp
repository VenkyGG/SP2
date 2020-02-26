#include "Physics.h"

struct Physics::Rectangle
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

bool Physics::IsIntersectingAABB_OBB(Rectangle r1, Rectangle r2)
{
	bool isIntersecting = false;

	//Find the min/max values for AABB
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
bool Physics::IsIntersectingAABB(float r1_minX, float r1_maxX, float r1_minZ, float r1_maxZ, float r2_minX, float r2_maxX, float r2_minZ, float r2_maxZ)
{
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
bool Physics::CheckCollision(Vector3 r1_FL, Vector3 r1_FR, Vector3 r1_BL, Vector3 r1_BR, Vector3 r2_FL, Vector3 r2_FR, Vector3 r2_BL, Vector3 r2_BR)
{
	bool isIntersecting = false;

	//Create two rectangles
	Rectangle r1 = Rectangle(r1_FL, r1_FR, r1_BL, r1_BR);
	Rectangle r2 = Rectangle(r2_FL, r2_FR, r2_BL, r2_BR);

	if (!IsIntersectingAABB_OBB(r1, r2))
	{
		return isIntersecting;
	}
	//Find out if the rectangles are intersecting by using the Separating Axis Theorem (SAT)
	isIntersecting = SATRectangleRectangle(r1, r2);

	return isIntersecting;
}

bool Physics::SATRectangleRectangle(Rectangle r1, Rectangle r2)
{
	bool isIntersecting = false;
	Vector3 normal1 = GetNormal(r1.BL, r1.FL);

	if (!IsOverlapping(normal1, r1, r2))
	{
		return isIntersecting;
	}
	Vector3 normal2 = GetNormal(r1.FL, r1.FR);
	if (!IsOverlapping(normal2, r1, r2))
	{
		return isIntersecting;
	}
	Vector3 normal3 = GetNormal(r2.BL, r2.FL);
	if (!IsOverlapping(normal3, r1, r2))
	{
		return isIntersecting;
	}
	Vector3 normal4 = GetNormal(r2.FL, r2.FR);
	if (!IsOverlapping(normal4, r1, r2))
	{
		return isIntersecting;
	}
	//all sides are overlapping so the rectangles are intersecting
	isIntersecting = true;

	return isIntersecting;
}

bool Physics::IsOverlapping(Vector3 normal, Rectangle r1, Rectangle r2)
{
	bool isOverlapping = false;

	//Project the four corners of 1st rectangle onto the normal
	float dot1 = DotProduct(normal, r1.FL);
	float dot2 = DotProduct(normal, r1.FR);
	float dot3 = DotProduct(normal, r1.BL);
	float dot4 = DotProduct(normal, r1.BR);

	//Finds rectangle 1's range
	float min1 = Math::Min(dot1, Math::Min(dot2, Math::Min(dot3, dot4)));
	float max1 = Math::Max(dot1, Math::Max(dot2, Math::Max(dot3, dot4)));

	//Project the four corners of 2nd rectangle onto the normal
	float dot5 = DotProduct(normal, r2.FL);
	float dot6 = DotProduct(normal, r2.FR);
	float dot7 = DotProduct(normal, r2.BL);
	float dot8 = DotProduct(normal, r2.BR);

	//Finds rectangle 2's range
	float min2 = Math::Min(dot5, Math::Min(dot6, Math::Min(dot7, dot8)));
	float max2 = Math::Max(dot5, Math::Max(dot6, Math::Max(dot7, dot8)));


	//Checks for overlap
	if (min1 <= max2 && min2 <= max1)
	{
		isOverlapping = true;
	}

	return isOverlapping;
}

Vector3 Physics::GetNormal(Vector3 startPos, Vector3 endPos)
{
	Vector3 dir = endPos - startPos;
	Vector3 normal = Vector3(-dir.z, dir.y, dir.x);
	return normal;
}

float Physics::DotProduct(Vector3 v1, Vector3 v2)
{
	float dotProduct = v1.x * v2.x + v1.z * v2.z;
	return dotProduct;
}