#pragma once
#include <inttypes.h>
#include <cxxabi.h>
#include <exception>
#include <string>
#include <sstream>

///@brief Macro for obtaining a class' method + an exception description. (class::method() : ${exception Description})
#define FUNCTION_EXCEPTION_INFO(exceptionDescription) std::string (abi::__cxa_demangle(typeid(this).name(), 0, 0, 0)) + std::string ("::") + std::string(__FUNCTION__) + std::string("() : ") + std::string (exceptionDescription)
using dataID_t = size_t;

/// @brief Basic iterator Implementation for a DataPool
/// @tparam T class type
template<class T>
struct IteratorDataPool
{

    /// @brief pointer to value
    T* ptr;

    /// @brief Class constructor
    /// @param _ptr data Pointer
    IteratorDataPool(T* _ptr) : ptr(_ptr) {}
    
    
    /// @return Pointer to next value
    IteratorDataPool<T>& operator ++(){
        ptr++;
        return *this;
    }

    /// @return Pointer to actual value copy before incrementing
    IteratorDataPool<T> operator ++(int) {
        auto it = *this;
        ptr++;
        return it;
    }

    /// @return Dereferenced pointer
    T& operator *(){
        return *ptr;
    }

    /// @return true if not equal
    bool operator !=(IteratorDataPool<T>& it){
        return ptr != it.ptr;
    }

    /// @return true if equal
    bool operator ==(IteratorDataPool<T>& it){
        return ptr == it.ptr;
    }
};

/**
 * @brief Container class for fast creation and fast access to data of any kind.
 * 
 *  Having the Data Oriented Design in mind. 
 *  For best results you should have the data completely separated from the algorithms.
 * 
 * @tparam T class type
*/
template<class T>
class DataPool
{
    private:
        /// @brief Data Array
        T* m_dataArray;

        /// @brief Data Array size
        size_t m_size;

        /// @brief Max size the Data Array can have
        size_t m_maxSize;

        /// @brief Array of free indexes of the data array ready for reusability
        dataID_t* m_freeIndexes;

        /// @brief Number of free indexes in the array
        size_t m_freeIndexesNumber;

        /// @brief Boolean array having the availableness of an index
        bool* m_isFree;

    public:
        /// @brief Class Constructor
        /// @param _size Max Number of elements the pool can have
        DataPool(size_t _size) : m_size(0), m_maxSize(_size), m_freeIndexesNumber(0) {
            m_dataArray = new T[_size]();
            m_isFree = new bool[_size];
            m_freeIndexes = new dataID_t[_size];
        }

        /// @brief Get a data's reference by it's dataID
        /// @param _id DataID
        /// @return data's reference
        ///
        /// @throws "std::runtime_error" if the dataID is not available
        T& get(dataID_t _id){
            if (isFree(_id)){
                throw std::runtime_error(FUNCTION_EXCEPTION_INFO(std::string("Not available DataID: ") + std::to_string(_id)));
            }

            return m_dataArray[_id];
        }

        /// @brief  Get a free data's reference. Changes it's state from free to used. 
        /// @return free data's reference
        ///
        /// @throws "std::runtime_error" if the Pool is full
        T& getFreeData(){
            if (m_size >= m_maxSize){
                throw std::runtime_error(FUNCTION_EXCEPTION_INFO(std::string("No more Data Available")));
            }

            if (m_freeIndexesNumber > 0){
                m_isFree[m_freeIndexes[--m_freeIndexesNumber]] = false;
                return m_dataArray[m_freeIndexes[m_freeIndexesNumber]];
            }

            m_isFree[m_size] = false;
            return m_dataArray[m_size++];
        }

        /// @brief Free a data by it's dataID
        /// @param _id data's id
        ///
        /// @throws "std::runtime_error" if the id is not valid or the data has already been fred
        void freeData(dataID_t _id){
            if(_id > m_size){
                throw std::runtime_error(FUNCTION_EXCEPTION_INFO(std::string("Not valid DataID: ") + std::to_string(_id)));
            }
            if(m_isFree[_id]){
                throw std::runtime_error(FUNCTION_EXCEPTION_INFO(std::string("The data has already been fred")));
            }

            m_isFree[_id] = true;

            m_freeIndexes[m_freeIndexesNumber++] = _id;
            
        }

        /// @brief Get availableness of data
        /// @param _id data's ID
        /// @return availableness of data
        inline bool isFree(dataID_t _id){
            return m_isFree[_id];
        }

        /// @brief Get current size of the pool
        /// @return size of the pool
        inline size_t size(){
            return m_size;
        }

        /// @brief Get a data's reference by it's dataID
        /// @param _id DataID
        /// @return data's reference
        ///
        /// @throws "std::runtime_error" if the dataID is not available
        T& operator [] (dataID_t _id){
            return get(_id);
        }

        /// @brief Useful for iteration through the data
        /// @return Iterator to begin of the data Array
        IteratorDataPool<T> begin(){
            return IteratorDataPool<T>(m_dataArray);
        }

        /// @brief Useful for iteration trhough the data
        /// @return Iterator to the end of Data Array
        IteratorDataPool<T> end(){
            return IteratorDataPool<T>(m_dataArray+m_size);
        }

        //debug
        
        /// @brief for Debug purposes
        /// @return string having value's list
        std::string getListStr(){
            std::stringstream ss;

            for(int i = 0; i < m_size; i++){
                ss << (isFree(i)? -1 : m_dataArray[i]) << ' ';
            }

            return ss.str();
        }
};