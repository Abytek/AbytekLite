module DeclarePages;

namespace Abytek::NFC::RenderGeometry
{
#ifdef ABYTEK_RENDER_GEOMETRY_ENABLE_WRITE_PAGES
    public typealias F_DeclaredPage = RWByteAddressBuffer;
#else
    public typealias F_DeclaredPage = ByteAddressBuffer;
#endif
}

#ifdef ABYTEK_RENDER_GEOMETRY_ENABLE_MULTIPLE_PAGES
public Abytek::NFC::RenderGeometry::F_DeclaredPage RenderGeometryPages[];
#else
public Abytek::NFC::RenderGeometry::F_DeclaredPage RenderGeometryPage_Single;
#endif