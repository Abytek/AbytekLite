#include "Abytek/Assimp.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Abytek
{
    F_FeedbackStatus H_Assimp::Decode(const TF_Span<const U8>& Bytes, TF_Vector<F_AssimpSimpleMeshData>& OutDataList)
    {
        Assimp::Importer importer;

        const aiScene* Scene = importer.ReadFileFromMemory(
            Bytes.data(),
            Bytes.size(),
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace
        );
        if (!Scene)
        {
            if (auto ErrorString = importer.GetErrorString())
            {
                return F_FeedbackStatus::MakeFailed(
                    ToText(ErrorString)    
                );
            }
            return F_FeedbackStatus::MakeFailed();
        }

        TF_Vector<F_AssimpSimpleMeshData> SimpleDataList;
        for (unsigned int i = 0; i < Scene->mNumMeshes; ++i)
        {
            F_AssimpSimpleMeshData SimpleData;
            const aiMesh* mesh = Scene->mMeshes[i];

            for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
            {
                F_Vector3_F32 VertexPosition = F_Vector3_F32::Zero();
                F_Vector3_F32 VertexNormal = F_Vector3_F32::Up();
                F_Vector4_F32 VertexTangentAndSign = F_Vector4_F32 {
                    F_Vector3_F32::Right(),
                    1.0f
                };
                F_Vector3_F32 VertexBitangent = F_Vector3_F32::Forward();
                F_Vector2_F32 VertexUV = F_Vector2_F32::Zero();
                
                {
                    const aiVector3D& p = mesh->mVertices[v];
                    VertexPosition.X = p.x;
                    VertexPosition.Y = p.y;
                    VertexPosition.Z = p.z;
                }

                if (mesh->HasNormals())
                {
                    const aiVector3D& n = mesh->mNormals[v];
                    VertexNormal.X = n.x;
                    VertexNormal.Y = n.y;
                    VertexNormal.Z = n.z;
                }

                if (mesh->HasTangentsAndBitangents())
                {
                    const aiVector3D& t = mesh->mTangents[v];
                    const aiVector3D& b = mesh->mBitangents[v];
                    VertexTangentAndSign.X = t.x;
                    VertexTangentAndSign.Y = t.y;
                    VertexTangentAndSign.Z = t.z;
                    VertexBitangent.X = b.x;
                    VertexBitangent.Y = b.y;
                    VertexBitangent.Z = b.z;
                    VertexTangentAndSign.W = (
                        (
                            Dot(
                                Cross(
                                    VertexTangentAndSign.StaticPermute<0, 1, 2>(), 
                                    VertexNormal), 
                                VertexBitangent
                            ) 
                            < 0.0f
                        ) 
                        ? -1.0f 
                        : 1.0f
                    );
                }

                if (mesh->HasTextureCoords(0))
                {
                    const aiVector3D& uv = mesh->mTextureCoords[0][v];
                    VertexUV.X = uv.x;
                    VertexUV.Y = uv.y;
                }
                
                SimpleData.Positions.push_back(VertexPosition);
                SimpleData.Normals.push_back(VertexNormal);
                SimpleData.TangentsAndSigns.push_back(VertexTangentAndSign);
                SimpleData.UVs.push_back(VertexUV);
            }

            for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
            {
                const aiFace& face = mesh->mFaces[f];
                if (face.mNumIndices != 3)
                {
                    return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Internal simple static mesh decode bug in face indices"));
                }
                for (unsigned int j = 0; j < face.mNumIndices; ++j)
                {
                    uint32_t index = face.mIndices[j];
                    SimpleData.Indices.push_back(index);
                }
            }
            SimpleDataList.push_back(ABYTEK_MOVE(SimpleData));
        }
        OutDataList = ABYTEK_MOVE(SimpleDataList);
        return F_FeedbackStatus::MakeSucceeded();
    }
}
