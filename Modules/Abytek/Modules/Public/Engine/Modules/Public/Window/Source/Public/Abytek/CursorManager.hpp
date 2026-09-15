#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"
#include "Abytek/Cursor.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API A_CursorManager : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_CursorManager);
        
    public:
        static F_Name GetUpdateUpdateFunctionName();
        static F_Name GetPostUpdateUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        TF_Set<TS<A_Cursor>> _Cursors;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCursors() const noexcept
        {
            return _Cursors;
        }
        ABYTEK_FORCE_INLINE auto& InjectCursors() noexcept
        {
            return _Cursors;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_CursorManager);
        virtual void Build();
        virtual void Release() override;

    public:
        virtual void Startup();
        virtual void Shutdown();
        
    protected:
        TW_Valid<A_Cursor> CreateCursor();
        void DestroyCursor(const TW_Valid<A_Cursor>& Cursor);
        
    protected:
        virtual void OnUpdate();
        virtual void OnPostUpdate();
        
    public:
        TW<A_Cursor> GetMainCursor() const;
    };
}
