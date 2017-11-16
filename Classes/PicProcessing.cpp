#include "PicProcessing.h"

namespace PicProcessing{

	Texture2D* flushTexture(Node* rModel)
	{
		auto modelSize = rModel->getContentSize();
			//记录状态
			auto cahceAnchor = rModel->getAnchorPoint();//记录锚点
			auto cachePos = rModel->getPosition();
			auto size = rModel->getContentSize();
			rModel->setAnchorPoint(Vec2(.0f, .0f));//设置锚点为左下
			rModel->setPosition(Vec2(0, 0));
			//记录状态 ------- end
		
			//定义一个截图框大小的渲染纹理  
			RenderTexture* pScreen = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
			pScreen->setContentSize(size);
			//渲染纹理开始捕捉  
			pScreen->begin();
			//当前场景参与绘制  
			rModel->visit();
			//结束捕捉  
			pScreen->end();
			pScreen->getSprite()->getTexture()->setAntiAliasTexParameters();
			//保存为png
			Director::getInstance()->getRenderer()->render();
		
			//恢复状态
			rModel->setAnchorPoint(cahceAnchor);//恢复锚点
			rModel->setPosition(cachePos);
		
			
			//如果直接给 其他 Sprite 替换 Texture //需要执行下面的函数
			//other->setTexture(newSpr->getTexture());
			//other->setFlipY(true);
			//
			auto name = pScreen->getSprite()->getTexture()->getPath();
			
		 	auto im = pScreen->newImage();
			Texture2D* texture = new Texture2D();
			texture->initWithImage(im);
		
		// 	Director::getInstance()->getTextureCache()->removeTextureForKey(name);
		//  auto texture = Director::getInstance()->getTextureCache()->addImage(im, name);
			delete im;
			return texture;
	}

	Image* captureNodeGetImage(Node* startNode, float scale /*= 1*/)
	{
		auto& size = startNode->getContentSize();

		Director::getInstance()->setNextDeltaTimeZero(true);

		RenderTexture* finalRtx = nullptr;

		auto rtx = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
		// rtx->setKeepMatrix(true);
		Point savedPos = startNode->getPosition();
		Point anchor;
		if (!startNode->isIgnoreAnchorPointForPosition()) {
			anchor = startNode->getAnchorPoint();
		}
		startNode->setPosition(Point(size.width * anchor.x, size.height * anchor.y));
		rtx->begin();
		startNode->visit();
		rtx->end();
		startNode->setPosition(savedPos);

		if (std::abs(scale - 1.0f) < 1e-6f/* no scale */)
			finalRtx = rtx;
		else {
			/* scale */
			auto finalRect = Rect(0, 0, size.width, size.height);
			Sprite *sprite = Sprite::createWithTexture(rtx->getSprite()->getTexture(), finalRect);
			sprite->setAnchorPoint(Point(0, 0));
			sprite->setFlippedY(true);

			finalRtx = RenderTexture::create(size.width * scale, size.height * scale, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);

			sprite->setScale(scale); // or use finalRtx->setKeepMatrix(true);
			finalRtx->begin();
			sprite->visit();
			finalRtx->end();
		}

		Director::getInstance()->getRenderer()->render();

		return finalRtx->newImage();
	}

	Texture2D* captureNodeGetTexture(Node* startNode, float scale /*= 1*/)
	{
		Image* image = captureNodeGetImage(startNode,scale);
		Texture2D* texture = new Texture2D();
		texture->initWithImage(image);
		delete image;
		return texture;
	}


}
