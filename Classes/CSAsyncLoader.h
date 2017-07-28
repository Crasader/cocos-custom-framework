#ifndef __CSAsyncLoader__
#define __CSAsyncLoader__

#include "PublicCoco.h"

namespace flatbuffers
{
	class FlatBufferBuilder;
	class Table;
	struct NodeTree;

	struct WidgetOptions;
	struct SingleNodeOptions;
	struct SpriteOptions;
	struct ParticleSystemOptions;
	struct TMXTiledMapOptions;
	struct ProjectNodeOptions;

	struct ComponentOptions;
	struct ComAudioOptions;
}

NS_CC_BEGIN
class CSAsyncLoader
{

public:
	struct ResPack
	{
		std::string plist;
		std::string classname;
		std::string path;
		int resType;
	};


public:
    static CSAsyncLoader* getInstance();
    static void destroyInstance();

	CSAsyncLoader();

	//static void loadCsb(const std::string& filename);
	static bool addCsb(const std::string& filename);
	static void load(std::function<void(float)> progressCallBack);
	static int getResSize();

	
	bool add(const std::string& fileName);
	bool pack(const flatbuffers::NodeTree* nodetree);
	//bool load();
	bool loadAsync(std::function<void(float)> progressCallBack);
	bool loadNode(const std::string& classname, const flatbuffers::Table* nodeOptions);

	bool notLoad(const std::string& classname);
	
protected:
	bool init();
	std::vector<ResPack> _allRes;
	std::string _csBuildID;

	int _iCurCount = 0;
	int _iMaxCount = 0;
};

NS_CC_END

#endif /* defined(__CSAsyncLoader__) */
