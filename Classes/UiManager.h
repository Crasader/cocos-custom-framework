#ifndef _UiManager_h_
#define _UiManager_h_

# include "PublicCoco.h"

class DialogAdapter;
class UiManager
{
public:
	static UiManager* shareManager();
	bool init();
	
	static DialogAdapter* ShowUI(const std::string& filepath);
	static DialogAdapter* CloseUI(const std::string& filepath);
	static bool onKeyBack();

    static void LockScreen(Node* root = nullptr);
	static void UnLockScreen(Node* root = nullptr);

	static void ShowWarning(const char * nwarningstr);
	
	static void ShowTransition(Action* transition,Node* root = nullptr);

	static Layout* getRandomColorLayout(const Size& size);


	static void convSpriteGray(Sprite* node);
	static void convSpriteNormal(Sprite* node);
	static void convImageViewGray(ImageView* node);
	static void convImageViewNormal(ImageView* node);
protected:
	
};

#endif /* _UiManager_h_ */
