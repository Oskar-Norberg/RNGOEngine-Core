//
// Created by Oskar.Norberg on 2025-11-04.
//

#pragma once

#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Utilities
{
    // Non-lazy loaded singleton base class.
    template<typename T>
    class Singleton
    {
    public:
        explicit Singleton(T* instance)
        {
            SetInstance(instance);
        }

        ~Singleton()
        {
            SetInstance(nullptr);
        }

        static T& GetInstance()
        {
            RNGO_ASSERT(m_instance && "Singleton<T>::GetInstance() called on invalid instance.");
            return *m_instance;
        }

    protected:
        void SetInstance(T* instance)
        {
            if (m_instance != nullptr && instance != nullptr)
            {
                RNGO_ASSERT(false && "Singleton<T>::SetInstance() called when instance already exists.");
                return;
            }
            m_instance = instance;
        }

    private:
        static T* m_instance;
    };

    template<typename T>
    T* Singleton<T>::m_instance = nullptr;
}