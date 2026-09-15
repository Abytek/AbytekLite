module Bindless;

public struct TF_ResourceBindlessHandle
{
    public static const uint32_t INVALID_INDEX = 0xFFFFFFFF;

    public uint32_t Index = INVALID_INDEX;

    public bool IsValid()
    {
        return Index != INVALID_INDEX;
    }

    public operator bool()
    {
        return IsValid();
    }

    public T Get<T>()
    {
        return ResourceDescriptorHeap[NonUniformResourceIndex(Index)];
    }
    public T GetUniform<T>()
    {
        return ResourceDescriptorHeap[Index];
    }
};