#ifdef ANDROID

#include <string>
#include <AssetManager.h>
#include <Logger.h>

namespace sst = sunspot;


sst::AssetManager sst::AssetManager::assets{};


sst::Asset::Asset(AAsset* file)
:	mFile   { file }
,	mLength { 0 }
,	mContent{ nullptr }
{}


size_t& sst::Asset::GetLength()
{
	if (!mContent)
	{
		mLength  = static_cast<size_t>(AAsset_getLength(mFile));
	}
	return mLength;
}


char* sst::Asset::GetContent()
{
	if (!mContent)
	{
		GetLength();
		mContent = std::unique_ptr<char[]>(new char[mLength + 1]);
		AAsset_read(mFile, mContent.get(), mLength);
		mContent.get()[mLength] = '\0';
	}
	return mContent.get();
}


sst::AssetManager::AssetManager()
:	mAssets{ nullptr }
{}


void sst::AssetManager::Init(AAssetManager* assets)
{
	mAssets = assets;
}


void sst::AssetManager::Init(JNIEnv* env, jobject assets)
{
	mAssets = AAssetManager_fromJava(env, assets);
}


sst::Asset sst::AssetManager::Open(const std::string& name) const
{
	if (!mAssets)
	{
		throw sst::AssetException{ "AssetManager instance is not initialized" };
	}

	AAsset* file{ AAssetManager_open(mAssets, name.c_str(), AASSET_MODE_BUFFER) };
	if (!file)
	{
		throw sst::AssetException{ "Could not open a file: " + name };
	}

	return sst::Asset{ file };
}

#endif // ANDROID
