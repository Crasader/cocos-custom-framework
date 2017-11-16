#include "c2d.h"
#include "VisibleRect.h"
#include "StringUtil.h"
NS_CC_BEGIN

namespace c2d {

	Node* GetChildByName(Node* root, const std::string& widgetname)
	{

		if (!root || widgetname == "")
		{
			return nullptr;
		}
		for (auto node : root->getChildren())
		{
			const std::string nstr = node->getName();
			if (nstr != "" && nstr == widgetname)
			{
				return node;
			}
			auto childnode = GetChildByName(node, widgetname);
			if (childnode)
			{
				return childnode;
			}

		}
		return nullptr;
	}

	Node* GetChildByPath(Node* root, const std::string& path)
	{

		std::vector<std::string> vec = StringUtil::split(path, "/");

		Node* node = nullptr;

		for (auto str : vec)
		{
			if (node == nullptr){
				node = root->getChildByName(str);
				if (node == nullptr)break;
			}
			else{
				node = node->getChildByName(str);
				if (node == nullptr)break;
			}
		}

		return node;
	}
	bool adaptScreen(Node * node, c2d::Align align)
	{
		Vec2 point = node->getPosition();
		Size size = node->getBoundingBox().size;
		auto _ResolutionPolicy = Director::getInstance()->getOpenGLView()->getResolutionPolicy();
		if (_ResolutionPolicy == ResolutionPolicy::NO_BORDER){
			Size designResolutionSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	
			switch (align)
			{
			case Align::left:
				node->setPosition(Vec2(VisibleRect::left().x + point.x, point.y));
				break;
			case Align::right:
				node->setPosition(Vec2(VisibleRect::right().x - (designResolutionSize.width - point.x), point.y));
				break;
			case Align::top:
				node->setPosition(Vec2(point.x, VisibleRect::top().y - (designResolutionSize.height - point.y)));
				break;
			case Align::bottom:
				node->setPosition(Vec2(point.x, VisibleRect::bottom().y + point.y));
				break;
			case Align::center:
				break;
		
			default:
				break;
			}
		}
		else if (_ResolutionPolicy == ResolutionPolicy::FIXED_HEIGHT){
			auto _designSize = Size(1536, 768);
			auto _screenSize = Director::getInstance()->getVisibleSize();//显示分辨率
			auto _screenLeft = (_designSize - _screenSize) / 2;
			switch (align)
			{
			case Align::left:
				node->setPosition(Vec2(_screenLeft.width + point.x, point.y));
				break;
			case Align::right:
				node->setPosition(Vec2(_screenLeft.width + _screenSize.width - (_designSize.width - point.x), point.y));
				break;
			default:
				break;
			}
		}
	
		return false;
	}
	
	bool adaptScreen(Node * node)
	{
		auto _designSize = Size(1536, 768);
		auto pos = node->getPosition();
		if (pos.x < _designSize.width / 2){
			return adaptScreen(node,Align::left);
		}
		else if (pos.x > _designSize.width / 2){
			return adaptScreen(node, Align::right);
		}
	
		return false;
	}
	
	void adaptScene(Node* sceneLayer)
	{
		auto _ResolutionPolicy = Director::getInstance()->getOpenGLView()->getResolutionPolicy();
		if (_ResolutionPolicy == ResolutionPolicy::FIXED_HEIGHT){
			auto _designSize = Size(1536, 768);
			auto _winSize = Director::getInstance()->getWinSize();//设计分辨率
			auto _screenSize = Director::getInstance()->getVisibleSize();//显示分辨率
			auto _machineSize = Director::getInstance()->getOpenGLView()->getFrameSize();//设备分辨率
	
			auto pos = (_screenSize - _designSize) / 2;
	
			sceneLayer->setPosition(pos);
			CCLOG("");
		}
		else{
			CCLOG("adaptScene -> Nothing had happened");
		}
		
	
	}

	void adaptScreen(Node* root, const std::string& childPath, c2d::Align align /*= c2d::Align::center*/)
	{
		Node* node = GetChildByPath(root, childPath);
		if (align == c2d::Align::center){
			adaptScreen(node);
		}
		else{
			adaptScreen(node, align);
		}
	}

	void adaptScreenChildren(Node*root, const std::string& rootPath, c2d::Align align /*= c2d::Align::center*/)
	{
		Node* node = GetChildByPath(root, rootPath);
		for (auto child : node->getChildren())
		{
			if (align == c2d::Align::center){
				adaptScreen(child);
			}
			else{
				adaptScreen(child, align);
			}
		}
	}

	void touchMove(Widget * node)
	{
		node->setTouchEnabled(true);
		node->addTouchEventListener([=](Ref*sender, Widget::TouchEventType type)
		{
			Widget *widget = static_cast<Widget*>(sender);
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
			{
			}
				break;
	
			case Widget::TouchEventType::MOVED:
			{
												  Vec2 npoint = widget->getPosition();
												  //CCLOG("Widget::TouchEventType::MOVED npoint.x = %f, npoint.y = %f ", npoint.x, npoint.y);
												  CCLOG("touch move tag = %d", widget->getTag());
												  CCLOG("getTouchMovePosition  x = %f ,y = %f", widget->getTouchMovePosition().x, widget->getTouchMovePosition().y);
												  widget->setPosition(widget->getTouchMovePosition());
			}
				break;
	
			case Widget::TouchEventType::ENDED:
			{
	
			}
				break;
	
			case Widget::TouchEventType::CANCELED:
				break;
	
			default:
				break;
			}
		});
	}
	
	bool intersection(Node* node1,Node*node2){
		Point node1Position = getWorldPoint(node1);
		Point node2Position = getWorldPoint(node2);
	
		auto node2ContentSize = node2->getContentSize();
		auto node2AnchorPoint = node2->getAnchorPoint();
	
		Point node2Center = Point(node2Position.x - node2ContentSize.width* node2AnchorPoint.x + node2ContentSize.width*0.5,
			node2Position.y - node2ContentSize.height* node2AnchorPoint.y + node2ContentSize.height*0.5);
		
		Rect bound = node1->getBoundingBox();
	
		return bound.containsPoint(node2Center);
	
	}
	
	void setCameraMask(Node* root, Node* node)
	{
		node->setCameraMask(root->getCameraMask());
	}
	
	DrawNode* getDrawNode(Rect rect, Color4F color /*= Color4F(0, 1, 0, 1)*/)
	{
	
		DrawNode* drawNode = DrawNode::create();
		Vec2 point[4];
		Rect nrect = rect;
		point[0] = Vec2(0, 0);
		point[1] = Vec2(0, nrect.size.height);
		point[2] = Vec2(nrect.size.width, nrect.size.height);
		point[3] = Vec2(nrect.size.width, 0);
		drawNode->drawPolygon(point, 4, Color4F(1, 0, 0, 0), 1, color);
		return drawNode;
	}
	
	void drawDebug(Node* root, Color4F color /*= Color4F(0, 1, 0, 1)*/){
	
		DrawNode* drawNode = static_cast<DrawNode*>(root->getChildByName("DebugDrawNode~"));
		if (drawNode){
	
		}
		else{
			drawNode = DrawNode::create();
			drawNode->setName("DebugDrawNode~");
			drawNode->setCameraMask(root->getCameraMask());
			root->addChild(drawNode);
		}
		Vec2 point[4];
		Rect nrect = root->getBoundingBox();
		point[0] = Vec2(0, 0);
		point[1] = Vec2(0, nrect.size.height);
		point[2] = Vec2(nrect.size.width, nrect.size.height);
		point[3] = Vec2(nrect.size.width, 0);
		//drawNode->drawPolygon(point, 4, Color4F(1, 0, 0, 0), 1, Color4F(0, 1, 0, 1));
		drawNode->drawPolygon(point, 4, Color4F(1, 0, 0, 0), 1, color);
	
	}
	
	void drawDebugAll(Node* root, Color4F color /*= Color4F(0, 1, 0, 1)*/){
	
			if (root->getChildrenCount() > 0){
				for (Node* node : root->getChildren())
				{
					drawDebugAll(node, color);
				}
			}
	
			drawDebug(root, color);
	}
	
	void drawDebugSpine(Node* root, Color4F color /*= Color4F(0, 1, 0, 1)*/)
	{
		Vec2 position = root->getPosition();
		Rect box = root->getBoundingBox();
		float _minx = box.origin.x - position.x;
		float _miny = box.origin.y - position.y;
		float _maxx = box.size.width + _minx;
		float _maxy = box.size.height + _miny;
	
		DrawNode* drawNode = static_cast<DrawNode*>(root->getChildByName("DebugDrawNode~"));
		if (drawNode){
		
		}
		else{
			drawNode = DrawNode::create();
			drawNode->setName("DebugDrawNode~");
			drawNode->setCameraMask(root->getCameraMask());
			root->addChild(drawNode);
		}
	
		Vec2 point[4];
		Rect nrect = box;
		point[0] = Vec2(_minx, _miny);
		point[1] = Vec2(_minx, _maxy);
		point[2] = Vec2(_maxx,_maxy);
		point[3] = Vec2(_maxx, _miny);
		//drawNode->drawPolygon(point, 4, Color4F(1, 0, 0, 0), 1, Color4F(0, 1, 0, 1));
		drawNode->drawPolygon(point, 4, Color4F(1, 0, 0, 0), 1, color);
	}
	
	//判断vector的某一元素是否存在
	bool is_element_in_vector(vector<int> v, int element){
		vector<int>::iterator it;
		it = find(v.begin(), v.end(), element);
		if (it != v.end()){
			return true;
		}
		else{
			return false;
		}
	}
	
	void setSafeAnchor(Node* node, float anchorX, float anchorY)
	{
		auto cacheAR = node->getAnchorPoint();
		auto diffX = (anchorX - cacheAR.x) * node->getContentSize().width  * (node->getScaleX());
		auto diffY = (anchorY - cacheAR.y) * node->getContentSize().height * (node->getScaleY());
	
		node->setAnchorPoint(Vec2(anchorX, anchorY));
		node->setPositionX(node->getPositionX() + diffX);
		node->setPositionY(node->getPositionY() + diffY);
	}
	
	cocos2d::Vec2 GetDistance(Vec2 npoint1, Vec2 npoint2)
	{
		return Vec2(npoint2.x - npoint1.x, npoint2.y - npoint1.y);
	}
	
	void SetRandomSeed()
	{
		struct timeval psv;
		gettimeofday(&psv, NULL);
		unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;
		srand(rand_seed);
	}
	
	
	int getRand(int start, int end)
	{
		float i = CCRANDOM_0_1()*((end + 1) - start) + start;  //产生一个从start到end间的随机数  
		int value = (int)i;
	
		if (value > end){
			value = end;
		}
		return value;
	}
	
	
	float getRandFloat(float start, float end)
	{
		int value = getRand(start * 1000, end * 1000);
		float relut = value / 1000.f;
		return relut;
	}
	
	bool getRandomBool()
	{
		float i = CCRANDOM_0_1();
		if (i > 0.5){
	
			return true;
		}
		return false;
	}
	
	Rect getWorldBoundingBox(Node *node)
	{
		Rect nrect = node->getBoundingBox();
		nrect.origin = node->getParent()->convertToWorldSpace(nrect.origin);
		return nrect;
	}
	Vec2 getWorldPoint(Node * node)
	{
		return node->getParent()->convertToWorldSpace(node->getPosition());
	}
	Vec2 getInNodePoint(Node *ncurparentnode, Vec2 npoint, Node* ndesnode)
	{
		Vec2 nworldpoint = ncurparentnode->convertToWorldSpace(npoint);
		return ndesnode->convertToNodeSpace(nworldpoint);
	}
	
	Point getRectCenter(Rect rect)
	{
		Point center;
		center.setPoint(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2);
		return center;
	}
	
	
	long getCurrentTime()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	
	
	RenderTexture* createRenderTexture(Size size)
	{
		RenderTexture*  pRenderTexture = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
		pRenderTexture->setContentSize(size);
		pRenderTexture->setAnchorPoint(Vec2(0.f, 0.f));
		pRenderTexture->setPosition(size / 2);
		pRenderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();//开启抗锯齿
		return pRenderTexture;
	}
	
	Image* createImageFromSprite(Sprite *rSprite)
	{
		//记录状态
		auto cahceAnchor = rSprite->getAnchorPoint();//记录锚点
		auto cachePos = rSprite->getPosition();
		auto size = rSprite->getContentSize();
		rSprite->setAnchorPoint(Vec2(.0f, .0f));//设置锚点为左下
		rSprite->setPosition(Vec2(0, 0));
		//记录状态 ------- end
	
		//定义一个截图框大小的渲染纹理  
		RenderTexture* pScreen = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
		pScreen->setPosition(Vec2(0, 0));
		pScreen->setAnchorPoint(Point(0, 0));
		pScreen->setContentSize(size);
		pScreen->setIgnoreAnchorPointForPosition(true);
	
		Sprite* pTempSpr = Sprite::createWithTexture(rSprite->getTexture());
		pTempSpr->setPosition(pTempSpr->getContentSize() / 2);
	
		//渲染纹理开始捕捉  
		pScreen->begin();
		//当前场景参与绘制  
		pTempSpr->visit();
		//结束捕捉  
		pScreen->end();
		Director::getInstance()->getRenderer()->render();
		//恢复状态
		rSprite->setAnchorPoint(cahceAnchor);//恢复锚点
		rSprite->setPosition(cachePos);
	
		return pScreen->newImage(true);
	}
	
	Sprite* createSprite(Sprite* rModel, Sprite* fore)
	{
		auto modelSize = rModel->getContentSize();
		auto foreSize = fore->getContentSize();
		auto scale = MAX(modelSize.width / foreSize.width, modelSize.height / foreSize.height);
		fore->setScale(scale);
	// 	fore->setScaleX(modelSize.width / foreSize.width);
	// 	fore->setScaleY(modelSize.height / foreSize.height);
		fore->setAnchorPoint(Vec2(.0f, .0f));//设置锚点为左下
		fore->setPosition(Vec2(0, 0));
		BlendFunc blendFunc = { GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA };
		fore->setBlendFunc(blendFunc);
	
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
		fore->visit();
		//结束捕捉  
		pScreen->end();
		pScreen->getSprite()->getTexture()->setAntiAliasTexParameters();
		//保存为png
		Director::getInstance()->getRenderer()->render();
	
		//恢复状态
		rModel->setAnchorPoint(cahceAnchor);//恢复锚点
		rModel->setPosition(cachePos);
	
	//   	auto newSpr = Sprite::createWithTexture(pScreen->getSprite()->getTexture());
	//   	newSpr->setFlipY(true);
		
		//如果直接给 其他 Sprite 替换 Texture //需要执行下面的函数
		//other->setTexture(newSpr->getTexture());
		//other->setFlipY(true);
		//
		auto name = pScreen->getSprite()->getTexture()->getPath();
		
	 	auto im = pScreen->newImage();
		Texture2D* texture = new Texture2D();
		texture->initWithImage(im);
	
	// 	Director::getInstance()->getTextureCache()->removeTextureForKey(name);
	//  	auto texture = Director::getInstance()->getTextureCache()->addImage(im, name);
	 	Sprite *newSpr = Sprite::createWithTexture(texture);
	
		//saveFinishImage();
	
		//delete im;
	
	
		return newSpr;
	}
	
	cocos2d::Color4B getColor4B(Image* rImage, Point rPoint)
	{
		return getColor4B(rImage, (int)rPoint.x, (int)rPoint.y);
	}
	
	cocos2d::Color4B getColor4B(Image* rImage, int x, int y)
	{
		Color4B color = { 0, 0, 0, 0 };
	
		CCLOG("the pos is -> [%d,%d]", x, y);
		auto imgWidth = (int)rImage->getWidth();
		auto imgHight = (int)rImage->getHeight();
		//获取像素数据
		unsigned char* data_ = rImage->getData();
		unsigned int *pixel = (unsigned int *)data_;
		pixel = pixel + ((imgHight - y) * imgWidth) * 1 + x * 1;
		//R通道
		color.r = *pixel & 0xff;
		//G通道
		color.g = (*pixel >> 8) & 0xff;
		//B通过
		color.b = (*pixel >> 16) & 0xff;
		//Alpha通道，我们有用的就是Alpha
		color.a = (*pixel >> 24) & 0xff;
	
		CCLOG("pos [%d,%d]the rgba -> [%d,%d,%d,%d]", color.r, color.g, color.b, color.a);
	
	
	
		return color;
	}
	
	bool isRenderTextureClearInRect(Image* image, Rect rext)
	{
		Color4B color = { 0, 0, 0, 0 };
	
		int x = rext.origin.x;
		int y = rext.origin.y;
		int width = rext.size.width;
		int height = rext.size.height;
		if (width == 0 || height == 0)return false;
	
	
		bool m_bEraserOk = false;
	
	
		int count = 0;
		int i = 0, j = 0;
		unsigned char* data_ = image->getData();
	
		auto imgWidth = (int)image->getWidth();
		auto imgHight = (int)image->getHeight();
		for (i = 0; i < width; ++i)
		{
			for (j = 0; j < height; ++j)
			{
				//获取像素数据
				unsigned int *pixel = (unsigned int *)data_;
				pixel = pixel + ((imgHight - y - 1 - j) * imgWidth) * 1 + i * 1 + x;
	
				color.r = *pixel & 0xff;
				color.g = (*pixel >> 8) & 0xff;
				color.b = (*pixel >> 16) & 0xff;
				color.a = (*pixel >> 24) & 0xff;
	
				//CCLOG("isDataClearInRect pos[%d,%d] the rgba -> [%d,%d,%d,%d]",i+x,j+y, color.r, color.g, color.b, color.a);
				count++;
	
				if (color.r > 5 && color.g > 5 && color.b > 5 && color.a > 5){
					//CCLOG("i have a pen~~~~");
					break;
				}
			}
	
			if (height != j)
			{
				break;
			}
		}
		//CCLOG("the count is -> %d", count);
		if (i == width && j == height)
		{
			//CCLOG("i have a apple~~~~");
			//CCLOG("the m_bEraserOk ");
			m_bEraserOk = true;
		}
	
		return m_bEraserOk;
	}
	
	bool isRenderTextureClear(Image* image)
	{
	
		Color4B color = { 0, 0, 0, 0 };
	
		auto width = (int)image->getWidth();
		auto height = (int)image->getHeight();
		if (width == 0 || height == 0)return false;
	
	
		bool m_bEraserOk = false;
	
		int count = 0;
		int i = 0, j = 0;
		//unsigned char* mdata = (unsigned char*)image->getData();
		unsigned char* data_ = image->getData();
	
		for (i = 0; i < width; ++i)
		{
			for (j = 0; j < height; ++j)
			{
				//获取像素数据
				unsigned int *pixel = (unsigned int *)data_;
				pixel = pixel + ((height - j - 1) * width) * 1 + i * 1;
	
				color.r = *pixel & 0xff;
				color.g = (*pixel >> 8) & 0xff;
				color.b = (*pixel >> 16) & 0xff;
				color.a = (*pixel >> 24) & 0xff;
	
				//CCLOG("isDataClearInRect pos[%d,%d] the rgba -> [%d,%d,%d,%d]", i, j, color.r, color.g, color.b, color.a);
				count++;
			}
	
			if (height != j)
			{
				break;
			}
		}
		CCLOG("the count is -> %d", count);
		if (i == width && j == height)
		{
			m_bEraserOk = true;
		}
	
		return m_bEraserOk;
	}
	
	#include "mybyte.h"
	static std::map<Node*, std::string> nodePngMap;
	bool saveNodeAsPng(Node* node, std::function<void(RenderTexture*, const std::string&)> callback)
	{
		bool save = false;
		try
		{
			auto nodename = node->getName();
			if (nodename == ""){
				if (nodePngMap.find(node) != nodePngMap.end()){
					nodename = nodePngMap.at(node);
				}
				else{
					nodename = StringUtils::format("pic_%d", getRand(0, 9999999));
					nodePngMap[node] = nodename;
				}
			}
	
			auto filename = nodename + ".png";
	
			//记录状态
			auto cahceAnchor = node->getAnchorPoint();//记录锚点
			auto cachePos = node->getPosition();
			auto size = node->getContentSize();
			node->setAnchorPoint(Vec2(.0f, .0f));//设置锚点为左下
			node->setPosition(Vec2(0, 0));
			//记录状态 ------- end
	
			//定义一个截图框大小的渲染纹理  
			RenderTexture* pScreen = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
			pScreen->setContentSize(size);
			//渲染纹理开始捕捉  
			pScreen->begin();
			//当前场景参与绘制  
			node->visit();
			//结束捕捉  
			pScreen->end();
			pScreen->getSprite()->getTexture()->setAntiAliasTexParameters();
			//保存为png
			save = pScreen->saveToFile(filename, Image::Format::PNG, true, callback);
			Director::getInstance()->getRenderer()->render();
	
	
			// 		char PNG_SIGNATURE[] = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
			// 		std::string fullpath = FileUtils::getInstance()->getWritablePath() + nodename + "999.png";
			// 		auto image = pScreen->newImage(true);
			// 	
			// 				
			// 		mybyte _mybyte;
			// 		_mybyte.assign(PNG_SIGNATURE, strlen(PNG_SIGNATURE));
			// 
			// 		unsigned char* s = image->getData();
			// 		
			// 		_mybyte.add((char*)image->getData(), image->getDataLen());
	
			// 
			// 		Data data;
			// 		data.fastSet((unsigned char*)_mybyte.data(), _mybyte.getlen());
			// 		FileUtils::getInstance()->writeDataToFile(data, fullpath);
	
			//恢复状态
			node->setAnchorPoint(cahceAnchor);//恢复锚点
			node->setPosition(cachePos);
	
	
			CCLOG("end -- the path is -> %s the file name is -> %s", FileUtils::getInstance()->getWritablePath().c_str(), filename.c_str());
		}
		catch (...)
		{
	
		}
		return save;
	}
	
	std::vector<Rect> splitRect(Size size, int rows, int cols)
	{
		
		CCASSERT(rows > 0, "C2DUtils splitRect The rows should not < 0");
		CCASSERT(cols > 0, "C2DUtils splitRect The cols should not < 0");
		std::vector<Rect> vec;
		int x = 0;
		int y = 0;
		int tileWidth = size.width / cols;
		int tileHeight = size.height / rows;
	
		int startX = x;
		for (int row = 0; row < rows; row++, y += tileHeight) {
			x = startX;
			for (int col = 0; col < cols; col++, x += tileWidth) {
				Rect temRect = Rect(x, y, tileWidth, tileHeight);
				vec.push_back(temRect);
	
				CCLOG("C2dUtils  the rect -> [%d,%d,%d,%d]", x, y, tileWidth, tileHeight);
				//tiles[row][col] = new TextureRegion(texture, x, y, tileWidth, tileHeight);
			}
		}
		return vec;
	}
	
	bool safeReleaseRef(Ref *pRef)
	{
		CCLOG("the node ReferenceCount is -> [ %d ] ", pRef->getReferenceCount());
		if (pRef->getReferenceCount() > 0){
			pRef->release();
			return true;
		}
		else{
			auto noed = static_cast<Node*>(pRef);
			CCLOG("the node -> [ %s ]is be release ", noed->getName().c_str());
		}
		return false;
	}
	
	float getDurationFromSpeed(Node* self, Node* target, float speed)
	{
		auto duration = getDurationFromSpeed(self->getPosition(), target->getPosition(),speed);
		return duration;
	}
	
	float getDurationFromSpeed(const Point& self, const Point& target, float speed)
	{
		auto MoveDistance = target.distance(self);
		auto duration = MoveDistance / speed;
		return duration;
	}
	


} // namespace StringUtils {


NS_CC_END