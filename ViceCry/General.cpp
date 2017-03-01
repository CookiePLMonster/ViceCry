#include "StdAfx.h"
#include "General.h"

RwMatrix* RwMatrixUpdate(RwMatrix* matrix)
{
	matrix->flags &= 0xFFFDFFFC;
	return matrix;
}

void CMatrix::SetRotateOnly(float fAngleX, float fAngleY, float fAngleZ)
{
	matrix.right.x = cos(fAngleZ) * cos(fAngleY) - sin(fAngleZ) * sin(fAngleX) * sin(fAngleY);
	matrix.right.y = cos(fAngleZ) * sin(fAngleX) * sin(fAngleY) + sin(fAngleZ) * cos(fAngleY);
	matrix.right.z = -cos(fAngleX) * sin(fAngleY);

	matrix.up.x = -sin(fAngleZ) * cos(fAngleX);
	matrix.up.y = cos(fAngleZ) * cos(fAngleX);
	matrix.up.z = sin(fAngleX);

	matrix.at.x = sin(fAngleZ) * sin(fAngleX) * cos(fAngleY) + cos(fAngleZ) * sin(fAngleY);
	matrix.at.y = sin(fAngleZ) * sin(fAngleY) - cos(fAngleZ) * sin(fAngleX) * cos(fAngleY);
	matrix.at.z = cos(fAngleX) * cos(fAngleY);
}

void CMatrix::SetRotateXOnly(float fAngle)
{
	matrix.right.x = 1.0f;
	matrix.right.y = 0.0f;
	matrix.right.z = 0.0f;

	matrix.up.x = 0.0f;
	matrix.up.y = cos(fAngle);
	matrix.up.z = sin(fAngle);

	matrix.at.x = 0.0f;
	matrix.at.y = -sin(fAngle);
	matrix.at.z = cos(fAngle);
}

void CMatrix::SetRotateYOnly(float fAngle)
{
	matrix.right.x = cos(fAngle);
	matrix.right.y = 0.0f;
	matrix.right.z = sin(fAngle);

	matrix.up.x = 0.0f;
	matrix.up.y = 1.0f;
	matrix.up.z = 0.0f;

	matrix.at.x = -sin(fAngle);
	matrix.at.y = 0.0f;
	matrix.at.z = cos(fAngle);
}

void CMatrix::SetRotateZOnly(float fAngle)
{
	matrix.right.z = matrix.up.z = matrix.at.x = matrix.at.y = 0.0f;
	matrix.at.z = 1.0f;
	matrix.up.x = -sin(fAngle);
	matrix.up.y = cos(fAngle);
	matrix.right.x = cos(fAngle);
	matrix.right.y = sin(fAngle);
}

void CMatrix::UpdateRW()
{
	if ( matrixPtr )
	{
		matrixPtr->right.x = matrix.right.x;
		matrixPtr->right.y = matrix.right.y;
		matrixPtr->right.z = matrix.right.z;

		matrixPtr->up.x = matrix.up.x;
		matrixPtr->up.y = matrix.up.y;
		matrixPtr->up.z = matrix.up.z;

		matrixPtr->at.x = matrix.at.x;
		matrixPtr->at.y = matrix.at.y;
		matrixPtr->at.z = matrix.at.z;

		matrixPtr->pos.x = matrix.pos.x;
		matrixPtr->pos.y = matrix.pos.y;
		matrixPtr->pos.z = matrix.pos.z;

		RwMatrixUpdate(matrixPtr);
	}
}

// Wrapper
__declspec(naked) void CEntity::UpdateRwFrame()
{
	_asm mov	eax, 489360h
	_asm jmp	eax
}