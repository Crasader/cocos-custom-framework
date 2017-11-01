# 定制引擎

![cocos2d](https://img.shields.io/badge/cocos2d-v3.10-green.svg)<br>

### Label 富文本
可以设置一段文本为其他颜色
```
auto str = "the text color is [#ff0000]red,[#00ff00]green,[#0000ff]blue,[#ffffff]restore the white";
Label* label = Label::createWithBMFont("fnt.fnt", str);
addChild(lab);

```