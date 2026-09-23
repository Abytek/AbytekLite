module DeclarePages;

#include "Abytek/Renderer/RenderGeometry/Macros.slangh"

namespace Abytek
{
#ifdef ABYTEK_RENDER_GEOMETRY_ENABLE_WRITE_PAGES
    public typealias F_DeclaredRenderGeometryPage = RWByteAddressBuffer;
#else
    public typealias F_DeclaredRenderGeometryPage = ByteAddressBuffer;
#endif
}

#ifdef ABYTEK_RENDER_GEOMETRY_ENABLE_MULTIPLE_PAGES
public Abytek::F_DeclaredRenderGeometryPage ABYTEK_RENDER_GEOMETRY_GET_RESOURCE()[];
#else
public Abytek::F_DeclaredRenderGeometryPage ABYTEK_RENDER_GEOMETRY_GET_RESOURCE();
#endif