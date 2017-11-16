#include "CSAsyncLoader.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"
#include "cocostudio/FlatBuffersSerialize.h"
#include "cocostudio/WidgetCallBackHandlerProtocol.h"
#include "cocostudio/CSParseBinary_generated.h"

#include "cocostudio/WidgetReader/NodeReaderProtocol.h"
#include "cocostudio/WidgetReader/NodeReaderDefine.h"


#include <fstream>


using namespace cocos2d::ui;
using namespace cocostudio;
using namespace cocostudio::timeline;
/* peterson */
using namespace flatbuffers;
/**/




NS_CC_BEGIN

static const char* ClassName_Node = "Node";
static const char* ClassName_SubGraph = "SubGraph";
static const char* ClassName_Sprite = "Sprite";
static const char* ClassName_Particle = "Particle";
static const char* ClassName_TMXTiledMap = "TMXTiledMap";

static const char* ClassName_Panel = "Panel";
static const char* ClassName_Button = "Button";
static const char* ClassName_CheckBox = "CheckBox";
static const char* ClassName_ImageView = "ImageView";
static const char* ClassName_TextAtlas = "TextAtlas";
static const char* ClassName_LabelAtlas = "LabelAtlas";
static const char* ClassName_LabelBMFont = "LabelBMFont";
static const char* ClassName_TextBMFont = "TextBMFont";
static const char* ClassName_Text = "Text";
static const char* ClassName_LoadingBar = "LoadingBar";
static const char* ClassName_TextField = "TextField";
static const char* ClassName_Slider = "Slider";
static const char* ClassName_Layout = "Layout";
static const char* ClassName_ScrollView = "ScrollView";
static const char* ClassName_ListView = "ListView";
static const char* ClassName_PageView = "PageView";
static const char* ClassName_Widget = "Widget";
static const char* ClassName_Label = "Label";

static const char* ClassName_ComAudio = "ComAudio";

// CSLoader
static CSAsyncLoader* _sharedCSLoader = nullptr;
cocos2d::CSAsyncLoader::CSAsyncLoader() 
:_csBuildID("2.1.0.0")
{
}

bool CSAsyncLoader::init()
{

	return true;
}

CSAsyncLoader* CSAsyncLoader::getInstance()
{

	if (!_sharedCSLoader)
	{
		_sharedCSLoader = new (std::nothrow) CSAsyncLoader();
		_sharedCSLoader->init();
	}

	return _sharedCSLoader;
}

bool CSAsyncLoader::addCsb(const std::string& filename)
{
	CSAsyncLoader* load = CSAsyncLoader::getInstance();

	return load->add(filename);
}

void CSAsyncLoader::load(std::function<void(float)> progressCallBack)
{
	CSAsyncLoader* load = CSAsyncLoader::getInstance();
	load->loadAsync(progressCallBack);
}

bool CSAsyncLoader::add(const std::string& fileName)
{

	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);

	CC_ASSERT(FileUtils::getInstance()->isFileExist(fullPath));

	Data buf = FileUtils::getInstance()->getDataFromFile(fullPath);

	if (buf.isNull())
	{
		CCLOG("CSLoader::nodeWithFlatBuffersFile - failed read file: %s", fileName.c_str());
		CC_ASSERT(false);
	}

	auto csparsebinary = GetCSParseBinary(buf.getBytes());


	auto csBuildId = csparsebinary->version();
	if (csBuildId)
	{
		CCASSERT(strcmp(_csBuildID.c_str(), csBuildId->c_str()) == 0,
			StringUtils::format("%s%s%s%s%s%s%s%s%s%s",
			"The reader build id of your Cocos exported file(",
			csBuildId->c_str(),
			") and the reader build id in your Cocos2d-x(",
			_csBuildID.c_str(),
			") are not match.\n",
			"Please get the correct reader(build id ",
			csBuildId->c_str(),
			")from ",
			"http://www.cocos2d-x.org/filedown/cocos-reader",
			" and replace it in your Cocos2d-x").c_str());
	}

// 	// decode plist
// 	auto textures = csparsebinary->textures();
// 	int textureSize = textures->size();
// 	for (int i = 0; i < textureSize; ++i)
// 	{
// 		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(textures->Get(i)->c_str());
// 	}

	pack(csparsebinary->nodeTree());

	CCLOG("the size -> %d", _allRes.size());
	return true;
}


bool CSAsyncLoader::pack(const flatbuffers::NodeTree* nodetree)
{
	if (nodetree == nullptr)
		return false;

	{
		bool node = false;

		std::string classname = nodetree->classname()->c_str();

		auto options = nodetree->options();

		if (classname == "ProjectNode"){
			auto projectNodeOptions = (ProjectNodeOptions*)options->data();
			std::string filePath = projectNodeOptions->fileName()->c_str();

			cocostudio::timeline::ActionTimeline* action = nullptr;
			if (filePath != "" && FileUtils::getInstance()->isFileExist(filePath))
			{
				addCsb(filePath);
			}
		}
		
		if (notLoad(classname) == false){
			
			CCLOG("the node name -> %s", classname.c_str());
			loadNode(classname, (flatbuffers::Table*)options->data());
		}

		auto children = nodetree->children();
		int size = children->size();
		for (int i = 0; i < size; ++i)
		{
			auto subNodeTree = children->Get(i);
			pack(subNodeTree);
		}
	}

	return true;
}
// 
// bool CSAsyncLoader::load()
// {
// 	
//  	for (int i = 0; i < _allRes.size(); i++)
//  	{
//  
//  		auto fileNameData = _allRes.at(i);
//  
//  		int resourceType = fileNameData.resType;
//  		std::string path = fileNameData.path;
//  		std::string errorFilePath = "";
//  
//  		switch (resourceType)
//  		{
//  		case 0:
//  		{
//  				  if (FileUtils::getInstance()->isFileExist(path))
//  				  {
//  					  //加载资源
//  					  CCLOG("load res -> %s", path.c_str());
// 
//  				  }
//  				  else{
//  					  errorFilePath = path;
//  				  }
//  				  break;
//  		}
//  
//  		case 1:
//  		{
//  				  std::string plist = fileNameData.plist;
//  				  SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(path);
//  				  if (spriteFrame)
//  				  {
//  					  //资源在内存中存在
//  				  }
//  				  else
//  				  {//资源在内存中不存在
//  					  if (FileUtils::getInstance()->isFileExist(plist))
//  					  {
//  
//  						  CCLOG("load plist res -> %s", plist.c_str());
//  						  ValueMap value = FileUtils::getInstance()->getValueMapFromFile(plist);
//  						  ValueMap metadata = value["metadata"].asValueMap();
//  						  std::string textureFileName = metadata["textureFileName"].asString();
//  						  CCLOG("load plist texture res -> %s", textureFileName.c_str());
//  						  if (!FileUtils::getInstance()->isFileExist(textureFileName))
//  						  {
//  							  errorFilePath = textureFileName;
//  						  }
//  					  }
//  					  else
//  					  {
//  						  errorFilePath = plist;
//  					  }
//  				  }
//  				  break;
//  		}
//  
//  		default:
//  			break;
//  		}
//  
//  
//  		if (errorFilePath != ""){
//  			CCLOG("the [ %s ]res load error !!!!!!", errorFilePath.c_str());
//  		}
//  	}
// 
// 	return true;
// }

bool CSAsyncLoader::loadAsync(std::function<void(float)> progressCallBack)
{
	_iMaxCount = 0;
	_iCurCount = 0;
	_funProgressCallBack = progressCallBack;
	auto pTextureCache = Director::getInstance()->getTextureCache();
	
	std::vector<std::string> didLoadPlist;
	_iMaxCount = _allRes.size();
	for (int i = 0; i < _allRes.size(); i++)
	{

		auto fileNameData = _allRes.at(i);
		int resourceType = fileNameData.resType;
		std::string path = fileNameData.path;
		std::string errorFilePath = "";
		bool errorLoad = false;
		CCLOG("-------------------------- %s,  index->  %d", path.c_str(),i);
		switch (resourceType)
		{
		case 0:
		{
				  if (FileUtils::getInstance()->isFileExist(path))
				  {
					  //加载资源
					  CCLOG("load res -> %s", path.c_str());

					  pTextureCache->addImageAsync(path, [=](Texture2D *pTexture)
					  {
						  percentAdd();
					  });
				  }
				  else{
					  errorFilePath = path;
					  errorLoad = true;
				  }
				  break;
		}

		case 1:
		{
				  std::string plist = fileNameData.plist;
				  CCLOG("load plist res -> %s", path.c_str());
				  SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(path);
				  bool isDidLoad = find(didLoadPlist.begin(), didLoadPlist.end(), plist) != didLoadPlist.end();
				  if (spriteFrame || isDidLoad)
				  {
					  //资源在内存中存在
					  percentAdd();
				  }
				  else
				  {//资源在内存中不存在
					  if (FileUtils::getInstance()->isFileExist(plist))
					  {
						  didLoadPlist.push_back(plist);
						  //CCLOG("load plist res -> %s", plist.c_str());
						  ValueMap value = FileUtils::getInstance()->getValueMapFromFile(plist);
						  ValueMap metadata = value["metadata"].asValueMap();
						  std::string textureFileName = metadata["textureFileName"].asString();
						  CCLOG("load plist texture res -> %s", textureFileName.c_str());


						  std::string texturePath = plist;

						  // remove .xxx
						  size_t startPos = texturePath.find_last_of(".");
						  texturePath = texturePath.erase(startPos);

						  // append .png
						  texturePath = texturePath.append(".png");

						  if (FileUtils::getInstance()->isFileExist(texturePath))
						  {

							  pTextureCache->addImageAsync(texturePath, [=](Texture2D *pTexture)
							  {
								  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, pTexture);
								  percentAdd();
							  });

						  }
						  else{
							  errorFilePath = textureFileName;
							  errorLoad = true;
						  
						  }
					  }
					  else
					  {
						  errorFilePath = plist;
						  errorLoad = true;
					  }
				  }
				  break;
		}

		default:
			break;
		}

		if (errorLoad){
			percentAdd();
		}
		if (errorFilePath != ""){
			CCLOG("the [ %s ]res load error !!!!!!", errorFilePath.c_str());
		}
	}
	return true;
}


void CSAsyncLoader::percentAdd()
{
	_iCurCount++;
	CCLOG("load index -> %d, total -> %d", _iCurCount, _iMaxCount);
	float per = ((float)_iCurCount / (float)(_iMaxCount));
	//CCLOG("Texture  per -> %.02f ,%d,  max -> %d", per, i, _allRes.size() - 1);
	if (_funProgressCallBack){
		_funProgressCallBack(per);
	}
	if (per >= 1){
		_allRes.clear();
	}
}





bool CSAsyncLoader::loadNode(const std::string& classname, const flatbuffers::Table* nodeOptions)
{
	std::vector<const flatbuffers::ResourceData*> resdata;

	if (classname == ClassName_Node){
		
	}
	else if (classname == ClassName_Sprite)
	{
		auto options = (SpriteOptions*)nodeOptions;
		auto fileNameData = options->fileNameData();
		resdata.push_back(fileNameData);
	}
	else if (classname == ClassName_Button)
	{
		auto options = (ButtonOptions*)nodeOptions;

		bool normalFileExist = false;
		auto normalDic = options->normalData();
		auto pressedDic = options->pressedData();
		auto disabledDic = options->disabledData();


		resdata.push_back(normalDic);
		resdata.push_back(pressedDic);
		resdata.push_back(disabledDic);
	}
	else if (classname == ClassName_ImageView)
	{
		auto options = (ImageViewOptions*)nodeOptions;
		auto imageFileNameDic = options->fileNameData();
		resdata.push_back(imageFileNameDic);
	}
	else if (classname == ClassName_SubGraph)
	{
	}
	else if (classname == ClassName_Particle)
	{
	}
	else if (classname == ClassName_TMXTiledMap)
	{
	}
	else if (classname == ClassName_Panel)
	{
	}
	else if (classname == ClassName_CheckBox)
	{
	}
	else if (classname == ClassName_TextAtlas)
	{
	}
	else if (classname == ClassName_LabelAtlas)
	{
	}
	else if (classname == ClassName_LabelBMFont)
	{
	}
	else if (classname == ClassName_TextBMFont)
	{
	}
	else if (classname == ClassName_Text)
	{
	}
	else if (classname == ClassName_LoadingBar)
	{
	}
	else if (classname == ClassName_TextField)
	{
	}
	else if (classname == ClassName_Slider)
	{
	}
	else if (classname == ClassName_Layout)
	{
	}
	else if (classname == ClassName_ScrollView)
	{
	}
	else if (classname == ClassName_ListView)
	{
	}
	else if (classname == ClassName_PageView)
	{
	}
	else if (classname == ClassName_Widget)
	{
	}
	else if (classname == ClassName_Label)
	{
	}	

	
	for (int i = 0; i < resdata.size(); i++)
	{
		ResPack _pack;
		_pack.classname = classname;

	 	auto fileNameData = resdata.at(i);
	 	int resourceType = fileNameData->resourceType();
	 	std::string path = fileNameData->path()->c_str();
		_pack.path = path;
			
		_pack.resType = resourceType;

	 	switch (resourceType)
	 	{
	 	case 0:
	 	{
				  
	 				break;
	 	}
	 
	 	case 1:
	 	{
				  std::string plist = fileNameData->plistFile()->c_str();
				  _pack.plist = plist;
	 	}
	 
	 	default:
	 		break;
	 	}
		_allRes.push_back(_pack);
	}


	return true;
}

bool CSAsyncLoader::notLoad(const std::string& classname)
{

	bool notload =
		classname == "SimpleAudio" ||
		classname == "ProjectNode";
	

	return notload;
}


int CSAsyncLoader::getResSize()
{
	CSAsyncLoader* load = CSAsyncLoader::getInstance();
	return load->_allRes.size();
}








// if (classname == "ProjectNode")
// {
// 	auto projectNodeOptions = (ProjectNodeOptions*)options->data();
// 	std::string filePath = projectNodeOptions->fileName()->c_str();
// 
// 	cocostudio::timeline::ActionTimeline* action = nullptr;
// 	if (filePath != "" && FileUtils::getInstance()->isFileExist(filePath))
// 	{
// 		Data buf = FileUtils::getInstance()->getDataFromFile(filePath);
// 		loadCsb(buf);
// 	}
// }
// else if (classname == "SimpleAudio")
// {
// 
// 	auto optionsSimpleAudio = (ComAudioOptions*)((const flatbuffers::Table*)options->data());
// 
// 	auto fileNameData = optionsSimpleAudio->fileNameData();
// 	CCLOG("the file name -> %s", fileNameData->path()->c_str());
// }
// else
// {
// 	std::string customClassName = nodetree->customClassName()->c_str();
// 	if (customClassName != "")
// 	{
// 		classname = customClassName;
// 	}
// 	std::string readername = getGUIClassName(classname);
// 	readername.append("Reader");
// 
// 	NodeReaderProtocol* reader = dynamic_cast<NodeReaderProtocol*>(ObjectFactory::getInstance()->createObject(readername));
// 	if (reader)
// 	{
// 		node = nodeClassification(classname, (const flatbuffers::Table*)options->data());
// 	}
// }
// 
// // If node is invalid, there is no necessity to process children of node.
// if (!node)
// {
// 	return false;
// }



void cocos2d::CSAsyncLoader::destroyInstance()
{

	CC_SAFE_DELETE(_sharedCSLoader);
	ActionTimelineCache::destroyInstance();
}

NS_CC_END
