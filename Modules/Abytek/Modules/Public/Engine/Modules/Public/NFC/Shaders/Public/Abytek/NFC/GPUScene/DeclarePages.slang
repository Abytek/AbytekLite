module DeclarePages;

namespace Abytek::NFC::GPUScene
{
#ifdef ABYTEK_GPU_SCENE_ENABLE_WRITE_PAGES
    public typealias F_DeclaredPage = RWByteAddressBuffer;
#else
    public typealias F_DeclaredPage = ByteAddressBuffer;
#endif
}

#ifdef ABYTEK_GPU_SCENE_ENABLE_MULTIPLE_PAGES
public Abytek::NFC::GPUScene::F_DeclaredPage GPUScenePages[];
#else
public Abytek::NFC::GPUScene::F_DeclaredPage GPUScenePage_Single;
#endif