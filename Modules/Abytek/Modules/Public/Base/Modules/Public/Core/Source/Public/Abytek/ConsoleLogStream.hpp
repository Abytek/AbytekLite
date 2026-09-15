#pragma once

#include "Abytek/TextLogStream.hpp"


namespace Abytek
{
    class F_ConsoleLogStream;
    
    class ABYTEK_BASE_CORE_API A_ConsoleLogAdapter
    {
    public:
        A_ConsoleLogAdapter();
        virtual ~A_ConsoleLogAdapter();
        
    public:
        virtual void Push(F_ConsoleLogStream& LogStream, const F_TextChar* TextChars);
    };
    
    /**
     * @brief A log stream implementation that writes to the console
     * 
     * Provides a convenient way to log messages to the standard console output.
     * This is a specialization of the StdOutTextStreamLogStream that is pre-configured
     * for console output.
     */
    class ABYTEK_BASE_CORE_API F_ConsoleLogStream : public A_TextLogStream
    {
    private:
        std::set<A_ConsoleLogAdapter*> _Adapters;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAdapters() const noexcept
        {
            return _Adapters;
        }
        
    public:
        /**
         * @brief Constructor for console log stream
         */
        F_ConsoleLogStream();
        
        /**
         * @brief Virtual destructor
         */
        virtual ~F_ConsoleLogStream() override;
        
    public:
        virtual void Push(const F_TextChar* TextChars) override;
        
    public:
        template<typename __F_Adapter, typename... __F_Args>
        __F_Adapter* AddAdapter(__F_Args&&... Args)
        {
            auto Adapter = new __F_Adapter(ABYTEK_FORWARD(Args)...);
            _Adapters.insert(Adapter);
            return Adapter;
        }
        void RemoveAdapter(A_ConsoleLogAdapter* Adapter)
        {
            _Adapters.erase(_Adapters.find(Adapter));
            delete Adapter;
        }
        void RemoveAllAdapters()
        {
            while (_Adapters.size() > 0)
            {
                RemoveAdapter(*_Adapters.begin());
            }
        }
    };

    /**
     * @brief Global instance of the console log stream
     * 
     * This can be used directly for logging to the console without
     * having to create your own instance.
     */
    extern ABYTEK_BASE_CORE_API F_ConsoleLogStream ConsoleLogStream;
}