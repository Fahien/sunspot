#ifdef ANDROID
#ifndef SST_ASSETMANAGER_H
#define SST_ASSETMANAGER_H

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <memory>
#include <stdexcept>


namespace sunspot
{

class AssetException : public std::runtime_error
{
public:
	AssetException(const std::string& msg)
	:	std::runtime_error(msg)
	{}
};


class Asset
{
public:
	Asset(AAsset* file);

	/// Returns the content as c string
	char* GetContent();

	/// Returns the length of the string
	size_t& GetLength();

private:
	/// Handle to the file (no need to release it)
	AAsset* mFile;

	/// Length in byte
	size_t  mLength;

	/// Content interpreted as c string
	std::unique_ptr<char[]> mContent;
};


class AssetManager
{
public:
	static AssetManager assets;

	void Init(AAssetManager* assets);
	void Init(JNIEnv* env, jobject assets);

	/// Opens an asset
	Asset Open(const std::string& name) const;

private:
	AssetManager();

	/// Android asset manager
	AAssetManager* mAssets;
};

}


#endif // SST_ASSETMANAGER_H
#endif // ANDROID
