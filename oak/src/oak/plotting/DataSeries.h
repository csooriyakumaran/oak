#pragma once

#include "oakpch.h"
#include "oak/core/UUID.h"
#include "implot.h"

    
namespace Oak
{  

    template<typename T>
    struct DataSeries : public Oak::RefCounted
    {

        uint32_t            SeriesID   = 0;
        std::string         Name       = "";
        uint32_t            Buffersize = 0;
        uint32_t            Offset     = 0;
        std::vector<T>      buff;
        std::vector<T>      x;
        std::vector<T>      y;
        ImVec4              Color;


        DataSeries() : Buffersize(0) , Color(ImVec4(0,0,0,0)) {}
        DataSeries(uint32_t _buffersize);
        DataSeries(const char* _name, uint32_t _buffersize);
        ~DataSeries();
        
        DataSeries(const DataSeries<T>& other);
        DataSeries(DataSeries<T>&& other) noexcept;
        
        DataSeries& operator=(DataSeries<T>& other);
        const DataSeries& operator=(const DataSeries<T>& other);
        DataSeries& operator=(DataSeries<T>&& other) noexcept;
        
        virtual void OnUpdate();
        void AddPoint(T _x, T _y);
        void Erase();

        static uint32_t GetNextID() { return idGenerator; }
    private:
        static inline uint32_t idGenerator = 0;
    };
    
    template<typename T>
    DataSeries<T>::DataSeries(uint32_t _buffersize)
        : SeriesID(++idGenerator), Buffersize(_buffersize), Color(ImPlot::GetColormapColor(SeriesID))
    {
        OAK_CORE_TRACE_TAG("DataSeries","Created with size {0}", Buffersize );
        buff.reserve(Buffersize);
        x.reserve(Buffersize);
        y.reserve(Buffersize);
    }



    template<typename T>
    DataSeries<T>::DataSeries(const char* _name, uint32_t _buffersize)
        : SeriesID(++idGenerator), Name(_name), Buffersize(_buffersize), Color(ImPlot::GetColormapColor(SeriesID))
    {
        OAK_CORE_TRACE_TAG("DataSeries", "Created with size {0}, ID {1}, Name '{2}'", Buffersize, SeriesID, Name);  
        buff.reserve(Buffersize);
        x.reserve(Buffersize);
        y.reserve(Buffersize);
    }

    template<typename T>
    DataSeries<T>::~DataSeries()
    {
        OAK_CORE_TRACE_TAG("DataSeries", "Destryed {0} ('{1}') References Remaining: {2}", SeriesID, Name, GetRefCount());
    }

    template<typename T>
    DataSeries<T>::DataSeries(const DataSeries<T>& other)
        : SeriesID(other.SeriesID), Name(other.Name), Buffersize(other.Buffersize), Offset(other.Offset), x(other.x), y(other.y), Color(other.Color)
    {
        OAK_CORE_TRACE_TAG("DataSeries", "Copied {0} ('{1}')", SeriesID, Name);
    }
    
    template<typename T>
    DataSeries<T>::DataSeries(DataSeries<T>&& other) noexcept
        : SeriesID(other.SeriesID), Name(other.Name), Buffersize(other.Buffersize), Offset(other.Offset), Color(other.Color)
    {
        
        std::move(other.x.begin(), other.x.end(), x.begin());
        std::move(other.y.begin(), other.y.end(), y.begin());

        OAK_CORE_TRACE_TAG("DataSeries", "Moved {0} ('{1}')", SeriesID, Name);
        other.SeriesID = 0;
        other.Name = "";
        other.Buffersize = 0;
        other.Offset = 0;
        other.buff.clear();
        other.x.clear();
        other.y.clear();
        other.Color = ImVec4(0, 0, 0, 0);
    }

    template<typename T>
    DataSeries<T>& DataSeries<T>::operator=(DataSeries<T>& other) 
    {
        SeriesID = other.SeriesID;
        Name = other.Name;
        Buffersize = other.Buffersize;
        Offset = other.Offset;
        x = other.x;
        y = other.y;
        Color = other.Color;
        OAK_CORE_TRACE_TAG("DataSeries", "copy assigning {0} ('{1}')", SeriesID, Name);
        return *this;
    }

    template<typename T>
    const DataSeries<T>& DataSeries<T>::operator=( const DataSeries<T>& other )
    {
        SeriesID = other.SeriesID;
        Name = other.Name;
        Buffersize = other.Buffersize;
        Offset = other.Offset;
        x = other.x;
        y = other.y;
        Color = other.Color;
        OAK_CORE_TRACE_TAG("DataSeries", "copy assigning {0} ('{1}')", SeriesID, Name);
        return *this;
    }

    template<typename T>
    DataSeries<T>& DataSeries<T>::operator=(DataSeries<T>&& other) noexcept
    {
        if (this != &other)
        {
            SeriesID   = std::move(other.SeriesID);
            Name       = std::move(other.Name);
            Buffersize = std::move(other.Buffersize);
            Offset     = std::move(other.Offset);
            std::move(other.x.begin(), other.x.end(), x.begin());
            std::move(other.y.begin(), other.y.end(), y.begin());
            Color.x = std::move(other.Color.x);
            Color.y = std::move(other.Color.y);
            Color.z = std::move(other.Color.z);
            Color.w = std::move(other.Color.w);

            other.SeriesID = 0;
            other.Name = "";
            other.Buffersize = 0;
            other.Offset = 0;
            other.buff.clear();
            other.x.clear();
            other.y.clear();
            other.Color = ImVec4(0, 0, 0, 0)
        }
        OAK_CORE_TRACE_TAG("DataSeries", "Move assigning {0} ('{1}')", SeriesID, Name);
        return *this;
    }

    template<typename T>
    void DataSeries<T>::AddPoint(T _x, T _y)
    {
        if (x.size() < Buffersize)
        {
            x.push_back(_x);
            y.push_back(_y);
            return;
        }
        else
        {
            if (Buffersize == 0)
                return;
            x[Offset] = _x;
            y[Offset] = _y;
            Offset = (Offset + 1) % Buffersize;
        }
        //Buffersize += 10000;
        //OAK_CORE_WARN_TAG("DataSeries::AddPoint", "Increasing Buffersie of {0} ('{1}') to {2}", SeriesID, Name, Buffersize);
        //x.reserve(Buffersize);
        //y.reserve(Buffersize);
        //x.push_back(_x);
        //y.push_back(_y);
    }
   
    template<typename T>
    void DataSeries<T>::Erase()
    {
        if (x.size() > 0 || y.size() > 0)
        {
            buff.clear();
            x.clear();
            y.clear();
            Offset = 0;
        }
    }

    template<typename T>
    void DataSeries<T>::OnUpdate()
    {
        OAK_CORE_WARN_TAG("DataSeries::OnUpdate", "Virtual Function not implemented");
    }
}