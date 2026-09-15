#pragma once

#include "Abytek/Base.Console.prerequisites.pch.hpp"


namespace Abytek
{
    class F_Console;
    
    namespace Console
    {
        static F_Text GetBundledConfigsDirectoryRelativePath()
        {
            return ABYTEK_TEXT("Abytek/Configs");
        }
    }
    
    class A_ConsoleVariable : public A_Object
    {
        
    private:
        TW<F_Console> _Console;
        F_Name _Name;
        F_Text _Description;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetConsole() const noexcept
        {
            return _Console;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetDescription() const noexcept
        {
            return _Description;
        }
        
    public:
        A_ConsoleVariable(const TW_Valid<F_Console>& Console, const F_Name& Name, const F_Text& Description);
        virtual ~A_ConsoleVariable() override;
        
    public:
        void ParseConfig(const TF_Vector<toml::parse_result>& ParsedResults);
        virtual void ParseConfig(const toml::node& Node) = 0;
        
    public:
        virtual void AddCLIOption(cxxopts::Options& Options) = 0;
        virtual void TryEvaluateCLIOption(const cxxopts::ParseResult& ParsedResult) = 0;
    };
    
    namespace Internal::ConsoleVariable
    {
        template<auto __IsArray, typename __F_Value>
        struct TH_ConsoleVariable;
        
        template<typename __F>
        struct TH_ConsoleVariable<false, __F>
        {
            static_assert(std::is_enum_v<__F>, "Invalid value type");
            
            using F_Value = __F;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return (__F)(*Node.value<I64>());
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<I64>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = (__F)(ParsedResult[Key].as<I64>());
            }
        };
        template<>
        struct TH_ConsoleVariable<false, B8>
        {
            using F_Value = B8;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<B8>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<B8>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<B8>()
                        ->default_value("false")
                        ->implicit_value("true")
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<B8>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, U8>
        {
            using F_Value = U8;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<U8>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<U8>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, U16>
        {
            using F_Value = U16;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<U16>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<U16>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, U32>
        {
            using F_Value = U32;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<U32>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<U32>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, U64>
        {
            using F_Value = U64;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<U64>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<U64>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, I8>
        {
            using F_Value = I8;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<I8>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<I8>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, I16>
        {
            using F_Value = I16;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<I16>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<I16>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, I32>
        {
            using F_Value = I32;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<I32>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<I32>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, I64>
        {
            using F_Value = I64;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<I64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<I64>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<I64>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, F_SzForSpecifications>
        {
            using F_Value = F_SzForSpecifications;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return { static_cast<Sz>(*Node.value<I64>()) };
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<Sz>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = { ParsedResult[Key].as<Sz>() };
            }
        };
        template<>
        struct TH_ConsoleVariable<false, F_PDiffForSpecifications>
        {
            using F_Value = F_PDiffForSpecifications;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<I64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return { static_cast<PDiff>(*Node.value<I64>()) };
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<PDiff>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = { ParsedResult[Key].as<PDiff>() };
            }
        };
        template<>
        struct TH_ConsoleVariable<false, F32>
        {
            using F_Value = F32;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<F64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<F64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<F32>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<F32>();
            }
        };
        template<>
        struct TH_ConsoleVariable<false, F64>
        {
            using F_Value = F64;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<F64>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return *Node.value<F64>();
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<F64>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ParsedResult[Key].as<F64>();
            }
        };
        template<typename __F_Char, class __F_Traits, class __F_Allocator>
        struct TH_ConsoleVariable<false, boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>>
        {
            using F_Value = boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<std::string>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return ToString<__F_Char, const F_Char*>(Node.value<std::string>()->data());
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::string>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = ToString<__F_Char, const F_Char*>(ParsedResult[Key].as<std::string>().c_str());
            }
        };
        template<>
        struct TH_ConsoleVariable<false, F_Name>
        {
            using F_Value = F_Name;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<std::string>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                return F_Name(ToString<F_TextChar, const F_Char*>(Node.value<std::string>()->data()));
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::string>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                OutValue = F_Name(ToString<F_TextChar, const F_Char*>(ParsedResult[Key].as<std::string>().c_str()));
            }
        };
        template<typename __F_Type>
        struct TH_ConsoleVariable<false, TF_ReflectionTypeHandle<__F_Type>>
        {
            using F_Value = TF_ReflectionTypeHandle<__F_Type>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is<std::string>())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Name ItemSearchKey = ToString<F_TextChar, const F_Char*>(Node.value<std::string>()->data());
                if (ItemSearchKey)
                {
                    auto ReflectionType = GlobalSearchReflectionItem<F_ReflectionType>(ItemSearchKey);
                    ABYTEK_BASE_CONSOLE_ASSERT(ReflectionType) << "Invalid reflection type: " << ItemSearchKey;
                    return ReflectionType;
                }
                else
                {
                    return {};
                }
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::string>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Name ItemSearchKey = ToString<F_TextChar, const F_Char*>(ParsedResult[Key].as<std::string>().c_str());
                if (ItemSearchKey)
                {
                    auto ReflectionType = GlobalSearchReflectionItem<F_ReflectionType>(ItemSearchKey);
                    ABYTEK_BASE_CONSOLE_ASSERT(ReflectionType) << "Invalid reflection type: " << ItemSearchKey;
                    OutValue = ReflectionType;
                }
                else
                {
                    OutValue = {};
                }
            }
        };
        
        template<typename __F>
        struct TH_ConsoleVariableArray
        {
            static_assert(std::is_enum_v<__F>, "Invalid value type");
            
            using F_Item = __F;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<I64>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<B8>
        {
            using F_Item = B8;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<B8>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<B8>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<B8>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<B8>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<U8>
        {
            using F_Item = U8;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<U16>
        {
            using F_Item = U16;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<U32>
        {
            using F_Item = U32;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<U64>
        {
            using F_Item = U64;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<I8>
        {
            using F_Item = I8;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<I16>
        {
            using F_Item = I16;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<I32>
        {
            using F_Item = I32;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<I64>
        {
            using F_Item = I64;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<I64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<F_SzForSpecifications>
        {
            using F_Item = F_SzForSpecifications;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back({ (Sz)(*Element.value<I64>()) });
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<Sz>>())
                {
                    Result.push_back({ (Sz)Element });
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<F_PDiffForSpecifications>
        {
            using F_Item = F_PDiffForSpecifications;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<I64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back({ (PDiff)(*Element.value<I64>()) });
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<I64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<PDiff>>())
                {
                    Result.push_back({ (PDiff)Element });
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<F32>
        {
            using F_Item = F32;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<F64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<F64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<F64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<F64>
        {
            using F_Item = F64;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<F64>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back((F_Item)(*Element.value<F64>()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<F64>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<F_Item>>())
                {
                    Result.push_back((F_Item)Element);
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<typename __F_Char, class __F_Traits, class __F_Allocator>
        struct TH_ConsoleVariableArray<boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>>
        {
            using F_Item = boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<std::string>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back(ToString<__F_Char, const F_Char*>(Element.value<std::string>()->data()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<std::string>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<std::string>>())
                {
                    Result.push_back(ToString<__F_Char, const F_Char*>(Element.c_str()));
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<>
        struct TH_ConsoleVariableArray<F_Name>
        {
            using F_Item = F_Name;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<std::string>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    Result.push_back(ToString<F_TextChar, const F_Char*>(Element.value<std::string>()->data()));
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<std::string>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<std::string>>())
                {
                    Result.push_back(ToString<F_TextChar, const F_Char*>(Element.c_str()));
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        template<typename __F_Type>
        struct TH_ConsoleVariableArray<TF_ReflectionTypeHandle<__F_Type>>
        {
            using F_Item = TF_ReflectionTypeHandle<__F_Type>;
            using F_Value = TF_Vector<F_Item>;
            
            static F_Value ParseConfig(const toml::node& Node, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                if  (!Node.is_array())
                {
                    ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") type mismatch, requires type: " << TypeFullName<F_Value>();
                }
                F_Value Result;
                for (const auto& Element : *Node.as_array())
                {
                    if  (!Element.is<std::string>())
                    {
                        ABYTEK_LOG_FATAL() << "Console variable (" << Variable->GetName() << ") item type mismatch, requires item type: " << TypeFullName<F_Item>();
                    }
                    F_Name ItemSearchKey = ToString<F_TextChar, const F_Char*>(Element.value<std::string>()->data());
                    if (ItemSearchKey)
                    {
                        auto ReflectionType = GlobalSearchReflectionItem<F_ReflectionType>(ItemSearchKey);
                        ABYTEK_BASE_CONSOLE_ASSERT(ReflectionType) << "Invalid reflection type: " << ItemSearchKey;
                        Result.push_back(ReflectionType);
                    }
                    else
                    {
                        Result.push_back({});
                    }
                }
                return ABYTEK_MOVE(Result);
            }
            
            static void AddCLIOption(cxxopts::Options& Options, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                Options.add_options()(
                    ToString(*Variable->GetName()).c_str(),
                    ToString(Variable->GetDescription()).c_str(),
                    cxxopts::value<std::vector<std::string>>()
                );
            }
            static void TryEvaluateCLIOption(F_Value& OutValue, const cxxopts::ParseResult& ParsedResult, const TW_Valid<A_ConsoleVariable>& Variable)
            {
                std::string Key = ToString(*Variable->GetName()).c_str();
                if (ParsedResult.count(Key) == 0)
                {
                    return;
                }
                F_Value Result;
                for (const auto& Element : ParsedResult[Key].as<std::vector<std::string>>())
                {
                    F_Name ItemSearchKey = ToString<F_TextChar, const F_Char*>(Element.c_str());
                    if (ItemSearchKey)
                    {
                        auto ReflectionType = GlobalSearchReflectionItem<F_ReflectionType>(ItemSearchKey);
                        ABYTEK_BASE_CONSOLE_ASSERT(ReflectionType) << "Invalid reflection type: " << ItemSearchKey;
                        Result.push_back(ReflectionType);
                    }
                    else
                    {
                        Result.push_back({});
                    }
                }
                OutValue = ABYTEK_MOVE(Result);
            }
        };
        
        template<typename __F_Value>
        struct TH_ConsoleVariable<true, TF_Vector<__F_Value>> : TH_ConsoleVariableArray<__F_Value>
        {
        };
        
        template<typename __F>
        struct TH_ConsoleVariableCheckVector
        {
            static constexpr B8 Value = false;
        };
        template<typename __F>
        struct TH_ConsoleVariableCheckVector<TF_Vector<__F>>
        {
            static constexpr B8 Value = true;
        };
    }
    
    template<typename __F_Value>
    class TF_ConsoleVariable final : public A_ConsoleVariable
    {
    public:
        using F_Value = __F_Value;
        
    private:
        F_Value _Value;
        B8 _CLISet = false;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetValue() const noexcept
        {
            return _Value;
        }
        ABYTEK_FORCE_INLINE auto& GetValue() noexcept
        {
            return _Value;
        }
        ABYTEK_FORCE_INLINE void SetValue(const F_Value& X) noexcept
        {
            _Value = X;
        }
        
    public:
        TF_ConsoleVariable(const TW_Valid<F_Console>& Console, const F_Name& Name, const F_Text& Description, const __F_Value& DefaultValue) :
            A_ConsoleVariable(Console, Name, Description),
            _Value(DefaultValue)
        {
        }
        virtual ~TF_ConsoleVariable() override
        {
        }
        
    public:
        virtual void ParseConfig(const toml::node& Node) override
        {
            if (_CLISet)
            {
                return;
            }
            _Value = Internal::ConsoleVariable::TH_ConsoleVariable<
                Internal::ConsoleVariable::TH_ConsoleVariableCheckVector<__F_Value>::Value,
                __F_Value
            >::ParseConfig(Node, ABYTEK_WTHIS());
        }
        virtual void AddCLIOption(cxxopts::Options& Options) override
        {
             Internal::ConsoleVariable::TH_ConsoleVariable<
                Internal::ConsoleVariable::TH_ConsoleVariableCheckVector<__F_Value>::Value,
                __F_Value
            >::AddCLIOption(Options, ABYTEK_WTHIS());
        }
        virtual void TryEvaluateCLIOption(const cxxopts::ParseResult& ParsedResult) override
        {
             Internal::ConsoleVariable::TH_ConsoleVariable<
                Internal::ConsoleVariable::TH_ConsoleVariableCheckVector<__F_Value>::Value,
                __F_Value
            >::TryEvaluateCLIOption(_Value, ParsedResult, ABYTEK_WTHIS());
        }
    };
    
    struct F_ConsoleData
    {
        struct F_CLI
        {
            cxxopts::ParseResult ParseResult;
        };
        struct F_Config
        {
            TF_Vector<toml::parse_result> ParsedResults;
        };
        
        F_CLI CLI;
        F_Config Config;
    };
    
    class F_Console : public A_Object
    {
    private:
        TF_Map<F_Name, TS<A_ConsoleVariable>> _Variables;
        TF_Map<F_Text, F_Text> _ConfigFiles;
        
        F_ConsoleData _Data;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetVariables() const noexcept
        {
            return _Variables;
        }
        ABYTEK_FORCE_INLINE auto HasVariable(const F_Name& Name) const noexcept
        {
            return _Variables.find(Name) != _Variables.end();
        }
        ABYTEK_FORCE_INLINE TS<A_ConsoleVariable> FindVariable(const F_Name& Name) const noexcept
        {
            auto It = _Variables.find(Name);
            if (It == _Variables.end())
            {
                return {};
            }
            return It->second;
        }
        ABYTEK_FORCE_INLINE const auto& GetVariable(const F_Name& Name) const noexcept
        {
            auto It = _Variables.find(Name);
            ABYTEK_BASE_CONSOLE_ASSERT(It != _Variables.end()) << "Not found variable: " << *Name;
            return It->second;
        }
        template<typename __F_Value>
        ABYTEK_FORCE_INLINE auto HasVariable(const F_Name& Name) const noexcept
        {
            auto It = _Variables.find(Name);
            if (It != _Variables.end())
            {
                return It->second.CheckPolymorphism<TF_ConsoleVariable<__F_Value>>();
            }
            return false;
        }
        template<typename __F_Value>
        ABYTEK_FORCE_INLINE TS<TF_ConsoleVariable<__F_Value>> FindVariable(const F_Name& Name) const noexcept
        {
            auto It = _Variables.find(Name);
            if (It == _Variables.end())
            {
                return {};
            }
            return It->second.FastCast<TF_ConsoleVariable<__F_Value>>();
        }
        template<typename __F_Value>
        ABYTEK_FORCE_INLINE TS<TF_ConsoleVariable<__F_Value>> GetVariable(const F_Name& Name) const noexcept
        {
            auto It = _Variables.find(Name);
            ABYTEK_BASE_CONSOLE_ASSERT(It != _Variables.end()) << "Not found variable: " << *Name;
            return It->second.FastCast<TF_ConsoleVariable<__F_Value>>();
        }
        ABYTEK_FORCE_INLINE const auto& GetConfigFiles() const noexcept
        {
            return _ConfigFiles;
        }
        ABYTEK_FORCE_INLINE auto HasConfigFile(const F_Text& ConfigFilePath) const noexcept
        {
            return _ConfigFiles.find(H_Path::Normalize(ConfigFilePath)) != _ConfigFiles.end();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetData() const noexcept
        {
            return _Data;
        }
        
    public:
        F_Console();
        virtual ~F_Console() override;
        
    private:
        void _RefreshCLIData(F_ConsoleData::F_CLI& CLIData);
        void _RefreshConfigData(F_ConsoleData::F_Config& ConfigData);
        
    public:
        void Update(B8 RefreshCLI = true, B8 RefreshConfig = true);
        
    public:
        template<typename __F_Value>
        TW<TF_ConsoleVariable<__F_Value>> RegisterVariable(const F_Name& Name, const F_Text& Description, const __F_Value& DefaultValue) noexcept
        {
            ABYTEK_BASE_CONSOLE_ASSERT(!HasVariable(Name)) << "Variable was already added with name: " << Name;
            auto Variable = TS<TF_ConsoleVariable<__F_Value>>()(ABYTEK_WTHIS(), Name, Description, DefaultValue);
            _Variables[Name] = Variable;
            return Variable.Weak();
        }
        void UnregisterVariable(const F_Name& Name) noexcept
        {
            ABYTEK_BASE_CONSOLE_ASSERT(HasVariable(Name)) << "Variable with name \"" << Name << "\" was not added";
            _Variables.erase(_Variables.find(Name));
        }
        
    public:
        void AddConfigFile(const F_Text& ConfigFilePath) noexcept
        {
            auto NormalizedConfigFilePath = H_Path::Normalize(ConfigFilePath);
            ABYTEK_BASE_CONSOLE_ASSERT(!HasConfigFile(NormalizedConfigFilePath)) << "Config file was already added: " << NormalizedConfigFilePath;
            F_Text ConfigFileContent;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::ReadFileText(NormalizedConfigFilePath, ConfigFileContent)    
            );
            _ConfigFiles[NormalizedConfigFilePath] = ConfigFileContent;
        }
        void RemoveConfigFile(const F_Text& ConfigFilePath) noexcept
        {
            auto NormalizedConfigFilePath = H_Path::Normalize(ConfigFilePath);
            ABYTEK_BASE_CONSOLE_ASSERT(HasConfigFile(NormalizedConfigFilePath)) << "Config file \"" << NormalizedConfigFilePath << "\" was not added";
            _ConfigFiles.erase(_ConfigFiles.find(NormalizedConfigFilePath));
        }
    };
}