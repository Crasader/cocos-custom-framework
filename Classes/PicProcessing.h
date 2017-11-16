#ifndef __PICPROCESSING_H__
#define __PICPROCESSING_H__

#include "PublicCoco.h"

namespace PicProcessing
{
	CC_DEPRECATED_ATTRIBUTE Texture2D* flushTexture(Node* node);

	Image* captureNodeGetImage(Node* startNode,float scale = 1);

	Texture2D* captureNodeGetTexture(Node* startNode, float scale = 1);

};

#endif /* defined(__PICPROCESSING_H__) */
