#ifndef _UiUtils_h_
#define _UiUtils_h_

# include "PublicCoco.h"
class UiUtils
{
public:
	
	/*创建帧动画*/
	static Animation *createAnimation(const std::string&img, int rows = 1, int cols = 1, float delay = 1.0F, unsigned int loops = 1);
	/*创建帧动画*/
	static Animation *createAnimation_with_imgs(const std::string& img, int count, float delay = 1.0F, unsigned int loops = 1);
	static ParticleSystemQuad* createParticle(const std::string& filename);

	static Node* ShowUI(const std::string& filepath,bool listenKeyBack = true);
	static bool CloseUI(const std::string& filepath,std::function<void()> callback = nullptr);
	static bool onKeyBackListenUI();

    static void LockScreen(Node* root = nullptr);
	static void UnLockScreen(Node* root = nullptr);

	static void ShowWarning(const char * nwarningstr);
	
	static void DoSoundBtn(Button* btnSound, const std::string& on, const std::string& off,bool bgm);

	static void ShowTransition(Action* transition,Node* root = nullptr);

	static Layout* getRandomColorLayout(const Size& size);

	static void convSpriteGray(Sprite* node);
	static void convSpriteNormal(Sprite* node);
	static void convImageViewGray(ImageView* node);
	static void convImageViewNormal(ImageView* node);
protected:
	static vector<std::string> dialogStack;
	
};

#endif /* _UiUtils_h_ */
