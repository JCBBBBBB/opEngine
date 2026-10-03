#pragma once
#include "opResource.h"

namespace op
{
	// 여러 리소스를 관리하는 클래스
	class Resources
	{
	public:
		template <typename T>
		static T* Find(const std::wstring& key)
		{
			auto iter = m_resources.find(key);
			if (iter == m_resources.end())
			{
				return nullptr;
			}

			return dynamic_cast<T*>(iter->second);
		}

		template <typename T>
		static T* Load(const std::wstring& key, const std::wstring& path)
		{
			T* resource = Resources::Find<T>(key);
			if (resource != nullptr)
			{
				return resource;
			}

			resource = new T();
			if (FAILED(resource->Load(path)))
			{
				assert(false);
			}

			resource->SetName(key);
			resource->SetPath(path);
			m_resources.insert(std::make_pair(key, resource));

			return resource;
		}

	private:
		static std::map<std::wstring, Resource*> m_resources;
	};
}