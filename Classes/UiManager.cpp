
#include "UiManager.h"
#include "PublicDefine.h"
#include "DialogAdapter.h"

static UiManager *_UiManager;



UiManager * UiManager::shareManager()
{
	if (!_UiManager)
	{
		_UiManager = new UiManager();
		_UiManager->init();
	}
	return _UiManager;
}
bool UiManager::init()
{

	return true;
}
void UiManager::LockScreen(Node* root /*= nullptr*/)
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
void UiManager::UnLockScreen(Node* root /*= nullptr*/)
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

void UiManager::ShowTransition(Action* transition, Node* root /*= nullptr*/)
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

 void UiManager::ShowWarning(const char * nwarningstr)
 {
#ifdef COCOS2D_DEBUG
	 MessageBox(nwarningstr, "message");
#endif 
 }

 Layout* UiManager::getRandomColorLayout(const Size& size)
 {

	 Layout* _layout = Layout::create();
	 _layout->setContentSize(size);
	 _layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	 _layout->setBackGroundColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
	 _layout->setBackGroundColorOpacity(100);
	 return _layout;
 }

 DialogAdapter* UiManager::ShowUI(const std::string& filepath)
 {
	return DialogHelper::ShowUI(filepath);
 }

 DialogAdapter* UiManager::CloseUI(const std::string& filepath)
 {

	return DialogHelper::CloseUI(filepath);
 }

 bool UiManager::onKeyBack()
 {
	 return DialogHelper::onKeyBack();
 }
 void UiManager::convSpriteGray(Sprite* node)
 {
	 GLProgramState *glState = nullptr;
	 auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert,
		 ccPositionTexture_GrayScale_frag);
	 glState = GLProgramState::getOrCreateWithGLProgram(program);
	 node->setGLProgramState(glState);
 }

 void UiManager::convSpriteNormal(Sprite* node)
 {
	 GLProgramState *glState = GLProgramState::getOrCreateWithGLProgramName("ShaderPositionTextureColor_noMVP");
	 glState->setUniformTexture("u_texture", node->getTexture());
	 glState->getGLProgram()->updateUniforms();
	 node->setGLProgramState(glState);
 }

 void UiManager::convImageViewGray(ImageView* iv)
 {
	 ((cocos2d::ui::Scale9Sprite*)(iv->getVirtualRenderer()))->setState(cocos2d::ui::Scale9Sprite::State::GRAY);
 }

 void UiManager::convImageViewNormal(ImageView* iv)
 {
	 ((cocos2d::ui::Scale9Sprite*)(iv->getVirtualRenderer()))->setState(cocos2d::ui::Scale9Sprite::State::NORMAL);
 }