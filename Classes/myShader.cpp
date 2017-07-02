#include "myShader.h"

const char* shaderNameStroke = "ShjyShader_Stroke";
NS_CC_BEGIN

namespace myShader
{
	// 传入描边宽度（像素为单位），描边颜色，图片大小，获得GLProgramState  
	cocos2d::GLProgramState* getStrokeProgramState(float outlineSize, cocos2d::Color3B outlineColor, cocos2d::Size textureSize, cocos2d::Color3B foregroundColor/* = cocos2d::Color3B::WHITE*/)
	{
		CCLOG("the size is --> %f,%f", textureSize.width, textureSize.height);
		auto glprogram = GLProgramCache::getInstance()->getGLProgram(shaderNameStroke);
		if (!glprogram)
		{

			std::string fragmentSource = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->fullPathForFilename("Shader/stroke.fsh"));
			glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragmentSource.c_str());
			GLProgramCache::getInstance()->addGLProgram(glprogram, shaderNameStroke);
		}

		auto glprogramState = GLProgramState::create(glprogram);

		glprogramState->setUniformFloat("outlineSize", outlineSize);
		glprogramState->setUniformVec3("outlineColor", Vec3(outlineColor.r / 255.0f, outlineColor.g / 255.0f, outlineColor.b / 255.0f));
		glprogramState->setUniformVec2("textureSize", Vec2(textureSize.width, textureSize.height));
		glprogramState->setUniformVec3("foregroundColor", Vec3(foregroundColor.r / 255.0f, foregroundColor.g / 255.0f, foregroundColor.b / 255.0f));
		
		return glprogramState;
	}
}

NS_CC_END