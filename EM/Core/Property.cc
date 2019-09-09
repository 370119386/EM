
#include "Property.h"
#include <complex>
using namespace EM;

Property::Property()
{
	mbPublic = false;
	mbPrivate = false;
	mbSave = false;
	mbCache = false;
	mbRef = false;
	mbForce = false;
	mbUpload = false;

	mSelf = GUID();
	eType = TDATA_UNKNOWN;

	msPropertyName = "";
}

Property::Property(const GUID& self, const std::string& strPropertyName, const DATA_TYPE varType)
{
	mbPublic = false;
	mbPrivate = false;
	mbSave = false;
	mbCache = false;
	mbRef = false;
	mbUpload = false;

	mSelf = self;

	msPropertyName = strPropertyName;
	eType = varType;
}

Property::~Property()
{
	for (TPROPERTYCALLBACKEX::iterator iter = mtPropertyCallback.begin(); iter != mtPropertyCallback.end(); ++iter)
	{
		iter->reset();
	}

	mtPropertyCallback.clear();
	mxData.reset();
}

void Property::SetValue(const Data& xData)
{
	if (eType != xData.GetType()
		|| xData.GetType() == DATA_TYPE::TDATA_UNKNOWN)
	{
		return;
	}

	if (xData.IsNullValue())
	{
		return;
	}

	if (nullptr == mxData)
	{
		mxData = EM_SHARE_PTR<Data>(EM_NEW Data(xData));
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->variantData = xData.variantData;
	}
	else
	{
		Data oldValue;
		oldValue = *mxData;

		mxData->variantData = xData.variantData;

		Data newValue;
		newValue = *mxData;

		OnEventHandler(oldValue, newValue);
	}

}

void Property::SetValue(const IProperty* pProperty)
{
	SetValue(pProperty->GetValue());
}

const Data& Property::GetValue() const
{
	if (mxData)
	{
		return *mxData;
	}

	return NULL_TDATA;
}

const std::string&  Property::GetKey() const
{
	return msPropertyName;
}

const bool Property::GetSave() const
{
	return mbSave;
}

const bool Property::GetPublic() const
{
	return mbPublic;
}

const bool Property::GetPrivate() const
{
	return mbPrivate;
}

const bool Property::GetCache() const
{
	return mbCache;
}

const bool Property::GetRef() const
{
	return mbRef;
}

const bool Property::GetForce() const
{
	return mbForce;
}

const bool Property::GetUpload() const
{
	return mbUpload;
}

void Property::SetSave(bool bSave)
{
	mbSave = bSave;
}

void Property::SetPublic(bool bPublic)
{
	mbPublic = bPublic;
}

void Property::SetPrivate(bool bPrivate)
{
	mbPrivate = bPrivate;
}

void Property::SetCache(bool bCache)
{
    mbCache = bCache;
}

void Property::SetRef(bool bRef)
{
	mbRef = bRef;
}

void Property::SetForce(bool bRef)
{
	mbForce = bRef;
}

void Property::SetUpload(bool bUpload)
{
	mbUpload = bUpload;
}

INT64 Property::GetInt() const
{
	if (!mxData)
	{
		return 0;
	}

	return mxData->GetInt();
}

int Property::GetInt32() const
{
	if (!mxData)
	{
		return 0;
	}

	return (int)mxData->GetInt();
}


double Property::GetFloat() const
{
	if (!mxData)
	{
		return 0.0;
	}

	return mxData->GetFloat();
}

const std::string& Property::GetString() const
{
	if (!mxData)
	{
		return NULL_STR;
	}

	return mxData->GetString();
}

const GUID& Property::GetObject() const
{
	if (!mxData)
	{
		return NULL_OBJECT;
	}

	return mxData->GetObject();
}

const Vector2& Property::GetVector2() const
{
	if (!mxData)
	{
		return NULL_VECTOR2;
	}

	return mxData->GetVector2();
}

const Vector3& Property::GetVector3() const
{
	if (!mxData)
	{
		return NULL_VECTOR3;
	}

	return mxData->GetVector3();
}

void Property::RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
	mtPropertyCallback.push_back(cb);
}

int Property::OnEventHandler(const Data& oldVar, const Data& newVar)
{
	if (mtPropertyCallback.size() <= 0)
	{
		return 0;
	}

	TPROPERTYCALLBACKEX::iterator it = mtPropertyCallback.begin();
	TPROPERTYCALLBACKEX::iterator end = mtPropertyCallback.end();
	for (; it != end; ++it)
	{
		
		PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(mSelf, msPropertyName, oldVar, newVar);
	}

	return 0;
}

bool Property::SetInt(const INT64 value)
{
	if (eType != TDATA_INT)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (0 == value)
		{
			return false;
		}

		mxData = EM_SHARE_PTR<Data>(EM_NEW Data(TDATA_INT));
		mxData->SetInt(0);
	}

	if (value == mxData->GetInt())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetInt(value);
	}
	else
	{
		Data oldValue;
		oldValue = *mxData;

		mxData->SetInt(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool Property::SetFloat(const double value)
{
	if (eType != TDATA_FLOAT)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (IsZeroDouble(value))
		{
			return false;
		}

		mxData = EM_SHARE_PTR<Data>(EM_NEW Data(TDATA_FLOAT));
		mxData->SetFloat(0.0);
	}

	if (IsZeroDouble(value - mxData->GetFloat()))
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetFloat(value);
	}
	else
	{
		Data oldValue;
		oldValue = *mxData;

		mxData->SetFloat(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool Property::SetString(const std::string& value)
{
	if (eType != TDATA_STRING)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.empty())
		{
			return false;
		}

		mxData = EM_SHARE_PTR<Data>(EM_NEW Data(TDATA_STRING));
		mxData->SetString(NULL_STR);
	}

	if (value == mxData->GetString())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetString(value);
	}
	else
	{
		Data oldValue;
		oldValue = *mxData;

		mxData->SetString(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool Property::SetObject(const GUID& value)
{
	if (eType != TDATA_OBJECT)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.IsNull())
		{
			return false;
		}

		mxData = EM_SHARE_PTR<Data>(EM_NEW Data(TDATA_OBJECT));
		mxData->SetObject(GUID());
	}

	if (value == mxData->GetObject())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetObject(value);
	}
	else
	{
		Data oldValue;
		oldValue = *mxData;

		mxData->SetObject(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool Property::SetVector2(const Vector2& value)
{
	if (eType != TDATA_VECTOR2)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.IsZero())
		{
			return false;
		}

		mxData = EM_SHARE_PTR<Data>(EM_NEW Data(TDATA_VECTOR2));
		mxData->SetVector2(Vector2());
	}

	if (value == mxData->GetVector2())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetVector2(value);
	}
	else
	{
		Data oldValue;
		oldValue = *mxData;

		mxData->SetVector2(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool Property::SetVector3(const Vector3& value)
{
	if (eType != TDATA_VECTOR3)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.IsZero())
		{
			return false;
		}

		mxData = EM_SHARE_PTR<Data>(EM_NEW Data(TDATA_VECTOR3));
		mxData->SetVector3(Vector3());
	}

	if (value == mxData->GetVector3())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetVector3(value);
	}
	else
	{
		Data oldValue;
		oldValue = *mxData;

		mxData->SetVector3(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool Property::Changed() const
{
	return !(GetValue().IsNullValue());
}

const DATA_TYPE Property::GetType() const
{
	return eType;
}

const bool Property::GeUsed() const
{
	if (mxData)
	{
		return true;
	}

	return false;
}

std::string Property::ToString()
{
	std::string strData;
	const DATA_TYPE eType = GetType();
	switch (eType)
	{
	case TDATA_INT:
		strData = lexical_cast<std::string> (GetInt());
		break;
	case TDATA_FLOAT:
		strData = lexical_cast<std::string> (GetFloat());
		break;
	case TDATA_STRING:
		strData = GetString();
		break;
	case TDATA_OBJECT:
		strData = GetObject().ToString();
		break;
	case TDATA_VECTOR2:
		strData = GetVector2().ToString();
		break;
	case TDATA_VECTOR3:
		strData = GetVector3().ToString();
		break;
	default:
		strData = NULL_STR;
		break;
	}

	return strData;
}

bool Property::FromString(const std::string& strData)
{
	try
	{
		switch (GetType())
		{
		case TDATA_INT:
			SetInt(lexical_cast<int64_t> (strData));
			break;

		case TDATA_FLOAT:
			SetFloat(lexical_cast<float> (strData));
			break;

		case TDATA_STRING:
			SetString(strData);
			break;

		case TDATA_OBJECT:
		{
			GUID xID;
			xID.FromString(strData);
			SetObject(xID);
		}
		break;

		case TDATA_VECTOR2:
		{
			Vector2 v;
			v.FromString(strData);
			SetVector2(v);
		}
		break;

		case TDATA_VECTOR3:
		{
			Vector3 v;
			v.FromString(strData);
			SetVector3(v);
		}
		break;

		default:
			break;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool Property::DeSerialization()
{
	bool bRet = false;

	const DATA_TYPE eType = GetType();
	if (eType == TDATA_STRING && nullptr != mxData && !mxData->IsNullValue())
	{
		DataList xDataList;
		const std::string& strData = mxData->GetString();

		xDataList.Split(strData.c_str(), ";");
		if (xDataList.GetCount() <= 0)
		{
			return bRet;
		}

		DataList xTemDataList;
		xTemDataList.Split(xDataList.String(0).c_str(), ",");
		const int nSubDataLength = xTemDataList.GetCount();

		if (xDataList.GetCount() == 1 && nSubDataLength == 1)
		{
			//most of property value only one value
			return bRet;
		}

		if (nullptr == mxEmbeddedList)
		{
			mxEmbeddedList = EM_SHARE_PTR<List<std::string>>(EM_NEW List<std::string>());
		}
		else
		{
			mxEmbeddedList->ClearAll();
		}

		for (int i = 0; i < xDataList.GetCount(); ++i)
		{
			if (xDataList.String(i).empty())
			{
				EMASSERT(0, strData, __FILE__, __FUNCTION__);
			}

			mxEmbeddedList->Add(xDataList.String(i));
		}



		////////////////////////////////////////

		if (nSubDataLength < 2 || nSubDataLength > 2)
		{
			return bRet;
		}

		if (nullptr == mxEmbeddedMap)
		{
			mxEmbeddedMap = EM_SHARE_PTR<MapExtend<std::string, std::string>>(EM_NEW MapExtend<std::string, std::string>());
		}
		else
		{
			mxEmbeddedMap->ClearAll();
		}

		for (int i = 0; i < xDataList.GetCount(); ++i)
		{
			DataList xTemDataList;
			const std::string& strTemData = xDataList.String(i);
			xTemDataList.Split(strTemData.c_str(), ",");
			{
				if (xTemDataList.GetCount() != nSubDataLength)
				{
					EMASSERT(0, strTemData, __FILE__, __FUNCTION__);
				}

				const std::string& strKey = xTemDataList.String(0);
				const std::string& strValue = xTemDataList.String(1);

				if (strKey.empty() || strValue.empty())
				{
					EMASSERT(0, strTemData, __FILE__, __FUNCTION__);
				}

				mxEmbeddedMap->AddElement(strKey, EM_SHARE_PTR<std::string>(EM_NEW std::string(strValue)));
			}
		}

		bRet = true;
	}

	return bRet;
}

const EM_SHARE_PTR<List<std::string>> Property::GetEmbeddedList() const
{
	return this->mxEmbeddedList;
}

const EM_SHARE_PTR<MapExtend<std::string, std::string>> Property::GetEmbeddedMap() const
{
	return this->mxEmbeddedMap;
}
