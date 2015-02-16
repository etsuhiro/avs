/*! @file Setup.cpp */
#include "Setup.h"

#include <windows.h>
#include <cstdio>
#include "tinyxml.h"

void WindowsSetupData::Init()
{
	width = 640;
	height  = 480;
	nLayer = 3;
	defaultTime = 500;
	fovy = 0;
	window.pos.x = 32;	// ウィンドウの表示位置
	window.pos.y = 320;	// ウィンドウの表示位置
	window.text.x = 32;	// テキストの開始位置（posからのオフセット）
	window.text.y = 32;	// テキストの開始位置（posからのオフセット）
	window.linefeed = 24;	// 改行幅
	window.name.x = 0;	// 名前の開始位置（posからのオフセット）
	window.name.y = 0;	// 名前の開始位置（posからのオフセット）
	window.click.x = 600;	// クリックアイコンの表示位置（posからのオフセット）
	window.click.y = 80;	// クリックアイコンの表示位置（posからのオフセット）
	text.clum = 25;
	text.row = 3;
	text.extra = 1;

	wcscpy_s( fontData, L"MS ゴシック" );
	fontSize = 26;
//	wcscpy_s( fontData, L"HGSｺﾞｼｯｸE" );
//	fontSize = 24;

}

void WindowsSetupData::Load()
{
	TiXmlDocument x;

	if (x.LoadFile("sink.cfg")){
		TiXmlElement* e1 = x.RootElement();
		TiXmlElement* e;
		if ((e = e1->FirstChildElement("screen"))){
			const char *p;
			if ((p = e->Attribute("width"))){
				width = atoi(p);
			}
			if ((p = e->Attribute("height"))){
				height = atoi(p);
			}
		}
		if ((e = e1->FirstChildElement("data"))){
			const char*	p;
			if ((p = e->Attribute("path"))){
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, p, -1, dataPath, MAX_PATH);
			}
		}
		if ((e = e1->FirstChildElement("image"))){
			const char *p;
			if ((p = e->Attribute("layer"))){
				nLayer = atoi(p);
			}
		}
		if ((e = e1->FirstChildElement("fade"))){
			const char *p;
			if ((p = e->Attribute("time"))){
				defaultTime = atoi(p);
			}
		}
		if ((e = e1->FirstChildElement("window"))){
			const char*	p;
			if ( p = e->Attribute("src")){
				strncpy_s( windowSrc, p, 256 );
			}
			if ((p = e->Attribute("x"))){
				window.pos.x = atoi(p);
			}
			if ((p = e->Attribute("y"))){
				window.pos.y = atoi(p);
			}
			if ((p = e->Attribute("text_x"))){
				window.text.x = atoi(p);
			}
			if ((p = e->Attribute("text_y"))){
				window.text.y = atoi(p);
			}
			if ((p = e->Attribute("linefeed"))){
				window.linefeed = atoi(p);
			}
			if ((p = e->Attribute("name_x"))){
				window.name.x = atoi(p);
			}
			if ((p = e->Attribute("name_y"))){
				window.name.y = atoi(p);
			}
			if ((p = e->Attribute("click_x"))){
				window.click.x = atoi(p);
			}
			if ((p = e->Attribute("click_y"))){
				window.click.y = atoi(p);
			}
		}
		if ((e = e1->FirstChildElement("text"))){
			const char*	p;
			if ( p = e->Attribute("font")){
//				strncpy_s( fontData, p, 256 );
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, p, -1, fontData, 128);
			}
			if ( p = e->Attribute("size")){
				fontSize = atoi(p);
			}
			if ((p = e->Attribute("clum"))){
				text.clum = atoi(p);
			}
			if ((p = e->Attribute("row"))){
				text.row = atoi(p);
			}
			if ((p = e->Attribute("extra"))){
				text.extra = atoi(p);
			}
		}
		for (e = e1->FirstChildElement("var"); e; e = e->NextSiblingElement("var"))
		{
			int n = 0;
			const char* p;
			if ((p = e->Attribute("default"))){
				n = atoi(p);
			}
			if ( p = e->Attribute("global")){
			}
			if ( p = e->Attribute("local")){
			}
		}
	}
}
