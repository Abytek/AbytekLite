#include "Abytek/Assets/MaterialProperty.hpp"
#include "Abytek/Assets/Texture.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_MaterialPropertyBase)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_MaterialPropertyBase"));   
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Name);
    }
    
    ABYTEK_REFLECT(F_MaterialPropertyTexture)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialPropertyTexture"));    
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(DefaultValue);
    }
    
    ABYTEK_REFLECT(F_MaterialPropertyList)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialPropertyList"));      
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_B8);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_U32);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_I32);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_F32);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_Texture);
    }

    F_MaterialPropertyList F_MaterialPropertyList::Combine(const F_MaterialPropertyList& A, const F_MaterialPropertyList& B)
    {
        F_MaterialPropertyList Result;
        
#define ABYTEK_INTERNAL_COMBINE_MATERIAL_PROPERTIES(...) \
            { \
                auto& Properties_Result = Result.Properties_ ## __VA_ARGS__; \
                auto& Properties_A = A.Properties_ ## __VA_ARGS__; \
                auto& Properties_B = B.Properties_ ## __VA_ARGS__; \
                Properties_Result.insert(Properties_Result.begin(), Properties_A.begin(), Properties_A.end()); \
                Properties_Result.insert(Properties_Result.begin(), Properties_B.begin(), Properties_B.end()); \
            }
        
        ABYTEK_INTERNAL_COMBINE_MATERIAL_PROPERTIES(B8);
        ABYTEK_INTERNAL_COMBINE_MATERIAL_PROPERTIES(U32);
        ABYTEK_INTERNAL_COMBINE_MATERIAL_PROPERTIES(I32);
        ABYTEK_INTERNAL_COMBINE_MATERIAL_PROPERTIES(F32);
        ABYTEK_INTERNAL_COMBINE_MATERIAL_PROPERTIES(Texture);
    
#undef ABYTEK_INTERNAL_COMBINE_MATERIAL_PROPERTIES
        return ABYTEK_MOVE(Result);
    }

    F_MaterialPropertyListLayout F_MaterialPropertyListLayout::Make(const F_MaterialPropertyList& PropertyList)
    {
        F_MaterialPropertyListLayout Result;
        
#define ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_DYNAMIC(...) \
            { \
                const auto& Properties = PropertyList.Properties_ ## __VA_ARGS__; \
                for (U32 Idx = 0; Idx < Properties.size(); ++Idx) \
                { \
                    const auto& Property = Properties[Idx]; \
                    using F_Property = std::remove_const_t<std::remove_reference_t<decltype(Property)>>; \
                    if (!Property.IsStatic()) \
                    { \
                        Result.DynamicPropertyIndices_ ## __VA_ARGS__.push_back(Idx); \
                    } \
                } \
            }
#define ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_STATIC(...) \
            { \
                const auto& Properties = PropertyList.Properties_ ## __VA_ARGS__; \
                for (U32 Idx = 0; Idx < Properties.size(); ++Idx) \
                { \
                    const auto& Property = Properties[Idx]; \
                    using F_Property = std::remove_const_t<std::remove_reference_t<decltype(Property)>>; \
                    if (Property.IsStatic()) \
                    { \
                        Result.StaticPropertyIndices_ ## __VA_ARGS__.push_back(Idx); \
                    } \
                } \
            }
#define ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_NAME_TO_PROPERTY_INDEX_STATIC(...) \
            { \
                const auto& Properties = PropertyList.Properties_ ## __VA_ARGS__; \
                for (U32 Idx = 0; Idx < Properties.size(); ++Idx) \
                { \
                    const auto& Property = Properties[Idx]; \
                    using F_Property = std::remove_const_t<std::remove_reference_t<decltype(Property)>>; \
                    ABYTEK_ENGINE_NFC_ASSERT(!Result.Has_ ## __VA_ARGS__(Property.Name)) << "Material property name duplication: " << Property.Name; \
                    Result.PropertyNameToPropertyIndex_ ## __VA_ARGS__[Property.Name] = Idx; \
                } \
            }
        
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_DYNAMIC(B8);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_DYNAMIC(U32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_DYNAMIC(I32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_DYNAMIC(F32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_DYNAMIC(Texture);
        
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_STATIC(B8);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_STATIC(U32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_STATIC(I32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_STATIC(F32);
        
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_NAME_TO_PROPERTY_INDEX_STATIC(B8);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_NAME_TO_PROPERTY_INDEX_STATIC(U32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_NAME_TO_PROPERTY_INDEX_STATIC(I32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_NAME_TO_PROPERTY_INDEX_STATIC(F32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_NAME_TO_PROPERTY_INDEX_STATIC(Texture);
    
#undef ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_NAME_TO_PROPERTY_INDEX_STATIC
#undef ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES_STATIC
#undef ABYTEK_INTERNAL_ADD_MATERIAL_PROPERTY_INDICES
        
        return ABYTEK_MOVE(Result);
    }

    ABYTEK_REFLECT(A_MaterialPropertyInstanceBase)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_MaterialPropertyInstanceBase"));   
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Name);
    }
    
    ABYTEK_REFLECT(F_MaterialPropertyInstanceTexture)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialPropertyInstanceTexture"));    
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Value);
    }
    
    ABYTEK_REFLECT(F_MaterialPropertyInstanceList)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialPropertyInstanceList"));      
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_B8);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_U32);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_I32);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_F32);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Properties_Texture);
    }

    F_MaterialPermutationHashCode F_MaterialPropertyInstanceList::CalculateHashCode(
        const F_MaterialPropertyList& PropertyList,
        const F_MaterialPropertyListLayout& PropertyListLayout
    ) const
    {
        F_MaterialPermutationHashCode Result = 0;
#define ABYTEK_INTERNAL_ADD_MATERIAL_PROP_INST_PERMUTATION_HASH_CODE(...) \
            { \
                const auto& Properties_Instance = Properties_ ## __VA_ARGS__; \
                const auto& Properties = PropertyList.Properties_ ## __VA_ARGS__; \
                for (U32 Idx = 0; Idx < Properties.size(); ++Idx) \
                { \
                    const auto& Property_Instance = Properties_Instance[Idx]; \
                    auto PropertyIndex = PropertyListLayout.GetIndex_ ## __VA_ARGS__(Property_Instance.Name); \
                    const auto& Property = Properties[PropertyIndex]; \
                    using F_Property_Instance = std::remove_const_t<std::remove_reference_t<decltype(Property_Instance)>>; \
                    using F_Property = std::remove_const_t<std::remove_reference_t<decltype(Property)>>; \
                    if (!Property.IsStatic()) \
                    { \
                        continue; \
                    } \
                    boost::hash_combine(Result, F_Property::GetValueHashCode(Property_Instance.Value)); \
                } \
            }
        
        ABYTEK_INTERNAL_ADD_MATERIAL_PROP_INST_PERMUTATION_HASH_CODE(B8);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROP_INST_PERMUTATION_HASH_CODE(U32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROP_INST_PERMUTATION_HASH_CODE(I32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PROP_INST_PERMUTATION_HASH_CODE(F32);
    
#undef ABYTEK_INTERNAL_ADD_MATERIAL_PROP_INST_PERMUTATION_HASH_CODE
        return Result;
    }

    F_MaterialPropertyInstanceList H_MaterialProperty::GenerateDefaultInstance(const F_MaterialPropertyList& PropertyList)
    {
        F_MaterialPropertyInstanceList Result;
        for (const auto& Property : PropertyList.Properties_B8)
        {
            Result.Properties_B8.push_back(
                TF_MaterialPropertyInstanceScalar<B8>::Make(Property)  
            );
        }
        for (const auto& Property : PropertyList.Properties_U32)
        {
            Result.Properties_U32.push_back(
                TF_MaterialPropertyInstanceScalar<U32>::Make(Property)  
            );
        }
        for (const auto& Property : PropertyList.Properties_I32)
        {
            Result.Properties_I32.push_back(
                TF_MaterialPropertyInstanceScalar<I32>::Make(Property)  
            );
        }
        for (const auto& Property : PropertyList.Properties_F32)
        {
            Result.Properties_F32.push_back(
                TF_MaterialPropertyInstanceScalar<F32>::Make(Property)  
            );
        }
        for (const auto& Property : PropertyList.Properties_Texture)
        {
            Result.Properties_Texture.push_back(
                F_MaterialPropertyInstanceTexture::Make(Property)  
            );
        }
        return ABYTEK_MOVE(Result);
    }
    TF_Vector<F_MaterialPropertyInstanceList> H_MaterialProperty::GeneratePermutations(const F_MaterialPropertyList& PropertyList)
    {
        TF_Vector<F_MaterialPropertyInstanceList> Result;
        
        F_MaterialPropertyInstanceList DefaultInstance = GenerateDefaultInstance(PropertyList);
        Result.push_back(DefaultInstance);
        
#define ABYTEK_INTERNAL_ADD_MATERIAL_PERMUTATIONS(...) \
            { \
                const auto& Properties = PropertyList.Properties_ ## __VA_ARGS__; \
                for (U32 Idx = 0; Idx < Properties.size(); ++Idx) \
                { \
                    const auto& Property = Properties[Idx]; \
                    using F_Property = std::remove_const_t<std::remove_reference_t<decltype(Property)>>; \
                    if (!Property.IsStatic()) \
                    { \
                        continue; \
                    } \
                     \
                    TF_Vector<F_MaterialPropertyInstanceList> BaseInstances = Result; \
                    TF_Vector<F_MaterialPropertyInstanceList> NewResult; \
                    for (const auto& PermutationValue : Property.PermutationValues) \
                    { \
                        for (const auto& BaseInstance : BaseInstances) \
                        { \
                            F_MaterialPropertyInstanceList Instance = BaseInstance; \
                            Instance.Properties_ ## __VA_ARGS__[Idx].Value = PermutationValue; \
                            NewResult.push_back(Instance); \
                        } \
                    } \
                    Result = ABYTEK_MOVE(NewResult); \
                } \
            }
        
        ABYTEK_INTERNAL_ADD_MATERIAL_PERMUTATIONS(B8);
        ABYTEK_INTERNAL_ADD_MATERIAL_PERMUTATIONS(U32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PERMUTATIONS(I32);
        ABYTEK_INTERNAL_ADD_MATERIAL_PERMUTATIONS(F32);
    
#undef ABYTEK_INTERNAL_ADD_MATERIAL_PERMUTATIONS
        return ABYTEK_MOVE(Result);
    }
}
