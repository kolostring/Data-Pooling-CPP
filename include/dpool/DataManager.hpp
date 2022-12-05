#pragma once
#include "DataPool.hpp"

/// @brief Manager class for handling behavior of a DataPool.
///
///     Has the responsibility to update DataPool's values and notify to other managers of it's
///     changes (Observer Pattern, out of scope for this project right now).
/// @tparam T class type
template<class T>
class DataManager{
    private:
        /// @brief Data Pool
        DataPool<T> m_dataPool;

        /// @brief Pointer to a function for updating values of the DataPool
        /// @param delta time passed since the last update
        /// @param dataManager reference to a DataManager
        void (*m_updateCallback) (float, DataManager<T>&);
    
    public:

        /// @brief Class Constructor
        /// @param _maxSize max size the pool can have
        /// @param _updateCallback pointer to a function for updating the DataPool. Recieves time passed since the last update and a reference to the DataManager.
        DataManager(size_t _maxSize, void (*_updateCallback) (float, DataManager<T>&)) 
            : m_dataPool(_maxSize), m_updateCallback(_updateCallback) {
                
        }

        /// @brief Calls the update Callback
        /// @param delta time passed since last update
        ///
        /// @throws "std::runtime_error" if the updateCallback is a nullptr
        void update(float delta){
            if (m_updateCallback == nullptr){
                throw std::runtime_error(FUNCTION_EXCEPTION_INFO("UpdateCallback is a nullptr"));
            }

            m_updateCallback(delta, *this);
        }

        /// @brief Get pointer to DataPool
        /// @return m_dataPool direction
        DataPool<T>* getDataPool(){
            return &m_dataPool;
        }

        /// @brief For observer pattern. Future implementation is required;
        void notify(){
            
        }
};