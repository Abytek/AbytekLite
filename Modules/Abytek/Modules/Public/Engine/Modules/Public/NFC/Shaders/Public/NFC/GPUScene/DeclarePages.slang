module DeclarePages;

namespace NFC::GPUScene
{
#ifdef ABYTEK_GPU_SCENE_ENABLE_WRITE_PAGES
    public typealias F_DeclaredPage = RWByteAddressBuffer;
#else
    public typealias F_DeclaredPage = ByteAddressBuffer;
#endif
}

#ifdef ABYTEK_GPU_SCENE_ENABLE_MULTIPLE_PAGES
public NFC::GPUScene::F_DeclaredPage GPUScenePages[];
#else
public NFC::GPUScene::F_DeclaredPage GPUScenePage_Single;
#endif