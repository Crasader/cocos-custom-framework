#include "EasyBuild.h"

ImageView* EasyBuild::toImageView(Ref* rNode, bool touchEnbale /* = false*/)
{
	auto img = static_cast<ImageView*>(rNode);
	img->setTouchEnabled(touchEnbale);
	return img;
}

Button* EasyBuild::toButton(Ref* rNode)
{
	return static_cast<Button*>(rNode);
}

LoadingBar* EasyBuild::toLoadingBar(Ref* rNode, int percent /*= 100*/)
{
	auto bar = static_cast<LoadingBar*>(rNode);
	bar->setPercent(percent);
	return bar;
}

Sprite* EasyBuild::toSprite(Ref* rNode)
{
	return static_cast<Sprite*>(rNode);
}

Layout* EasyBuild::toLayout(Ref* rNode)
{
	return static_cast<Layout*>(rNode);
}

TextAtlas* EasyBuild::toTextAtlas(Ref* rNode)
{
	return static_cast<TextAtlas*>(rNode);
}

ParticleSystemQuad* EasyBuild::toParticle(Ref* rNode, bool play)
{
	ParticleSystemQuad*  pParticleSystemQuad = static_cast<ParticleSystemQuad*>(rNode);
	play ? pParticleSystemQuad->resetSystem() : pParticleSystemQuad->stopSystem();
	return pParticleSystemQuad;
}

Label* EasyBuild::toLabel(Ref* rNode)
{
	return static_cast<Label*>(rNode);
}

Layer* EasyBuild::toLayer(Ref* rNode)
{
	return static_cast<Layer*>(rNode);
}

CheckBox* EasyBuild::toCheckBox(Ref* rNode)
{
	return static_cast<CheckBox*>(rNode);
}

Node* EasyBuild::toNode(Ref* rNode)
{
	return static_cast<Node*>(rNode);
}
