#pragma once

#include <iostream>
#include <include/glm.h>

namespace Transform2D
{
	//matricea de translatie 2D
	inline glm::mat3 Translate(float translateX, float translateY)
	{
		return glm::transpose(glm::mat3(1, 0, translateX,
			0, 1, translateY,
			0, 0, 1));
	}

	//matricea de scalare in 2D
	inline glm::mat3 Scale(float scaleX, float scaleY)
	{
		return glm::transpose(glm::mat3(scaleX, 0, 0,
			0, scaleY, 0,
			0, 0, 1));
	}

	//matricea de rotatie in 2D
	inline glm::mat3 Rotate(float radians)
	{
		return glm::transpose(glm::mat3(cos(radians), -sin(radians), 0,
			sin(radians), cos(radians), 0,
			0, 0, 1));
	}

	//matricea de vizualizare (transformare poarta-fereastra)
	inline glm::mat3 Visualization(float viewWidth, float viewHeight, float logicWidth, float logicHeight)
	{
		float sx, sy, tx, ty, smin;
		sx = viewWidth / logicWidth;
		sy = viewHeight / logicHeight;
		if (sx < sy)
			smin = sx;
		else
			smin = sy;
		tx = (viewWidth - smin * logicWidth) / 2;
		ty = (viewHeight - smin * logicHeight) / 2;
		return glm::transpose(glm::mat3(
			smin, 0.0f, tx,
			0.0f, smin, ty,
			0.0f, 0.0f, 1.0f));
	}
}
