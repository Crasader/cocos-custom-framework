
#include "UiUtils.h"
#include "PublicDefine.h"
#include "PublicHead.h"
#include "UIFactory.h"
#include "AudioManager.h"

vector<std::string> UiUtils::dialogStack;

ParticleSystemQuad* UiUtils::createParticle(const std::string& filename){
	return ParticleSystemQuad::create(filename);
}

Animation * UiUtils::createAnimation(const std::string&img, int rows /*= 1*/, int cols /*= 1*/, float delay /*= 1.0F*/, unsigned int loops /*= 1*/)
{

	//创建2D纹理
	auto texture = Director::getInstance()->getTextureCache()->addImage(img);
	int width = texture->getPixelsWide();
	int hight = texture->getPixelsHigh();

	//int rows = 1;
	//int cols = 5;

	int frameWidth = width / cols;
	int frameHight = hight / rows;

	//保存图片帧
	Vector<cocos2d::SpriteFrame *> array;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			auto frame = SpriteFrame::createWithTexture(texture, Rect(frameWidth * j, frameHight * i, frameWidth, frameHight));
			array.pushBack(frame);

		}
	}
	auto animation = Animation::createWithSpriteFrames(array, delay, loops);   //此处createWithSpriteFrames()函数确实每帧间隔时间参数，需自行加上去！！！
	//animation->setDelayPerUnit(2.8f / 14.0f); // 这个动画包含14帧，将会持续2.8秒.  
	//animation->setRestoreOriginalFrame(true); // 播放完之后返回到第一帧  

	return animation;
}

Animation * UiUtils::createAnimation_with_imgs(const std::string& img, int count, float delay /*= 1.0F*/, unsigned int loops /*= 1*/)
{
	//保存图片帧
	Vector<cocos2d::SpriteFrame *> array;
	for (int i = 0; i < count; i++)
	{
		auto path = StringUtils::format("%s%ld.png", img.c_str(), i);
		//创建2D纹理
		auto texture = Director::getInstance()->getTextureCache()->addImage(path);
		Size size = texture->getContentSize();
		auto frame = SpriteFrame::createWithTexture(texture, Rect(0, 0, size.width, size.height));
		array.pushBack(frame);
	}
	//创建2D纹理
	auto texture = Director::getInstance()->getTextureCache()->addImage(img);
	auto animation = Animation::createWithSpriteFrames(array, delay, loops);   //此处createWithSpriteFrames()函数确实每帧间隔时间参数，需自行加上去！！！

	return animation;
}

void UiUtils::LockScreen(Node* root /*= nullptr*/)
{
	if (root == nullptr){
		root = Director::getInstance()->getRunningScene();
	}
	
	auto nLockPanel = root->getChildByName("LockPanel");
    if (nLockPanel)
    {
        return;
    }
    auto nLockpanel = Layout::create();
    nLockpanel->setTouchEnabled(true);
// 	nLockpanel->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
// 	nLockpanel->setBackGroundColor(Color3B(255,0,0));
	nLockpanel->setBackGroundColorOpacity(180);
	nLockpanel->setContentSize(Size(designSize.width, designSize.height));
    nLockpanel->setName("LockPanel");
	root->addChild(nLockpanel);
	nLockpanel->setLocalZOrder(999);
    
}
void UiUtils::UnLockScreen(Node* root /*= nullptr*/)
{
	if (root == nullptr){
		root = Director::getInstance()->getRunningScene();
	}
   
	auto nLockPanel = root->getChildByName("LockPanel");
    if (nLockPanel)
    {
        nLockPanel->removeFromParent();
    }
}

void UiUtils::ShowTransition(Action* transition, Node* root /*= nullptr*/)
{
	if (root == nullptr){
		root = Director::getInstance()->getRunningScene();
	}
	Layout* _layout = Layout::create();
	_layout->setContentSize(winSize);
	_layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	_layout->setBackGroundColor(Color3B(255, 255, 255));
	_layout->setName("TransitionPanel");
	root->addChild(_layout);
	_layout->runAction(transition);
}


void UiUtils::DoSoundBtn(Button* btnSound, const std::string& on, const std::string& off, bool bgm)
 {
	auto key = btnSound->getName().c_str();
	bool sound = UserDefault::getInstance()->getBoolForKey(key, true);
	 if (sound)
	 {
		 btnSound->loadTextureNormal(on, TextureResType::PLIST);
		 btnSound->loadTexturePressed(on, TextureResType::PLIST);
	 }
	 else
	 {
		 btnSound->loadTextureNormal(off, TextureResType::PLIST);
		 btnSound->loadTexturePressed(off, TextureResType::PLIST);
	 }
	 btnSound->addClickEventListener([=](Ref* sender) {
		 Button* btn = static_cast<Button*>(sender);
	//	 AudioManager::clickBtn();
		 auto nismusicon = UserDefault::getInstance()->getBoolForKey(key, true);
		 if (nismusicon)
		 {
			 btnSound->loadTextureNormal(off, TextureResType::PLIST);
			 btnSound->loadTexturePressed(off, TextureResType::PLIST);
			 if (bgm){
				 Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("turnoffmusic");
				 AudioManager::getInstance()->stopAll();
				 AudioManager::getInstance()->setMusicOn(false);
			 }
			 else {
				 Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("turnoffsound");
				 AudioManager::getInstance()->stopAllVoice();
				 AudioManager::getInstance()->setSoundOn(false);
			 }
			 
			 CCLOG("VoiceBtnClick-->off");
// 			 
 			 UserDefault::getInstance()->setBoolForKey(key, false);
 			 UserDefault::getInstance()->flush();
		 }
		 else
		 {
			 CCLOG("VoiceBtnClick-->on");
			 btnSound->loadTextureNormal(on, TextureResType::PLIST);
			 btnSound->loadTexturePressed(on, TextureResType::PLIST);
 			 UserDefault::getInstance()->setBoolForKey(key, true);
 			 UserDefault::getInstance()->flush();
			 if (bgm){

				 Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("turnoonmusic");
				 AudioManager::getInstance()->setMusicOn(true);
				 AudioManager::getInstance()->restoreBMG();
			 }
			 else {

				 Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("turnonsound");
				 AudioManager::getInstance()->setSoundOn(true);
			 }
		 }
	 });

 }

 void UiUtils::ShowWarning(const char * nwarningstr)
 {
#ifdef COCOS2D_DEBUG
	 MessageBox(nwarningstr, "message");
#endif 
 }

 Layout* UiUtils::getRandomColorLayout(const Size& size)
 {

	 Layout* _layout = Layout::create();
	 _layout->setContentSize(size);
	 _layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	 _layout->setBackGroundColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
	 _layout->setBackGroundColorOpacity(100);
	 return _layout;
 }

 Node* UiUtils::ShowUI(const std::string& filepath, bool listenKeyBack /*= true*/)
 {
	
	 auto scene = Director::getInstance()->getRunningScene();
	 Node* dialogRoot = scene->getChildByName(filepath);
	 Node* dialog = nullptr;
	 if (dialogRoot){//当前界面已经显示了要显示 dialog
		 dialog = dialogRoot->getChildByName("dialog");
	 }
	 else{

		 Layout* _layout = Layout::create();
		 _layout->setContentSize(screenSize);
		 _layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
		 _layout->setBackGroundColor(Color3B(0,0,0));
		 _layout->setBackGroundColorOpacity(100);
		 _layout->setName(filepath);
		 _layout->setTouchEnabled(true);


		 dialog = UIFactory::getInstance()->createUI(filepath);
		 if (dialog == nullptr){
			 CCLOG("UIFactory can't create the UI->%s", filepath.c_str());
			 dialog = CSLoader::createNode(filepath);
		 }
		 
		 dialog->setAnchorPoint(Vec2(0.5, 0.5));
		 dialog->setPosition(screenSize/2);
		 dialog->setName("dialog");
		 dialog->setScale(0);
		 dialog->setOpacity(50);
		 dialog->runAction(Spawn::create(ScaleTo::create(0.2,1),FadeIn::create(0.2),nullptr));
		 
		 _layout->addChild(dialog);
		 scene->addChild(_layout);
		 dialogRoot = _layout;

		 dialogStack.push_back(filepath);
	 }

	
	 return dialog;
 }

 bool UiUtils::CloseUI(const std::string& filepath, std::function<void()> callback /*= nullptr*/)
 {
	 vectorEraseObject(dialogStack, filepath);
	 auto scene = Director::getInstance()->getRunningScene();
	 Node* dialogRoot = scene->getChildByName(filepath);
	 Node* dialog = nullptr;
	 if (dialogRoot){
		 dialog = dialogRoot->getChildByName("dialog");
		 if (dialog){
			 dialog->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2, 0), FadeOut::create(0.2), nullptr), CallFunc::create([=](){
				 dialogRoot->removeAllChildren();
				 dialogRoot->removeFromParent();
				 if (callback){
					 callback();
				 }
			 }), nullptr));
		 }
		 return true;
	 }
	 return false;
 }

 bool UiUtils::onKeyBackListenUI()
 {
	 if (dialogStack.empty()){ 
		 return false;
	 }
	 auto dialogStr = dialogStack.back();
	 
	 return CloseUI(dialogStr);;
 }
 void UiUtils::convSpriteGray(Sprite* node)
 {
	 GLProgramState *glState = nullptr;
	 auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,
		 ccPositionTexture_GrayScale_frag);
	 glState = GLProgramState::getOrCreateWithGLProgram(program);
	 node->setGLProgramState(glState);
 }

 void UiUtils::convSpriteNormal(Sprite* node)
 {
	 GLProgramState *glState = GLProgramState::getOrCreateWithGLProgramName("ShaderPositionTextureColor_noMVP");
	 glState->setUniformTexture("u_texture", node->getTexture());
	 glState->getGLProgram()->updateUniforms();
	 node->setGLProgramState(glState);
 }

 void UiUtils::convImageViewGray(ImageView* iv)
 {
	 ((cocos2d::ui::Scale9Sprite*)(iv->getVirtualRenderer()))->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
 }

 void UiUtils::convImageViewNormal(ImageView* iv)
 {
	 ((cocos2d::ui::Scale9Sprite*)(iv->getVirtualRenderer()))->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
 }