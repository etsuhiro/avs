#include "AvsImage.h"
#include "AvsSprite.h"

using namespace avs;

AvsImage::AvsImage()
	: m_busy(false)
{
}

AvsImage::~AvsImage()
{
	for (std::list<avs::AvsSprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); ++it){
		AvsSprite::Delete(*it);
	}
}

void AvsImage::FinalizeSprite(avs::AvsSprite* sprite)
{
	// spriteを親に持つ親子関係を解消
	for (std::list<avs::AvsSprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); ++it){
		if ((*it)->GetParent() == sprite)
			(*it)->Disown();
	}

	// 対象のspriteをコントロールしているcgtがあったら削除
	for (std::list<avs::Cgt>::iterator it = cgt_list.begin(); it != cgt_list.end();){
		if (it->IsTarget(sprite)){
			it = cgt_list.erase(it);
		}
		else
			++it;
	}

	delete sprite;
}

avs::AvsSprite* AvsImage::SearchSprite(int id)
{
	for (std::list<avs::AvsSprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); ++it){
		if ((*it)->GetId() == id){
			return *it;
		}
	}
	return NULL;
}

void AvsImage::Cg(int id, const char* file, const avs::ImageInfo& info, int parentId)
{
	for (std::list<avs::AvsSprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); ++it){
		if ((*it)->GetId() == id){
			avs::AvsSprite* sprite = *it;
			// 先にリストから削除しておく
			sprite_list.erase(it);
			FinalizeSprite(sprite);
			break;
		}
	}
	if (file == 0)	return;

	avs::AvsSprite* parent = (parentId) ? SearchSprite(parentId) : NULL;
	avs::AvsSprite* pSprite = CreateSprite(id, file, info, parent);
	if (pSprite)
		sprite_list.push_back(pSprite);
}

void AvsImage::CgTrans(int id, float time, const avs::ImageInfo& info, unsigned int change, int method)
{
	avs::AvsSprite* sprite = SearchSprite(id);
	if (sprite){
		if (change){
			cgt_list.push_back(avs::Cgt(sprite, info, time, change, method));
		}
		else{
			for (std::list<avs::Cgt>::iterator it = cgt_list.begin(); it != cgt_list.end();){
				if (it->IsTarget(sprite)){
					it->Finalize();
					it = cgt_list.erase(it);
				}
				else
					++it;
			}
		}
	}
}

void AvsImage::Update(float elapsedTime)
{
	for (std::list<avs::Cgt>::iterator it = cgt_list.begin(); it != cgt_list.end();){
		if (it->Update(elapsedTime) < 0)
			it = cgt_list.erase(it);
		else
			++it;
	}
	for (std::list<avs::AvsSprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); ++it){
		(*it)->Update();
	}
}

void AvsImage::Draw()
{
	// priolityに従ってソートリストを作成
	std::list<avs::AvsSprite*> priolityList;
	for (std::list<avs::AvsSprite*>::iterator it = sprite_list.begin(); it != sprite_list.end(); ++it){
		std::list<avs::AvsSprite*>::iterator p = priolityList.begin();
		for (; p != priolityList.end(); ++p){
			if ((*p)->ImageInfo().priority > (*it)->ImageInfo().priority)
				break;
		}
		priolityList.insert(p, *it);
	}

	for (std::list<avs::AvsSprite*>::iterator it = priolityList.begin(); it != priolityList.end(); ++it){
		(*it)->Draw();
	}
}

bool AvsImage::IsBusy()
{
	return m_busy;
}
