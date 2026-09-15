#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/CrtUseObject.hpp"


namespace Abytek
{
    namespace Internal
    {
        template<
            typename __F,
            typename __F_Base,
            bool __F_IsAbstract,
            typename... __F_Args
        >
        struct TF_UniqueObjectFactory;

        template<
            typename __F,
            typename __F_Base,
            typename... __F_Args
        >
        struct TF_UniqueObjectFactory<
            __F,
            __F_Base,
            false,
            __F_Args...
        >
        {
            static TU<__F_Base> Create(__F_Args... Args)
            {
                return TU<__F>()(
                    ABYTEK_FORWARD(Args)...
                );
            }
        };

        template<
            typename __F,
            typename __F_Base,
            typename... __F_Args
        >
        struct TF_UniqueObjectFactory<
            __F,
            __F_Base,
            true,
            __F_Args...
        >
        {
            static TU<__F_Base> Create(__F_Args...)
            {
                ABYTEK_BASE_OBJECT_ASSERT(false)
                    << "Cannot create abstract reflected type";

                return {};
            }
        };



        template<
            typename __F,
            typename __F_Base,
            bool __F_IsAbstract,
            typename... __F_Args
        >
        struct TF_SharedObjectFactory;

        template<
            typename __F,
            typename __F_Base,
            typename... __F_Args
        >
        struct TF_SharedObjectFactory<
            __F,
            __F_Base,
            false,
            __F_Args...
        >
        {
            static TS<__F_Base> Create(__F_Args... Args)
            {
                return TS<__F>()(
                    ABYTEK_FORWARD(Args)...
                );
            }
        };

        template<
            typename __F,
            typename __F_Base,
            typename... __F_Args
        >
        struct TF_SharedObjectFactory<
            __F,
            __F_Base,
            true,
            __F_Args...
        >
        {
            static TS<__F_Base> Create(__F_Args...)
            {
                ABYTEK_BASE_OBJECT_ASSERT(false)
                    << "Cannot create abstract reflected type";

                return {};
            }
        };



        template<
            typename __F,
            typename __F_Base,
            bool __F_IsAbstract,
            typename... __F_Args
        >
        struct TF_RAObjectFactory;

        template<
            typename __F,
            typename __F_Base,
            typename... __F_Args
        >
        struct TF_RAObjectFactory<
            __F,
            __F_Base,
            false,
            __F_Args...
        >
        {
            static TS<__F_Base> Create(__F_Args... Args)
            {
                return TS<__F>()(
                    ABYTEK_FORWARD(Args)...
                );
            }
        };

        template<
            typename __F,
            typename __F_Base,
            typename... __F_Args
        >
        struct TF_RAObjectFactory<
            __F,
            __F_Base,
            true,
            __F_Args...
        >
        {
            static TS<__F_Base> Create(__F_Args...)
            {
                ABYTEK_BASE_OBJECT_ASSERT(false)
                    << "Cannot create abstract reflected type";

                return {};
            }
        };
    }
    
    template<typename __F_Base, typename... __F_Args>
    struct TF_UniqueObjectCreator
    {
        template<typename __F>
        static constexpr B8 Check()
        {
            return std::is_constructible_v<__F, __F_Args...>;
        }
        
        TF_Function<TU<__F_Base>(__F_Args...)> Functor;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return static_cast<B8>(Functor);
        }
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return IsValid();
        }

        template<typename __F>
        static TF_UniqueObjectCreator Make()
        {
            return {
                [](__F_Args... Args) -> TU<__F_Base>
                {
                    return Internal::TF_UniqueObjectFactory<
                        __F,
                        __F_Base,
                        !Check<__F>(),
                        __F_Args...
                    >::Create(
                        ABYTEK_FORWARD(Args)...
                    );
                }
            };
        }
        template<typename __F, typename __F_PostProcess>
        static TF_UniqueObjectCreator Make(__F_PostProcess&& PostProcess)
        {
            return {
                [CachedPostProcess = ABYTEK_MOVE(PostProcess)](__F_Args... Args) -> TU<__F_Base>
                {
                    auto Result = Internal::TF_UniqueObjectFactory<
                        __F,
                        __F_Base,
                        !Check<__F>(),
                        __F_Args...
                    >::Create(
                        ABYTEK_FORWARD(Args)...
                    );
                    CachedPostProcess(Result);
                    return ABYTEK_MOVE(Result);
                }
            };
        }

        TU<__F_Base> operator()(__F_Args... Args) const
        {
            return Functor(
                ABYTEK_FORWARD(Args)...
            );
        }
    };
    
    template<typename __F_Base, typename... __F_Args>
    struct TF_SharedObjectCreator
    {
        template<typename __F>
        static constexpr B8 Check()
        {
            return std::is_constructible_v<__F, __F_Args...>;
        }
        
        TF_Function<TS<__F_Base>(__F_Args...)> Functor;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return static_cast<B8>(Functor);
        }
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return IsValid();
        }

        template<typename __F>
        static TF_SharedObjectCreator Make()
        {
            return {
                [](__F_Args... Args) -> TS<__F_Base>
                {
                    return Internal::TF_SharedObjectFactory<
                        __F,
                        __F_Base,
                        !Check<__F>(),
                        __F_Args...
                    >::Create(
                        ABYTEK_FORWARD(Args)...
                    );
                }
            };
        }
        template<typename __F, typename __F_PostProcess>
        static TF_SharedObjectCreator Make(__F_PostProcess&& PostProcess)
        {
            return {
                [CachedPostProcess = ABYTEK_MOVE(PostProcess)](__F_Args... Args) -> TS<__F_Base>
                {
                    auto Result = Internal::TF_SharedObjectFactory<
                        __F,
                        __F_Base,
                        !Check<__F>(),
                        __F_Args...
                    >::Create(
                        ABYTEK_FORWARD(Args)...
                    );
                    CachedPostProcess(Result);
                    return ABYTEK_MOVE(Result);
                }
            };
        }

        TS<__F_Base> operator()(__F_Args... Args) const
        {
            return Functor(
                ABYTEK_FORWARD(Args)...
            );
        }
    };
}