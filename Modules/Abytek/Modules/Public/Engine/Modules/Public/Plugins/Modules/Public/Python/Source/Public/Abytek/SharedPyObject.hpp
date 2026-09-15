#pragma once

#include "Python.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_PLUGINS_PYTHON_API F_SharedPyObject
    {
    protected:
        PyObject* _Handle;

    public:
        ABYTEK_FORCE_INLINE PyObject* GetHandle() const { return _Handle; }

    public:
        ABYTEK_FORCE_INLINE F_SharedPyObject() noexcept :
            _Handle(Py_None)
        {
            Py_INCREF(_Handle);
        }
        virtual ~F_SharedPyObject()
        {
            Py_DECREF(_Handle);
        }

        ABYTEK_FORCE_INLINE explicit F_SharedPyObject(PyObject* Handle) noexcept :
            _Handle(Handle)
        {
                        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(_Handle) << "invalid handle";
        }

        F_SharedPyObject(const F_SharedPyObject& X) noexcept :
            _Handle(X._Handle)
        {
            Py_INCREF(_Handle);
        }
        F_SharedPyObject& operator = (const F_SharedPyObject& X) noexcept
        {
            _Handle = X._Handle;
            Py_INCREF(_Handle);
            return *this;
        }

        F_SharedPyObject(F_SharedPyObject&& X) noexcept :
            _Handle(X._Handle)
        {
            X._Handle = Py_None;
            X.IncreaseRef();
        }
        F_SharedPyObject& operator = (F_SharedPyObject&& X) noexcept
        {
            _Handle = X._Handle;
            X._Handle = Py_None;
            X.IncreaseRef();
            return *this;
        }

    public:
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept { return _Handle != Py_None; }
        ABYTEK_FORCE_INLINE B8 IsNull() const noexcept { return _Handle == Py_None; }
        ABYTEK_FORCE_INLINE operator B8 () const noexcept { return IsValid(); }

        ABYTEK_FORCE_INLINE PyObject* operator * () const noexcept { return _Handle; }

    public:
        void Reset() noexcept
        {
            Py_DECREF(_Handle);
            _Handle = Py_None;
            Py_INCREF(_Handle);
        }
        void IncreaseRef() noexcept
        {
            Py_INCREF(_Handle);
        }
        void DecreaseRef() noexcept
        {
            Py_DECREF(_Handle);
        }
        template<typename __F>
        ABYTEK_FORCE_INLINE __F Cast() const noexcept
        {
            Py_INCREF(_Handle);
            return __F(_Handle);
        }

    public:
        F_SharedPyObject GetAttribute(const WithCrtAllocator::F_String& Name) const
        {
            return F_SharedPyObject(
                PyObject_GetAttrString(GetHandle(), Name.c_str())
            );
        }
        void SetAttribute(const WithCrtAllocator::F_String& Name, const F_SharedPyObject& Value) const
        {
            PyObject_SetAttrString(GetHandle(), Name.c_str(), Value.GetHandle());
        }
    };
}


#define ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(...) \
            ABYTEK_PUBLIC_KEYWORD \
                ABYTEK_FORCE_INLINE __VA_ARGS__() noexcept = default; \
                \
                ABYTEK_FORCE_INLINE explicit __VA_ARGS__(PyObject* Handle) noexcept : \
                    Abytek::F_SharedPyObject(Handle) \
                { \
                } \
                \
                ABYTEK_FORCE_INLINE __VA_ARGS__(const __VA_ARGS__& X) noexcept : \
                    Abytek::F_SharedPyObject(X) \
                { \
                } \
                ABYTEK_FORCE_INLINE __VA_ARGS__& operator = (const __VA_ARGS__& X) noexcept \
                { \
                    (Abytek::F_SharedPyObject&)(*this) = X; \
                    return *this; \
                } \
                \
                ABYTEK_FORCE_INLINE __VA_ARGS__(__VA_ARGS__&& X) noexcept : \
                    Abytek::F_SharedPyObject(X) \
                { \
                } \
                ABYTEK_FORCE_INLINE __VA_ARGS__& operator = (__VA_ARGS__&& X) noexcept \
                { \
                    (Abytek::F_SharedPyObject&)(*this) = boost::move(X); \
                    return *this; \
                } \
                \
                friend ABYTEK_FORCE_INLINE Abytek::B8 operator == (const __VA_ARGS__& A, const __VA_ARGS__& B) noexcept \
                { \
                    return (A._Handle == B._Handle);\
                } \
                friend ABYTEK_FORCE_INLINE Abytek::B8 operator != (const __VA_ARGS__& A, const __VA_ARGS__& B) noexcept \
                { \
                    return (A._Handle != B._Handle);\
                }