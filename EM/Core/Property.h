#ifndef __EM__PROPERTY__H__
#define __EM__PROPERTY__H__

#include "Map.hpp"
#include "List.hpp"
#include "DataList.hpp"
#include "MapExtend.hpp"

#include "IProperty.h"
#include "../Common/Platform.h"

namespace EM
{
    class _EMExport Property : public IProperty
    {
    private:
        Property();

    public:
        Property(const GUID& self, const std::string& strPropertyName, const DATA_TYPE varType);

        virtual ~Property();

        virtual void SetValue(const Data& TData);
        virtual void SetValue(const IProperty* pProperty);

        virtual bool SetInt(const INT64 value);
        virtual bool SetFloat(const double value);
        virtual bool SetString(const std::string& value);
        virtual bool SetObject(const GUID& value);
        virtual bool SetVector2(const Vector2& value);
        virtual bool SetVector3(const Vector3& value);

        virtual const DATA_TYPE GetType() const;
        virtual const bool GeUsed() const;
        virtual const std::string& GetKey() const;
        virtual const bool GetSave() const;
        virtual const bool GetPublic() const;
        virtual const bool GetPrivate() const;
        virtual const bool GetCache() const;
        virtual const bool GetRef() const;
        virtual const bool GetForce() const;
        virtual const bool GetUpload() const;

        virtual void SetSave(bool bSave);
        virtual void SetPublic(bool bPublic);
        virtual void SetPrivate(bool bPrivate);
        virtual void SetCache(bool bCache);
        virtual void SetRef(bool bRef);
        virtual void SetForce(bool bRef);
        virtual void SetUpload(bool bUpload);

        virtual INT64 GetInt() const;
        virtual int GetInt32() const;
        virtual double GetFloat() const;
        virtual const std::string& GetString() const;
        virtual const GUID& GetObject() const;
        virtual const Vector2& GetVector2() const;
        virtual const Vector3& GetVector3() const;

        virtual const Data& GetValue() const;
        virtual const EM_SHARE_PTR<List<std::string>> GetEmbeddedList() const;
        virtual const EM_SHARE_PTR<MapExtend<std::string, std::string>> GetEmbeddedMap() const;

        virtual bool Changed() const;

        virtual std::string ToString();
        virtual bool FromString(const std::string& strData);
        virtual bool DeSerialization();

        virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    private:
        int OnEventHandler(const Data& oldVar, const Data& newVar);

    private:
        typedef std::vector<PROPERTY_EVENT_FUNCTOR_PTR> TPROPERTYCALLBACKEX;
        TPROPERTYCALLBACKEX mtPropertyCallback;

        GUID mSelf;
        std::string msPropertyName;
        DATA_TYPE eType;

        EM_SHARE_PTR<Data> mxData;
        EM_SHARE_PTR<MapExtend<std::string, std::string>> mxEmbeddedMap;
        EM_SHARE_PTR<List<std::string>> mxEmbeddedList;

        bool mbPublic;
        bool mbPrivate;
        bool mbSave;
        bool mbCache;
        bool mbRef;
        bool mbForce;
        bool mbUpload;
    };
}

#endif//__EM__PROPERTY__H__
