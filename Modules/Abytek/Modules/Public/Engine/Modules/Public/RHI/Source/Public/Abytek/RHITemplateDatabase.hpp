#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIFeature.hpp"
#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHIAPI.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_RHITemplateDatabase : public A_Object
    {
    public:
        friend class A_RHITemplate;
        
    private:
        E_RHIAPI _API = E_RHIAPI::NONE;
        F_RHIFeatureSupports _FeatureSupports;

        F_SpinLock _SpinLock;
        TF_Map<F_RHITemplateHashCode, TW<A_RHITemplate>> _Templates;
        TF_Set<F_RHITemplateHashCode> _LockedHashCodes;
        
    public:
        ABYTEK_FORCE_INLINE const auto GetAPI() const noexcept
        {
            return _API;
        }
        ABYTEK_FORCE_INLINE const auto& GetFeatureSupports() const noexcept
        {
            return _FeatureSupports;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplates() const noexcept
        {
            return _Templates;
        }

    protected:
        A_RHITemplateDatabase(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports);
        
    public:
        ~A_RHITemplateDatabase() override;

    private:
        void _TrackTemplate(const TW_Valid<A_RHITemplate>& Template);
        void _UntrackTemplate(const TW_Valid<A_RHITemplate>& Template);

    public:
        static TU<A_RHITemplateDatabase> Create(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports);

    public:
        E_RHITemplateHashCodeLockStatus LockHashCode(
            F_RHITemplateHashCode HashCode,
            TS<A_RHITemplate>& OutTemplateIfCreated
        );
        void UnlockHashCode(F_RHITemplateHashCode HashCode);
        template<typename __F_CallbackToCreateTemplate>
        TS<A_RHITemplate> LockedCreateTemplate(
            F_RHITemplateHashCode HashCode,
            __F_CallbackToCreateTemplate&& CallbackToCreateTemplate
        )
        {
            TS<A_RHITemplate> Template;
            E_RHITemplateHashCodeLockStatus LockStatus;
            while (true)
            {
                LockStatus = LockHashCode(HashCode, Template);
                if (LockStatus == E_RHITemplateHashCodeLockStatus::CREATED)
                {
                    UnlockHashCode(HashCode);
                    return Template;
                }
                if (LockStatus == E_RHITemplateHashCodeLockStatus::NEED_TO_CREATE)
                {
                    Template = CallbackToCreateTemplate();
                    UnlockHashCode(HashCode);
                    return Template;
                }
            }
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown error";
            return {};
        }

    public:
        TS_Valid<A_RHITemplate> GetTemplate(F_RHITemplateHashCode HashCode);
        TS<A_RHITemplate> FindTemplate(F_RHITemplateHashCode HashCode);
        B8 HasTemplate(F_RHITemplateHashCode HashCode);
        
    public:
        TF_Vector<TS<A_RHITemplate>> GatherTemplates();
        TF_Vector<F_RHITemplateHashCode> GatherTemplateHashCodes();
    };
}
